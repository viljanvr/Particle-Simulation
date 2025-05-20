#include "CollideableObject.h"
#include "DebugFunctions.h"
#include "Particle.h"
#include "gfx/vec2.h"

CollideableObject::CollideableObject(double bounce) : m_bounce(bounce) {};


void CollideableObject::bounce_particle(const Collision &c) const {
    // Backtrack velocity
    c.p->m_Velocity = c.p->m_Velocity + c.backtracking_factor * (c.p->m_PreviousVelocity - c.p->m_Velocity);

    // Flip velocity
    Vec2f v_normal = (c.p->m_Velocity * c.collisionNormal) * c.collisionNormal;
    c.p->m_Velocity -= v_normal * (1.0 + m_bounce);


    Vec2f intersection = c.p->m_Position + c.backtracking_factor * (c.p->m_PreviousPosition - c.p->m_Position);
    Vec2f post_collision_movement = c.p->m_Position - intersection;
    c.p->m_Position = intersection + post_collision_movement +
                      (-2 * post_collision_movement * c.collisionNormal) * c.collisionNormal;
}
