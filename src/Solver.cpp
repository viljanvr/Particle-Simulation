#include <EulerScheme.h>
#include <vector>
#include "Constraint.h"
#include "Force.h"
#include "JWJ.h"
#include "Particle.h"
#include "SparseMatrix.h"

#define DAMP 0.99f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)

#define DIMS 2 // Particle dimension
#define KS 0.0
#define KD 0.0

void reset_particle_forces(std::vector<Particle *> pVector) {
    for (auto particle : pVector) {
        particle->m_Forces = Vec2f(0.0, 0.0);
    }
}

void apply_forces_to_particles(std::vector<Force *> fVector) {
    for (auto force : fVector) {
        force->applyForce();
    }
}

void apply_constraint_forces_to_particles(std::vector<Particle *> pVector, std::vector<Constraint *> cVector) {
    SparseMatrix J(cVector.size(), pVector.size() * DIMS);
    SparseMatrix JDeriv(cVector.size(), pVector.size() * DIMS);

    for (auto c: cVector) {
        for (auto jacobianEntry: c->getJacobian()) {
            J.addCell(c->m_index, jacobianEntry.p->m_index * DIMS, jacobianEntry.x);
            J.addCell(c->m_index, jacobianEntry.p->m_index * DIMS + 1, jacobianEntry.y);
        }
        for (auto jacobianEntry: c->getJacobianDeriv()) {
            JDeriv.addCell(c->m_index, jacobianEntry.p->m_index * DIMS, jacobianEntry.x);
            JDeriv.addCell(c->m_index, jacobianEntry.p->m_index * DIMS + 1, jacobianEntry.y);
        }
    }

    std::cout << "J" << std::endl;
    J.debugPrint();

    std::cout << "JDeriv" << std::endl;
    JDeriv.debugPrint();

    std::vector<double> W;
    double qDeriv[pVector.size() * DIMS];
    double wq[pVector.size() * DIMS];
    for (size_t i = 0; i < pVector.size(); ++i) {
        Particle *p = pVector[i];

        W.emplace_back(1.0 / p->m_Mass);
        W.emplace_back(1.0 / p->m_Mass);

        qDeriv[2 * i] = p->m_Velocity[0];
        qDeriv[2 * i + 1] = p->m_Velocity[1];

        wq[2 * i] = p->m_Forces[0] / p->m_Mass;
        wq[2 * i + 1] = p->m_Forces[1] / p->m_Mass;
    }

    std::cout << "qDeriv" << std::endl;
    for (size_t i = 0; i < pVector.size() * DIMS; ++i) {
        std::cout << qDeriv[i] << std::endl;
    }
    std::cout << "wq" << std::endl;
    for (size_t i = 0; i < pVector.size() * DIMS; ++i) {
        std::cout << wq[i] << std::endl;
    }

    JWJ jwj(J, W);

    // JDeriv: (#const, 2 * #particles), QDeriv: (2 * #particles) -> #const
    double jDerivQDeriv[cVector.size()];
    JDeriv.matVecMult(qDeriv, jDerivQDeriv);

    std::cout << "jDerivQDeriv" << std::endl;
    for (size_t i = 0; i < cVector.size(); ++i) {
        std::cout << jDerivQDeriv[i] << std::endl;
    }

    // J: (#const, 2 * #particles), WQ: (2 * #particles) -> #const
    double jwq[cVector.size()];
    J.matVecMult(wq, jwq);

    std::cout << "jwq" << std::endl;
    for (size_t i = 0; i < cVector.size(); ++i) {
        std::cout << jwq[i] << std::endl;
    }

    double right_hand_side[cVector.size()];
    for (size_t i = 0; i < cVector.size(); ++i) {
        right_hand_side[i] = (-jDerivQDeriv[i] - jwq[i]) - cVector[i]->getC() * KS - cVector[i]->getCDeriv() * KD;
    }

    std::cout << "right_hand_side" << std::endl;
    for (size_t i = 0; i < cVector.size(); ++i) {
        std::cout << right_hand_side[i] << std::endl;
    }


    double lambda[cVector.size()];

    int steps = 0;
    ConjGrad(cVector.size(), &jwj, lambda, right_hand_side, 0.000001, &steps);

    std::cout << "lambda" << std::endl;
    for (size_t i = 0; i < cVector.size(); ++i) {
        std::cout << lambda[i] << std::endl;
    }
    std::cout << "steps" << std::endl;
    std::cout << steps << std::endl;

    double qHat[pVector.size() * DIMS];
    J.matTransVecMult(lambda, qHat);

    std::cout << "qHat" << std::endl;
    for (size_t i = 0; i < pVector.size() * DIMS; ++i) {
        std::cout << qHat[i] << std::endl;
    }


    for (size_t i = 0; i < pVector.size(); ++i) {
        auto p = pVector[i];
        p->m_Forces[0] += qHat[2 * i];
        p->m_Forces[1] += qHat[2 * i + 1];
    }
    std::cout << std::endl << std::endl << "=============" << std::endl << std::endl;
}

void compute_total_forces(std::vector<Particle *> pVector, std::vector<Force *> fVector, std::vector<Constraint* > cVector) {
    reset_particle_forces(pVector);

    apply_forces_to_particles(fVector);

    apply_constraint_forces_to_particles(pVector, cVector);
}

void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> fVector, std::vector<Constraint *> cVector,
                     float dt, IntegrationScheme& integration_scheme) {

    integration_scheme.updateParticlesBasedOnForce(pVector, [&]() {
        compute_total_forces(pVector, fVector, cVector);
    } ,dt);
}
