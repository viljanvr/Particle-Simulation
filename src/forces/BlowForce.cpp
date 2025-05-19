#include "BlowForce.h"

BlowForce::BlowForce(std::vector<Particle *> particles, Vec2f origin, float intensity, float maxStrength) :
        m_particles(particles), m_origin(origin), m_intensity(intensity), m_maxStrength(maxStrength) {}

void BlowForce::applyForce() {
    for (Particle *p: m_particles) {
        Vec2f dir = p->m_Position - m_origin;
        // 0.05, 0.6
        const float strength = std::min(m_intensity / norm(dir), m_maxStrength);
        unitize(dir);
        p->m_Forces += p->m_Mass * dir * strength;
    }
}

void BlowForce:: setOrigin(Vec2f origin) {
    m_origin = origin;
}

std::vector<Force::JacobianEntry> BlowForce::getJx() {
    return {};
}

std::vector<Force::JacobianEntry> BlowForce::getJv() {
    return {};
}

void BlowForce::draw() {}
