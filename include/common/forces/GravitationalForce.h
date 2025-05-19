#pragma once
#include "Force.h"
#include "Particle.h"

class GravitationalForce : public Force{
public:
    GravitationalForce(std::vector<Particle*> particles, double G);
    void applyForce() override;
    void draw() override;

private:
    std::vector<Particle*> m_particles;
    double m_G;
};
