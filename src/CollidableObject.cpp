#include "CollideableObject.h"
#include "Particle.h"

CollideableObject::CollideableObject(std::vector<Particle *> particles, double bounce, double friction_fator, double slide_threshold) :
    m_particles(std::move(particles)), m_bounce(bounce), m_friction_factor(friction_fator), m_sliding_threshold(slide_threshold){};

std::vector<Collision> CollideableObject::get_earliest_collisions() const {
    // We want to backtrack everything to the earliest collision (if multiple are earliest, we handle all).
    std::vector<Collision> earliest_collisions;
    for (auto p: m_particles) {
        if (is_particle_colliding(p)) {
            Collision collision = compute_collision_details(p);
            if (earliest_collisions.empty() ||
                collision.backtracking_factor == earliest_collisions.back().backtracking_factor) {
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

bool CollideableObject::is_collision_sliding(const Collision &collision) const {
    return abs(collision.p->m_Velocity * collision.collisionNormal) <= m_sliding_threshold;
}

void CollideableObject::slow_particle_from_friction(const Collision &collision, double dt) const {
    Vec2f f_normal = (collision.p->m_Forces * collision.collisionNormal) * collision.collisionNormal;
    double slowing_force_magnitude = norm(f_normal) * m_friction_factor;
    Vec2f v_normal = (collision.p->m_Velocity * collision.collisionNormal) * collision.collisionNormal;
    Vec2f v_tangential = collision.p->m_Velocity - v_normal;
    double v_tangential_length = norm(v_tangential);
    double new_length = v_tangential_length - slowing_force_magnitude * dt;
    if (new_length < 0) new_length = 0;
    Vec2f v_tangential_new = v_tangential / v_tangential_length * new_length;

    collision.p->m_Velocity = v_normal + v_tangential_new;
}




void CollideableObject::backtrack_particles(double backtracking_factor) const {
    if (backtracking_factor <= 0) {
        return;
    }
    for (auto p: m_particles) {
        p->m_Position = p->m_Position + backtracking_factor * (p->m_PreviousPosition - p->m_Position);
        p->m_Velocity = p->m_Velocity + backtracking_factor * (p->m_PreviousVelocity - p->m_Velocity);
    }
}
