#pragma once
#include "Force.h"
#include "Particle.h"
#include "gfx/vec2.h"
#include <SparseMatrix.h>

class BlowForce : public Force{
public:
    BlowForce(std::vector<Particle*> particles, Vec2f origin, float intensity, float maxStrength);
    void applyForce() override;
    void draw() override;
    void setOrigin(Vec2f origin);
    std::vector<JacobianEntry> getJx() override;
    std::vector<JacobianEntry> getJv() override;

private:
    std::vector<Particle*> m_particles;
    Vec2f m_origin;
    float m_intensity;
    float m_maxStrength;
};

