#pragma once
#include "Force.h"
#include "Particle.h"

class LinearForce : public Force{
public:
    LinearForce(std::vector<Particle*> particles, Vec2f G);
    void applyForce() override;
    void draw() override;

private:
    std::vector<Particle*> m_particles;
    Vec2f m_G;
};
