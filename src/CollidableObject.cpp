#include "CollideableObject.h"
#include "Particle.h"

CollideableObject::CollideableObject(std::vector<Particle*> particles, double bounce) : m_particles(std::move(particles)), m_bounce(bounce) {};

std::vector<Collision> CollideableObject::get_earliest_collisions() const {
    // We want to backtrack everything to the earliest collision (if multiple are earliest, we handle all).
    std::vector<Collision> earliest_collisions;
    for (auto p : m_particles) {
        if (is_particle_colliding(p)) {
            Collision collision = compute_collision_details(p);
            if (earliest_collisions.empty() || collision.backtracking_factor == earliest_collisions.back().backtracking_factor) {
                earliest_collisions.push_back(collision);
            } else if (collision.backtracking_factor > earliest_collisions.back().backtracking_factor) {
                earliest_collisions.clear();
                earliest_collisions.push_back(collision);
            }
        }
    }
    return earliest_collisions;
}

void CollideableObject::bounce_single_particle(const Collision &collision) const {
    Vec2f v_normal = (collision.p->m_Velocity * collision.collisionNormal) * collision.collisionNormal;
    collision.p->m_Velocity -= v_normal * (1.0 + m_bounce);
}

void CollideableObject::backtrack_particles(double backtracking_factor) const {
    if (backtracking_factor <= 0) {
        return;
    }
    for (auto p : m_particles) {
        p->m_Position = p->m_Position + backtracking_factor * (p->m_PreviousPosition - p->m_Position);
        p->m_Velocity = p->m_Velocity + backtracking_factor * (p->m_PreviousVelocity - p->m_Velocity);
    }
}
