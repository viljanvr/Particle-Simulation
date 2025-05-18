#include "CollideableObject.h"
#include "Particle.h"

CollideableObject::CollideableObject(std::vector<Particle*> particles, double bounce) : m_particles(std::move(particles)), m_bounce(bounce) {};

Collision CollideableObject::get_earliest_collision() const {
    // We want to backtrack everything to the earliest collision;
    Collision earliest_collision {nullptr, {0, 0}, 0.0};
    for (auto p : m_particles) {
        if (is_particle_colliding(p)) {
            Collision collision = compute_collision_details(p);
            if (earliest_collision.p == nullptr|| earliest_collision.backtracking_factor < collision.backtracking_factor) {
                earliest_collision = collision;
            }
        }
    }
    return earliest_collision;
}

void CollideableObject::bounce_single_particle(const Collision &collision) const {
    Vec2f v_normal = (collision.p->m_Velocity * collision.collisionNormal) * collision.collisionNormal;
    collision.p->m_Velocity -= v_normal * (1.0 + m_bounce);
}

void CollideableObject::backtrack_particles(double backtracking_factor) const {
    std::cout << "backtracking_factor: " << backtracking_factor << std::endl;
    if (backtracking_factor <= 0) {
        return;
    }
    for (auto p : m_particles) {
        p->m_Position = p->m_Position + backtracking_factor * (p->m_PreviousPosition - p->m_Position);
        p->m_Velocity = p->m_Velocity + backtracking_factor * (p->m_PreviousVelocity - p->m_Velocity);
        if (is_particle_colliding(p)) {
            std::cout << "particle is still colliding after backtracking!" << std::endl;
        }
    }
}
