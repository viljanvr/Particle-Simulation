# pragma once

#include "IntegrationScheme.h"

class RungeKuttaScheme : public IntegrationScheme {
public:
    void updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces, float dt) override;
};
