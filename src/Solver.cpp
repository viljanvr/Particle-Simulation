#include <vector>
#include "Constraint.h"
#include "Force.h"
#include "JWJ.h"
#include "Particle.h"
#include "SparseMatrix.h"

#define DAMP 0.99f
#define RAND (((rand() % 2000) / 1000.f) - 1.f)

#define DIMS 2 // Particle dimension
#define KS 0.5
#define KD 0.5

void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> fVector, std::vector<Constraint *> cVector,
                     float dt) {

    for (auto p: pVector) { // Reset forces
        p->m_Forces = Vec2f(0.0, 0.0);
    }
    for (auto f: fVector) { // Recalculate forces
        f->applyForce();
    }

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

    // JDeriv: (#const, 2 * #particles), QDeriv: (2 * #particles) -> #const
    double jDerivQDeriv[cVector.size()];
    JDeriv.matVecMult(qDeriv, jDerivQDeriv);


    // J: (#const, 2 * #particles), WQ: (2 * #particles) -> #const
    double jwq[cVector.size()];
    J.matVecMult(wq, jwq);

    double right_hand_side[cVector.size()];
    for (size_t i = 0; i < cVector.size(); ++i) {
        right_hand_side[i] = (-jDerivQDeriv[i] - jwq[i]) - cVector[i]->getC() * KS - cVector[i]->getCDeriv() * KD;
    }


    double lambda[cVector.size()];

    int steps = 10;
    ConjGrad(cVector.size(), &jwj, lambda, right_hand_side, 0.00000000001, &steps);

    double qHat[pVector.size() * DIMS];
    J.matTransVecMult(lambda, qHat);


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
