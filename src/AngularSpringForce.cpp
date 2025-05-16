#include "AngularSpringForce.h"

#include <math.h>

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


static double cross(Vec2f v1, Vec2f v2) {
    return v1[0]*v2[1] - v1[1]*v2[0];
}

static double degToRad(double deg) {
    return deg*M_PI/180;
}

static double angle_diff(double a, double b) {
    double diff = a - b;
    while (diff <= -M_PI) diff += 2 * M_PI;
    while (diff > M_PI)   diff -= 2 * M_PI;
    return diff;
}

AngularSpringForce::AngularSpringForce(Particle* p0, Particle *p1, Particle *p2, double angle_deg, double ks, double kd) :
    m_p0(p0), m_p1(p1), m_p2(p2), m_target_angle(degToRad(angle_deg)), m_ks(ks), m_kd(kd) {}

void AngularSpringForce::applyForce() {
    Vec2f v = m_p0->m_Position - m_p1->m_Position;
    Vec2f vDeriv = m_p0->m_Velocity - m_p1->m_Velocity;
    Vec2f w = m_p2->m_Position - m_p1->m_Position;
    Vec2f wDeriv = m_p2->m_Velocity - m_p1->m_Velocity;
    double alpha = atan2(cross(v, w), v * w);
    //double alphaDeriv = cross(v, vDeriv) / (v * v) +
    //     cross(w, wDeriv) / (w * w);
    double alphaDeriv = ((v * w) * (cross(vDeriv, w) + cross(v, wDeriv)) - cross(v,w) * (vDeriv *  w + v * wDeriv)) / (
        (v * w) * (v * w) + cross(v, w) * cross(v, w));
    double torque = m_ks * angle_diff(alpha, m_target_angle) + m_kd * (alphaDeriv);

    Vec2f f0 = - torque / norm(v) * perp(v);
    Vec2f f2 = torque / norm(w) * perp(w);

    m_p0->m_Forces += f0;
    m_p2->m_Forces += f2;
    m_p1->m_Forces += -(f0 + f2);
}

void AngularSpringForce::draw() {
    //glBegin(GL_LINES);
    //glColor3f(0.6, 0.7, 0.8);
    //glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
    //glColor3f(0.6, 0.7, 0.8);
    //glVertex2f(m_p2->m_Position[0], m_p2->m_Position[1]);
    //glEnd();
}
