#include "RungeKuttaScheme.h"

void RungeKuttaScheme::updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces,
                                                      JacobianFunction getJx, JacobianFunction getJv, float dt) {
    std::vector<Vec2f> original_positions;
    std::vector<Vec2f> original_velocities;
    for (auto particle : particles) {
        original_positions.push_back(particle->m_Position);
        original_velocities.push_back(particle->m_Velocity);
    }

    // Calculate k1
    updateForces();
    std::vector<Vec2f> k1_velocities;
    std::vector<Vec2f> k1_accelerations;
    for (auto particle : particles) {
        k1_velocities.push_back(particle->m_Velocity);
        k1_accelerations.push_back(particle->m_Forces / particle->m_Mass);
    }

    // Calculate k2
    for (size_t i = 0; i < particles.size(); i++) {
        Particle *particle = particles[i];
        particle->m_Position = original_positions[i] + k1_velocities[i] * dt * 0.5;
        particle->m_Velocity = original_velocities[i] + k1_accelerations[i] * dt * 0.5;
    }
    updateForces();
    std::vector<Vec2f> k2_velocities;
    std::vector<Vec2f> k2_accelerations;
    for (auto particle : particles) {
        k2_velocities.push_back(particle->m_Velocity);
        k2_accelerations.push_back(particle->m_Forces / particle->m_Mass);
    }

    // Calculate k3
    for (size_t i = 0; i < particles.size(); i++) {
        Particle *particle = particles[i];
        particle->m_Position = original_positions[i] + k2_velocities[i] * dt * 0.5;
        particle->m_Velocity = original_velocities[i] + k2_accelerations[i] * dt * 0.5;
    }
    updateForces();
    std::vector<Vec2f> k3_velocities;
    std::vector<Vec2f> k3_accelerations;
    for (auto particle : particles) {
        k3_velocities.push_back(particle->m_Velocity);
        k3_accelerations.push_back(particle->m_Forces / particle->m_Mass);
    }

    // Calculate k4
    for (size_t i = 0; i < particles.size(); i++) {
        Particle *particle = particles[i];
        particle->m_Position = original_positions[i] + k3_velocities[i] * dt;
        particle->m_Velocity = original_velocities[i] + k3_accelerations[i] * dt;
    }
    updateForces();
    std::vector<Vec2f> k4_velocities;
    std::vector<Vec2f> k4_accelerations;
    for (auto particle : particles) {
        k4_velocities.push_back(particle->m_Velocity);
        k4_accelerations.push_back(particle->m_Forces / particle->m_Mass);
    }

    // Calculate final update to position and velocity
    for (size_t i = 0; i < particles.size(); i++) {
        Particle *particle = particles[i];
        particle->m_Position = original_positions[i] + (k1_velocities[i] / 6.0 + k2_velocities[i] / 3.0 + k3_velocities[i] / 3.0 + k4_velocities[i] / 6.0) * dt;
        particle->m_Velocity = original_velocities[i] + (k1_accelerations[i] / 6.0 + k2_accelerations[i] / 3.0 + k3_accelerations[i] / 3.0 + k4_accelerations[i] / 6.0) * dt;
    }
}
