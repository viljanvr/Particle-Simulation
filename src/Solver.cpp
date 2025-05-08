#include <vector>
#include "Constraint.h"
#include "Force.h"
#include "JWJ.h"
#include "Particle.h"
#include "SparseMatrix.h"

#define DAMP 0.99f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)

#define DIMS 2 // Particle dimension

void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> fVector, std::vector<Constraint *> cVector,
                     float dt) {

    for (auto p: pVector) { // Reset forces
        p->m_Forces = Vec2f(0.0, 0.0);
    }
    for (auto f: fVector) { // Recalculate forces
        f->applyForce();
    }

    SparseMatrix J(pVector.size() * DIMS, cVector.size());
    SparseMatrix JDeriv(pVector.size() * DIMS, cVector.size());

    for (auto c: cVector) {
        for (auto jacobianEntry: c->getJacobian()) {
            J.addCell(jacobianEntry.p->m_index * DIMS, c->m_index, jacobianEntry.x);
            J.addCell(jacobianEntry.p->m_index * DIMS + 1, c->m_index, jacobianEntry.y);
        }
        for (auto jacobianEntry: c->getJacobianDeriv()) {
            JDeriv.addCell(jacobianEntry.p->m_index * DIMS, c->m_index, jacobianEntry.x);
            JDeriv.addCell(jacobianEntry.p->m_index * DIMS + 1, c->m_index, jacobianEntry.y);
        }
    }

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

    JWJ jwj(J, W);

    double jDerivQDeriv[pVector.size() * DIMS];
    JDeriv.matVecMult(qDeriv, jDerivQDeriv);

    double jwq[pVector.size() * DIMS];
    J.matVecMult(wq, jwq);

    double right_hand_side[pVector.size() * DIMS];
    for (size_t i = 0; i < pVector.size() * DIMS; ++i) {
        right_hand_side[i] = - jDerivQDeriv[i] - jwq[i];
    }

    double lambda[pVector.size() * DIMS];

    ConjGrad(pVector.size() * DIMS, &jwj, lambda, right_hand_side, 0.1, NULL);

    double qHat[pVector.size() * DIMS];
    J.matVecMult(lambda, qHat);

    for (size_t i = 0; i < pVector.size(); ++i) {
        auto p = pVector[i];
        p->m_Forces[0] += qHat[2 * i];
        p->m_Forces[1] += qHat[2 * i + 1];
    }


    for (size_t ii = 0; ii < pVector.size(); ii++) {
        pVector[ii]->m_Position += dt * pVector[ii]->m_Velocity; // Euler
        // Vec2f random_move = DAMP*pVector[ii]->m_Velocity + Vec2f(RAND,RAND) * 0.005;	// Random movement
        pVector[ii]->m_Velocity = pVector[ii]->m_Velocity + dt * (pVector[ii]->m_Forces / pVector[ii]->m_Mass);
    }
}
