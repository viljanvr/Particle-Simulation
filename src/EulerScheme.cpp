#include "EulerScheme.h"

void EulerScheme::updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces,
                                              float dt) {
    updateForces();

    for (auto particle: particles) {
        particle->m_Position += particle->m_Velocity * dt;
        particle->m_Velocity += (particle->m_Forces / particle->m_Mass) * dt;
    }
}
