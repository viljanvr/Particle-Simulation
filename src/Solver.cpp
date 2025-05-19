#include <EulerScheme.h>
#include <vector>
#include "CollideableObject.h"
#include "Constraint.h"
#include "Force.h"
#include "JWJ.h"
#include "Particle.h"
#include "SparseMatrix.h"

#define DAMP 0.99f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)

#define DIMS 2 // Particle dimension
#define KS 1.0
#define KD 1.0

void reset_particle_forces(const std::vector<Particle *> &pVector) {
    for (const auto particle: pVector) {
        particle->m_Forces = Vec2f(0.0, 0.0);
    }
}

void apply_forces_to_particles(const std::vector<Force *> &fVector) {
    for (const auto force: fVector) {
        force->applyForce();
    }
}

void apply_constraint_forces_to_particles(std::vector<Particle *> pVector, std::vector<Constraint *> cVector) {
    SparseMatrix J(cVector.size(), pVector.size() * DIMS);
    SparseMatrix JDeriv(cVector.size(), pVector.size() * DIMS);

    for (auto c: cVector) {
        for (auto jacobianEntry: c->getJacobian()) {
            J.addToCell(c->m_index, jacobianEntry.p->m_index * DIMS, jacobianEntry.x);
            J.addToCell(c->m_index, jacobianEntry.p->m_index * DIMS + 1, jacobianEntry.y);
        }
        for (auto jacobianEntry: c->getJacobianDeriv()) {
            JDeriv.addToCell(c->m_index, jacobianEntry.p->m_index * DIMS, jacobianEntry.x);
            JDeriv.addToCell(c->m_index, jacobianEntry.p->m_index * DIMS + 1, jacobianEntry.y);
        }
    }

    std::vector<double> W;
    std::vector<double> qDeriv(pVector.size() * DIMS);
    std::vector<double> wq(pVector.size() * DIMS);
    for (size_t i = 0; i < pVector.size(); ++i) {
        Particle *p = pVector[i];

        W.emplace_back(1.0 / p->m_Mass);
        W.emplace_back(1.0 / p->m_Mass);

        qDeriv[2 * i] = p->m_Velocity[0];
        qDeriv[2 * i + 1] = p->m_Velocity[1];

        wq[2 * i] = p->m_Forces[0] / p->m_Mass;
        wq[2 * i + 1] = p->m_Forces[1] / p->m_Mass;
    }


    JWJ jwj(J, W);

    // JDeriv: (#const, 2 * #particles), QDeriv: (2 * #particles) -> #const
    std::vector<double> jDerivQDeriv = JDeriv.matVecMult(qDeriv);


    // J: (#const, 2 * #particles), WQ: (2 * #particles) -> #const
    std::vector<double> jwq = J.matVecMult(wq);


    std::vector<double> right_hand_side(cVector.size());
    for (size_t i = 0; i < cVector.size(); ++i) {
        right_hand_side[i] = (-jDerivQDeriv[i] - jwq[i]) - cVector[i]->getC() * KS - cVector[i]->getCDeriv() * KD;
    }

    int steps = 0;
    std::vector<double> lambda(cVector.size());
    ConjGrad(cVector.size(), &jwj, lambda.data(), right_hand_side.data(), 0.000000001, &steps);

    if (steps >= 20) {
        std::cout << "ConjGrad took many steps: " << std::endl;
        std::cout << steps << std::endl;
    }

    std::vector<double> qHat = J.matTransVecMult(lambda);

    for (size_t i = 0; i < pVector.size(); ++i) {
        auto p = pVector[i];
        p->m_Forces[0] += qHat[2 * i];
        p->m_Forces[1] += qHat[2 * i + 1];
    }
}

void compute_total_forces(const std::vector<Particle *> &pVector, const std::vector<Force *> &fVector,
                          const std::vector<Constraint *> &cVector) {
    reset_particle_forces(pVector);

    apply_forces_to_particles(fVector);

    apply_constraint_forces_to_particles(pVector, cVector);
}

