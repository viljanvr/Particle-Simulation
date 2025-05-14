#pragma once

#include "Particle.h"
#include <vector>
#include <functional>

using derivEvalFunction = std::function<void()>;

class IntegrationScheme {
public:
    virtual ~IntegrationScheme() = default;
    virtual void updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces, float dt) = 0;
};