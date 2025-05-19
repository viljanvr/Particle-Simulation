#include "QuadraticDragForce.h"

#include <utility>

QuadraticDragForce::QuadraticDragForce(std::vector<Particle *> particles, const double strength) : m_particles(std::move(particles)), m_strength(strength) {}

void QuadraticDragForce::applyForce() {
    for (Particle *p: m_particles) {
        p->m_Forces += - 0.5 * m_strength * p->m_Velocity * norm(p->m_Velocity);
    }
}

std::vector<Force::JacobianEntry> QuadraticDragForce::getJx() {
	return {};
}

std::vector<Force::JacobianEntry> QuadraticDragForce::getJv() {
    std::vector<Force::JacobianEntry> res;
    for (Particle *p: m_particles) {
        Vec2f v = p->m_Velocity;
        // two rows of v * v^T
        Vec2f o1 = Vec2f(v[0] * v[0], v[0] * v[1]);
        Vec2f o2 = Vec2f(v[1] * v[0], v[1] * v[1]);
        // two rows of jacobian
        Vec2f j1 = Vec2f(- 0.5 * o1 + 0.5 * Vec2f(norm(v), 0));
        Vec2f j2 = Vec2f(- 0.5 * o1 + 0.5 * Vec2f(0, norm(v)));

        res.push_back(JacobianEntry{p, p, j1[0], j1[1], j2[0], j2[1]});
    }
    return res;
}

void QuadraticDragForce::draw() {}
