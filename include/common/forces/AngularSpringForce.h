#pragma once
#include "Force.h"
#include "Particle.h"

// Tries to keep constant angle between vectors (p1, p0) and (p1, p2).
class AngularSpringForce : public Force {
public:
    AngularSpringForce(Particle *p0, Particle *p1, Particle *p2, double angle_deg, double ks, double kd);
    void applyForce() override;
    void draw() override;

private:
    Particle *const m_p0; // particle 1
    Particle *const m_p1; // particle 2
    Particle *const m_p2; // particle 2
    double const m_target_angle; // rest angle
    double const m_ks, m_kd; // spring strength constants
};
