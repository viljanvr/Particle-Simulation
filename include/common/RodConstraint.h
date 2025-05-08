#pragma once

#include "Constraint.h"
#include "Particle.h"

class RodConstraint : Constraint {
public:
    RodConstraint(Particle *p1, Particle *p2, double dist, size_t index);
    std::vector<JacobianEntry> getJacobian() override;
    std::vector<JacobianEntry> getJacobianDeriv() override;

    void draw() override;

private:
    Particle *const m_p1;
    Particle *const m_p2;
    double const m_dist;
};
