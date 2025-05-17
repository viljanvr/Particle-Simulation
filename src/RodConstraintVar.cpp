#include "RodConstraintVar.h"

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

RodConstraintVar::RodConstraintVar(Particle *p1, Particle *p2, double dist, size_t index) : Constraint(index), m_p1(p1), m_p2(p2), m_dist(dist) {}

std::vector<Constraint::JacobianEntry> RodConstraintVar::getJacobian() {
    double dist = getC() + m_dist;
    return {JacobianEntry{m_p1, (m_p1->m_Position[0] - m_p2->m_Position[0]) / dist,
                          (m_p1->m_Position[1] - m_p2->m_Position[1]) / dist},
            JacobianEntry{m_p2, - (m_p1->m_Position[0] - m_p2->m_Position[0]) / dist,
                          - (m_p1->m_Position[1] - m_p2->m_Position[1]) / dist}};
}

std::vector<Constraint::JacobianEntry> RodConstraintVar::getJacobianDeriv() {
    double dist = getC() + m_dist;
    double interim = getCDeriv() * dist;
    return {JacobianEntry{m_p1, - interim * (m_p1->m_Position[0] - m_p2->m_Position[0]) / pow(dist, 3) + (m_p1->m_Velocity[0] - m_p2->m_Velocity[0]) / dist,
                          - interim * (m_p1->m_Position[1] - m_p2->m_Position[1]) / pow(dist, 3) + (m_p1->m_Velocity[1] - m_p2->m_Velocity[1]) / dist},
        JacobianEntry{m_p2, interim * (m_p1->m_Position[0] - m_p2->m_Position[0]) / pow(dist, 3) - (m_p1->m_Velocity[0] - m_p2->m_Velocity[0]) / dist,
                          interim * (m_p1->m_Position[1] - m_p2->m_Position[1]) / pow(dist, 3) - (m_p1->m_Velocity[1] - m_p2->m_Velocity[1]) / dist}};
}

double RodConstraintVar::getC() {
    return sqrt(pow(m_p1->m_Position[0] - m_p2->m_Position[0], 2) +
        pow(m_p1->m_Position[1] - m_p2->m_Position[1], 2)) - m_dist;
}

double RodConstraintVar::getCDeriv() {
    double dist = getC() + m_dist;
    return ((m_p1->m_Position[0] - m_p2->m_Position[0]) * (m_p1->m_Velocity[0] - m_p2->m_Velocity[0])
        + (m_p1->m_Position[1] - m_p2->m_Position[1]) * (m_p1->m_Velocity[1] - m_p2->m_Velocity[1]))
        / dist;
}



void RodConstraintVar::draw() {
    glBegin(GL_LINES);
    glColor3f(0.8, 0.7, 0.6);
    glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
    glColor3f(0.8, 0.7, 0.6);
    glVertex2f(m_p2->m_Position[0], m_p2->m_Position[1]);
    glEnd();
}
