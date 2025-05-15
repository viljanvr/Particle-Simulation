#include "LinearDragForce.h"

#include <utility>

LinearDragForce::LinearDragForce(std::vector<Particle *> particles, const double strength) : m_particles(std::move(particles)), m_strength(strength) {}

void LinearDragForce::applyForce() {
    for (Particle *p: m_particles) {
        p->m_Forces += - m_strength * p->m_Velocity;
    }
}

void LinearDragForce::draw() {}
