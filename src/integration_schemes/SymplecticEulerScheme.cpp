#include "SymplecticEulerScheme.h"

void SymplecticEulerScheme::updateParticlesBasedOnForce(std::vector<Particle *> particles,
                                                        derivEvalFunction updateForces, JacobianFunction getJx,
                                                        JacobianFunction getJv, float dt) {
    updateForces();

    for (auto particle: particles) {
        particle->m_Velocity += (particle->m_Forces / particle->m_Mass) * dt;
        particle->m_Position += particle->m_Velocity * dt;
    }
}
