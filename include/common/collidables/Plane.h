#pragma once
#include <gfx/vec2.h>
#include <vector>
#include "CollideableObject.h"
#include "Particle.h"

#define EPSILON 0.005
#define BOUNCE 1.0

class Plane : public CollideableObject {
public:
    Plane(const Vec2f &origin, const Vec2f &normal, std::vector<Particle *> particles, double epsilon = EPSILON,
          double bounce = BOUNCE, double friction_factor = 0.0, double slide_threshold = EPSILON);
    ~Plane() = default;
    bool is_particle_colliding(Particle *p) const override;
    Collision compute_collision_details(Particle *p) const override;
    void draw() override;

private:
    Vec2f m_Origin;
    Vec2f m_Normal;
    double m_Epsilon;
};
