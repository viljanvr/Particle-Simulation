#include "GravityForce.h"

GravityForce::GravityForce(std::vector<Particle *> particles, Vec2f G) : m_particles(particles), m_G(G) {}

void GravityForce::applyForce() {
    for (Particle *p: m_particles) {
        p->m_Forces += p->m_Mass * m_G;
    }
}

void GravityForce::draw() {}
