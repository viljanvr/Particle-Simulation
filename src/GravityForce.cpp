#include "GravityForce.h"
#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GravityForce::GravityForce(std::vector<Particle*> particles, Vec2f G)
    : m_particles(particles), m_G(G) {}

void GravityForce::applyForce() {
    for (Particle* p : m_particles) {
        p->m_Forces += p->m_Mass * m_G;
    }
}

void GravityForce::draw()
{
}