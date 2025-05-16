#pragma once
#include <gfx/vec2.h>
#include "CollideableObject.h"
#include "Particle.h"

#define EPSILON 0.01
#define KR 1.0

class Plane : public CollideableObject {
public:
    Plane(const Vec2f &origin, const Vec2f &normal, std::vector<Particle *> particles, double epsilon = EPSILON,
          double kr = KR);
    ~Plane() = default;
    void detect_collision() override;
    void draw() override;
    void backtrack_particles(Particle *collisionParticle);
    void respond_collision(Particle *collisionParticle);
    float cross_product(const Vec2f &a, const Vec2f &b);

private:
    Vec2f m_Origin;
    Vec2f m_Normal;
    std::vector<Particle *> m_Particles;
    double m_Epsilon;
    double m_kr; // bounciness of the wall
};
