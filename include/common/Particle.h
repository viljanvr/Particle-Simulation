#pragma once

#include <gfx/vec2.h>

class Particle {
public:
    Particle(const Vec2f &ConstructPos, bool forceVisualization, size_t index);
    virtual ~Particle(void);

    void reset();
    void draw();

    Vec2f m_ConstructPos;
    Vec2f m_Position;
    Vec2f m_Velocity;
    Vec2f m_Forces;
    float m_Mass;
    bool m_forceVisualization;

    size_t m_index;
};
