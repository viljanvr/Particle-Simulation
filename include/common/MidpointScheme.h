# pragma once

#include "IntegrationScheme.h"

class MidpointScheme : public IntegrationScheme {
public:
    void updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces, float dt) override;
};
