#pragma once
#include "Force.h"
#include "Particle.h"

#include <SparseMatrix.h>

class LinearForce : public Force{
public:
    LinearForce(std::vector<Particle*> particles, Vec2f G);
    void applyForce() override;
    void draw() override;
    void setForce(Vec2f newForce);
    std::vector<JacobianEntry> getJx() override;
    std::vector<JacobianEntry> getJv() override;

private:
    std::vector<Particle*> m_particles;
    Vec2f m_G;
};
