#include "CollideableObject.h"
#include "DebugFunctions.h"
#include "Particle.h"
#include "gfx/vec2.h"

CollideableObject::CollideableObject(double bounce, double friction_fator, double slide_threshold) :
    m_bounce(bounce), m_friction_factor(friction_fator), m_sliding_threshold(slide_threshold) {};

void CollideableObject::bounce_particle(const Collision &c, double dt) const {
    // Backtrack velocity
    c.p->m_Velocity = c.p->m_Velocity + c.backtracking_factor * (c.p->m_PreviousVelocity - c.p->m_Velocity);

    // Calculate the normal component of the velocity
    Vec2f v_normal = (c.p->m_Velocity * c.collisionNormal) * c.collisionNormal;

    if (is_collision_sliding(c)) {
        c.p->m_Velocity -= v_normal;
    } else {
        c.p->m_Velocity -= v_normal * (1.0 + m_bounce);
    }

    Vec2f intersection = c.p->m_Position + c.backtracking_factor * (c.p->m_PreviousPosition - c.p->m_Position);
    Vec2f post_collision_movement = c.p->m_Position - intersection;
    c.p->m_Position = intersection + post_collision_movement +
                      (-2 * post_collision_movement * c.collisionNormal) * c.collisionNormal;

    if (is_collision_sliding(c)) {
        slow_particle_from_friction(c, dt);
    }
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
    if (new_length < 0)
        new_length = 0;
    Vec2f v_tangential_new = v_tangential / v_tangential_length * new_length;

    collision.p->m_Velocity = v_normal + v_tangential_new;
}
