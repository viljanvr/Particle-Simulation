#pragma once
#include <gfx/vec2.h>
#include "CollideableObject.h"
#include "Particle.h"

#define PI 3.1415926535897932384626433832795


class CircularCollisionObject : public CollideableObject {
public:
    CircularCollisionObject(const Vec2f &origin, double radius, std::vector<Particle *> particles, double epsilon = 0.02, double bounce = 1.0);
    ~CircularCollisionObject() = default;
    bool is_particle_colliding(Particle *p) const override;
    Collision compute_collision_details(Particle *p) const override;
    void draw() override;

private:
    Vec2f m_Origin;
    double m_Radius;
    double m_Epsilon;
};
