#pragma once

#include <gfx/vec2.h>

class Particle {
public:
    Particle(const Vec2f &ConstructPos, bool forceVisualization, size_t index, double mass = 1.0, const Vec2f &ConstructionVelocity = Vec2f(0.0, 0.0));
    virtual ~Particle(void);

    void reset();
    void draw();

    Vec2f m_ConstructPos;
    Vec2f m_ConstructionVelocity;
    Vec2f m_Position;
    Vec2f m_Velocity;
    Vec2f m_Forces;
    double m_Mass;
    bool m_forceVisualization;

    size_t m_index;
};
