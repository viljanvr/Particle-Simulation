#pragma once

#include "Particle.h"

#include <SparseMatrix.h>
#include <functional>
#include <vector>

using derivEvalFunction = std::function<void()>;
using JacobianFunction = std::function<SparseMatrix()>;

class IntegrationScheme {
public:
    virtual ~IntegrationScheme() = default;
    virtual void updateParticlesBasedOnForce(
        std::vector<Particle *> particles,
        derivEvalFunction updateForces,
        JacobianFunction getJx,
        JacobianFunction getJv,
        float dt
    ) = 0;
};