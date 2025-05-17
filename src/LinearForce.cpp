#include "LinearForce.h"

LinearForce::LinearForce(std::vector<Particle *> particles, Vec2f G) : m_particles(particles), m_G(G) {}

void LinearForce::applyForce() {
    for (Particle *p: m_particles) {
        p->m_Forces += p->m_Mass * m_G;
    }
}

void LinearForce::draw() {}
