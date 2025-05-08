#include "RodConstraint.h"

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

RodConstraint::RodConstraint(Particle *p1, Particle *p2, double dist) : m_p1(p1), m_p2(p2), m_dist(dist) {}

std::vector<JacobianEntry> RodConstraint::getJacobian() {
    return {JacobianEntry{m_p1, 2.0 * (m_p1->m_Position[0] - m_p2->m_Position[0]),
                          2.0 * (m_p1->m_Position[1] - m_p2->m_Position[1])},
            JacobianEntry{m_p2, -2.0 * (m_p1->m_Position[0] - m_p2->m_Position[0]),
                          -2.0 * (m_p1->m_Position[1] - m_p2->m_Position[1])}};
}

std::vector<JacobianEntry> RodConstraint::getJacobianDeriv() {
    return {JacobianEntry{m_p1, 2.0 * (m_p1->m_Velocity[0] - m_p2->m_Velocity[0]),
                          2.0 * (m_p1->m_Velocity[1] - m_p2->m_Velocity[1])},
            JacobianEntry{m_p2, -2.0 * (m_p1->m_Velocity[0] - m_p2->m_Velocity[0]),
                          -2.0 * (m_p1->m_Velocity[1] - m_p2->m_Velocity[1])}};
}

void RodConstraint::draw() {
    glBegin(GL_LINES);
    glColor3f(0.8, 0.7, 0.6);
    glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
    glColor3f(0.8, 0.7, 0.6);
    glVertex2f(m_p2->m_Position[0], m_p2->m_Position[1]);
    glEnd();
}
