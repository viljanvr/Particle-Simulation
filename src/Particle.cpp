#include "Particle.h"

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Particle::Particle(const Vec2f &ConstructPos, size_t index) :
    m_ConstructPos(ConstructPos), m_Position(Vec2f(0.0, 0.0)), m_Velocity(Vec2f(0.0, 0.0)), m_Forces(Vec2f(0.0, 0.0)),
    m_index(index), m_Mass(1.0) {}

Particle::~Particle(void) {}

void Particle::reset() {
    m_Position = m_ConstructPos;
    m_Velocity = Vec2f(0.0, 0.0);
    m_Forces = Vec2f(0.0, 0.0);
    m_Mass = 1.0;
}
void Particle::draw() {
    const double h = 0.03;
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
    glVertex2f(m_Position[0] - h / 2.0, m_Position[1] - h / 2.0);
    glVertex2f(m_Position[0] + h / 2.0, m_Position[1] - h / 2.0);
    glVertex2f(m_Position[0] + h / 2.0, m_Position[1] + h / 2.0);
    glVertex2f(m_Position[0] - h / 2.0, m_Position[1] + h / 2.0);
    glEnd();
}
