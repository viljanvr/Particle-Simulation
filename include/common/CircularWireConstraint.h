#pragma once

#include "Particle.h"

#include "Constraint.h"

class CircularWireConstraint : public Constraint {
public:
    CircularWireConstraint(Particle *p, const Vec2f &center, const double radius, size_t index);
    std::vector<JacobianEntry> getJacobian() override;
    std::vector<JacobianEntry> getJacobianDeriv() override;

    void draw() override;

private:
    Particle *const m_p;
    Vec2f const m_center;
    double const m_radius;
};
