#include "QuadraticDragForce.h"

#include <utility>

QuadraticDragForce::QuadraticDragForce(std::vector<Particle *> particles, const double strength) : m_particles(std::move(particles)), m_strength(strength) {}

void QuadraticDragForce::applyForce() {
    for (Particle *p: m_particles) {
        p->m_Forces += - 0.5 * m_strength * p->m_Velocity * norm(p->m_Velocity);
    }
}

void QuadraticDragForce::draw() {}
