#include "MidpointScheme.h"

void MidpointScheme::updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces, float dt) {
    updateForces();

    std::vector<Vec2f> original_positions;
    std::vector<Vec2f> original_velocities;
    for (auto particle : particles) {
        original_positions.push_back(particle->m_Position);
        original_velocities.push_back(particle->m_Velocity);
        particle->m_Position += particle->m_Velocity * dt * 0.5;
        particle->m_Velocity += (particle->m_Forces / particle->m_Mass) * dt * 0.5;
    }

    updateForces();
    for (size_t i = 0; i < particles.size(); i++) {
        Particle *particle = particles[i];

        particle->m_Position = original_positions[i] + particle->m_Velocity * dt;
        particle->m_Velocity = original_velocities[i] + (particle->m_Forces / particle->m_Mass) * dt;
    }
}
