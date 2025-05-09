#pragma once

#include "Particle.h"

struct JacobianEntry {
    Particle *const p;
    double x;
    double y;
};

class Constraint {

public:
    virtual ~Constraint() = default;
    explicit Constraint(size_t index) : m_index(index) {};
    virtual std::vector<JacobianEntry> getJacobian() = 0;
    virtual std::vector<JacobianEntry> getJacobianDeriv() = 0;
    virtual double getC() = 0;
    virtual double getCDeriv() = 0;
    virtual void draw() = 0;

    size_t m_index;
};
