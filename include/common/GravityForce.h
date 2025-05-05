#pragma once
#include "Force.h"
#include "Particle.h"

class GravityForce : public Force{
public:
    GravityForce(std::vector<Particle*> particles, Vec2f G);
    void applyForce() override;
    void draw() override;

private:
    std::vector<Particle*> m_particles;
    Vec2f m_G;
};
