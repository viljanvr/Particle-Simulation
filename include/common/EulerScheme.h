# pragma once

#include "IntegrationScheme.h"

class EulerScheme : public IntegrationScheme {
public:
    void updateParticlesBasedOnForce(std::vector<Particle *> particles, derivEvalFunction updateForces,
                                     JacobianFunction getJx, JacobianFunction getJv, float dt) override;
};