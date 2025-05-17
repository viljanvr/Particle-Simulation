#pragma once
#include "Force.h"
#include "Particle.h"

class FixedEndpointSpringForce : public Force {
public:
    FixedEndpointSpringForce(Particle *p, Vec2f end, double dist, double ks, double kd);
    void applyForce() override;
    void draw() override;
    std::vector<JacobianEntry> getJx() override;
    std::vector<JacobianEntry> getJv() override;

private:
    Particle *const m_p; // particle
    Vec2f m_end;
    double const m_dist; // rest length
    double const m_ks, m_kd; // spring strength constants
};
