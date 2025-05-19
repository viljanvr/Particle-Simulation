#pragma once
#include "Force.h"
#include "Particle.h"

class QuadraticDragForce : public Force{
public:
    QuadraticDragForce(std::vector<Particle*> particles, double strength);
    void applyForce() override;
    void draw() override;
    std::vector<JacobianEntry> getJx() override;
    std::vector<JacobianEntry> getJv() override;

private:
    std::vector<Particle*> m_particles;
    double m_strength;
};
