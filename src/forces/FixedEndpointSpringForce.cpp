#include "FixedEndpointSpringForce.h"

#include <linearSolver.h>

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

FixedEndpointSpringForce::FixedEndpointSpringForce(Particle *p, Vec2f end, double dist, double ks, double kd) :
    m_p(p), m_end(end), m_dist(dist), m_ks(ks), m_kd(kd) {}

void FixedEndpointSpringForce::applyForce() {
    Vec2f l = m_p->m_Position - m_end;
    Vec2f l_dot = m_p->m_Velocity;
    Vec2f fp1 = -(m_ks * (norm(l) - m_dist) + m_kd * (l_dot * l / norm(l))) * (l / norm(l));
    m_p->m_Forces += fp1;
}

std::vector<Force::JacobianEntry> FixedEndpointSpringForce::getJx() {
    Vec2f d = m_end - m_p->m_Position;
    Vec2f v = - m_p->m_Velocity;
    double l = norm(d);
    Vec2f dn = d / l;

    // two rows of dn * dn^T
    Vec2f o1 = Vec2f(dn[0] * dn[0], dn[0] * dn[1]);
    Vec2f o2 = Vec2f(dn[1] * dn[0], dn[1] * dn[1]);

    // Two rows of I - dn * dn^T
    Vec2f i1 = Vec2f(1, 0) - o1;
    Vec2f i2 = Vec2f(0, 1) - o2;

    double rat = (1.0 - m_dist / l);

    // Jacobian of non damped spring force for each particle
    Vec2f s1 = -m_ks * ((rat * i1) + o1);
    Vec2f s2 = -m_ks * ((rat * i2) + o2);

    // two rows of dn * v^T
    Vec2f ov1 = Vec2f(dn[0] * v[0], dn[0] * v[1]);
    Vec2f ov2 = Vec2f(dn[1] * v[0], dn[1] * v[1]);

    // two rows of ov * i
    Vec2f ovi1 = Vec2f(ov1[0] * i1[0] + ov1[1] * i2[0], ov1[0] * i1[1] + ov1[1] * i2[1]);
    Vec2f ovi2 = Vec2f(ov2[0] * i1[0] + ov2[1] * i2[0], ov2[0] * i1[1] + ov2[1] * i2[1]);

    // Jacobian of damping force for each particle
    Vec2f d1 = - m_kd / l * (ovi1 + (v * dn) * i1);
    Vec2f d2 = - m_kd / l * (ovi2 + (v * dn) * i2);

    return {
        JacobianEntry {m_p, m_p, s1[0] + d1[0], s1[1] + d1[1], s2[0] + d2[0], s2[1] + d2[1]},
    };
}


std::vector<Force::JacobianEntry> FixedEndpointSpringForce::getJv() {
    Vec2f d = m_p->m_Position - m_end;
    Vec2f dn = d / norm(d);
    return {
        JacobianEntry {
            m_p,
            m_p,
            - m_kd * dn[0] * dn[0],
            - m_kd * dn[0] * dn[1],
            - m_kd * dn[1] * dn[0],
            - m_kd * dn[1] * dn[1],
        },
    };

}

void FixedEndpointSpringForce::draw() {
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(m_p->m_Position[0], m_p->m_Position[1]);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(m_end[0], m_end[1]);
    glEnd();
}
