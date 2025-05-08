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
    int ii, size = pVector.size();

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

    for (auto p: pVector) {
        for (int d = 0; d < DIMS; d++) {
            W.emplace_back(1.0 / p->m_Mass);
        }
    }

    JWJ jwj(J, W);


    for (ii = 0; ii < size; ii++) {
        pVector[ii]->m_Position += dt * pVector[ii]->m_Velocity; // Euler
        // Vec2f random_move = DAMP*pVector[ii]->m_Velocity + Vec2f(RAND,RAND) * 0.005;	// Random movement
        pVector[ii]->m_Velocity = pVector[ii]->m_Velocity + dt * (pVector[ii]->m_Forces / pVector[ii]->m_Mass);
    }
}
