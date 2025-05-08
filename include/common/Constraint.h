#pragma once

#include "Particle.h"

struct JacobianEntry {
    Particle *const p;
    double x;
    double y;
};

class Constraint {

public:
    Constraint(size_t index);
    virtual std::vector<JacobianEntry> getJacobian() = 0;
    virtual std::vector<JacobianEntry> getJacobianDeriv() = 0;

    size_t m_index;
};
