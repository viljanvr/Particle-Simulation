#include "CircularWireConstraint.h"

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1415926535897932384626433832795

std::vector<JacobianEntry> CircularWireConstraint::getJacobian() {
    return {JacobianEntry{m_p, 2.0 * (m_p->m_Position[0] - m_center[0]),
                          2.0 * (m_p->m_Position[1] - m_center[1])}};
}

std::vector<JacobianEntry> CircularWireConstraint::getJacobianDeriv() {
    return {JacobianEntry{m_p, 2.0 * (m_p->m_Velocity[0] - m_center[0]),
                          2.0 * (m_p->m_Velocity[1] - m_center[1])}};
}

static void draw_circle(const Vec2f &vect, float radius) {
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < 360; i = i + 18) {
        float degInRad = i * PI / 180;
        glVertex2f(vect[0] + cos(degInRad) * radius, vect[1] + sin(degInRad) * radius);
    }
    glEnd();
}

CircularWireConstraint::CircularWireConstraint(Particle *p, const Vec2f &center, const double radius, size_t index) :
    m_p(p), m_center(center), m_radius(radius), Constraint(index) {}

void CircularWireConstraint::draw() { draw_circle(m_center, m_radius); }
