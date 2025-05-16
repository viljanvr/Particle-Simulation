#pragma once
#include "Force.h"
#include "Particle.h"

class QuadraticDragForce : public Force{
public:
    QuadraticDragForce(std::vector<Particle*> particles, double strength);
    void applyForce() override;
    void draw() override;

private:
    std::vector<Particle*> m_particles;
    double m_strength;
};
