#include "ImplicitEulerScheme.h"

#include <MatrixForImplicitEuler.h>

void ImplicitEulerScheme::updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces,
                                                      JacobianFunction getJx, JacobianFunction getJv, float dt) {
    size_t n = particles.size() * 2;

    updateForces();
    SparseMatrix Jx = getJx();
    SparseMatrix Jv = getJv();

    MatrixForImplicitEuler mat(Jx, Jv, dt);

    std::vector rhs(n, 0.0);
    std::vector v(n, 0.0);
    for (auto p : particles) {
        size_t i = p->m_index;
        v[2 * i] = p->m_Velocity[0];
        v[2 * i + 1 ] = p->m_Velocity[1];
        rhs[2 * i] = dt * p->m_Forces[0];
        rhs[2 * i + 1] = dt * p->m_Forces[1];
    }
    std::vector jxv = Jx.matVecMult(v);
    for (size_t i = 0; i < n; i++) {
        rhs[i] += dt * dt * jxv[i];
    }

    std::vector deltaV(n, 0.0);

    int steps = 0;
    ConjGrad(n, &mat, deltaV.data(), rhs.data(), 0.000000001, &steps);

    for (auto p : particles) {
        size_t i = p->m_index;
        p->m_Velocity[0] += deltaV[2 * i];
        p->m_Velocity[1] += deltaV[2 * i + 1];
        p->m_Position[0] += p->m_Velocity[0] * dt;
        p->m_Position[1] += p->m_Velocity[1] * dt;
    }
}