SparseMatrix getForceJacobianX(const std::vector<Particle *> &pVector, const std::vector<Force *> &fVector) {
    size_t n = pVector.size() * DIMS;
    SparseMatrix j(n, n);
    for (auto force: fVector) {
        for (auto jacobianEntry: force->getJx()) {
            j.addToCell(jacobianEntry.p1->m_index * 2, jacobianEntry.p2->m_index * 2, jacobianEntry.xx);
            j.addToCell(jacobianEntry.p1->m_index * 2, jacobianEntry.p2->m_index * 2 + 1, jacobianEntry.xy);
            j.addToCell(jacobianEntry.p1->m_index * 2 + 1, jacobianEntry.p2->m_index * 2, jacobianEntry.yx);
            j.addToCell(jacobianEntry.p1->m_index * 2 + 1, jacobianEntry.p2->m_index * 2 + 1, jacobianEntry.yy);
        }
    }
    return j;
}

SparseMatrix getForceJacobianV(const std::vector<Particle *> &pVector, const std::vector<Force *> &fVector) {
    size_t n = pVector.size() * DIMS;
    SparseMatrix j(n, n);
    for (auto force: fVector) {
        for (auto jacobianEntry: force->getJv()) {
            j.addToCell(jacobianEntry.p1->m_index * 2, jacobianEntry.p2->m_index * 2, jacobianEntry.xx);
            j.addToCell(jacobianEntry.p1->m_index * 2, jacobianEntry.p2->m_index * 2 + 1, jacobianEntry.xy);
            j.addToCell(jacobianEntry.p1->m_index * 2 + 1, jacobianEntry.p2->m_index * 2, jacobianEntry.yx);
            j.addToCell(jacobianEntry.p1->m_index * 2 + 1, jacobianEntry.p2->m_index * 2 + 1, jacobianEntry.yy);
        }
    }
    return j;
}

void handle_collisions(const std::vector<CollideableObject *> &oVector) {
    // We want to backtrack everything to the earliest collision if there are multiple earliest collisions,
    // we handle all of them.
    std::vector<Collision> earliest_collisions;
    std::vector<CollideableObject *> collision_objects;
    // Get earliest collisions among all colliders.
    for (auto o: oVector) {
        std::vector<Collision> collisions = o->get_earliest_collisions();
        if (collisions.empty()) {
            continue;
        }
        double backtracking = collisions.back().backtracking_factor;
        if (!earliest_collisions.empty() && earliest_collisions.back().backtracking_factor > backtracking) {
            continue;
        }
        if (!earliest_collisions.empty() && earliest_collisions.back().backtracking_factor < backtracking) {
            earliest_collisions.clear();
            collision_objects.clear();
        }
        for (auto collision: collisions) {
            earliest_collisions.push_back(collision);
            collision_objects.push_back(o);
        }
    }

    // Backtrack back to earliest collisions.
    if (!earliest_collisions.empty()) {
        for (auto o: oVector) {
            o->backtrack_particles(earliest_collisions.back().backtracking_factor);
        }
    }


    for (size_t i = 0; i < earliest_collisions.size(); ++i) {
        Collision collision = earliest_collisions[i];
        CollideableObject *o = collision_objects[i];
        o->bounce_single_particle(collision);
    }
}


void simulation_step(const std::vector<Particle *> &pVector, const std::vector<Force *> &fVector,
                     const std::vector<Constraint *> &cVector, const std::vector<CollideableObject *> &oVector,
                     const float dt, IntegrationScheme &integration_scheme) {
    for (auto p: pVector) {
        p->m_PreviousPosition = p->m_Position;
        p->m_PreviousVelocity = p->m_Velocity;
    }
    integration_scheme.updateParticlesBasedOnForce(
            pVector, [&]() { compute_total_forces(pVector, fVector, cVector); },
            [&]() { return getForceJacobianX(pVector, fVector); },
            [&]() { return getForceJacobianV(pVector, fVector); }, dt);

    handle_collisions(oVector);
}
