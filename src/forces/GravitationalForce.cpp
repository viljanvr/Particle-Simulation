#include "GravitationalForce.h"

#include <utility>

static constexpr float EPSILON = 0.0005;

GravitationalForce::GravitationalForce(std::vector<Particle *> particles, const double G) : m_particles(std::move(particles)), m_G(G) {}

void GravitationalForce::applyForce() {
    for (Particle *p1: m_particles) {
        for (Particle *p2 : m_particles) {
            if (p1 == p2) {
                continue;
            }
            Vec2f delta = p2->m_Position - p1->m_Position;
            double strength = m_G * p1->m_Mass * p2->m_Mass / std::max(delta * delta, + EPSILON);
            p1->m_Forces += strength * delta;
            p2->m_Forces -= strength * delta;
        }
    }
}

void GravitationalForce::draw() {}
