#include "Plane.h"
#include "gfx/vec2.h"

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Plane::Plane(const Vec2f &origin, const Vec2f &normal, std::vector<Particle *> particles, double epsilon,
             double bounce) : CollideableObject(bounce), m_Origin(origin), m_Epsilon(epsilon) {
    m_Normal = normal / norm(normal);
};

static float cross_product(const Vec2f &a, const Vec2f &b) { return a[0] * b[1] - a[1] * b[0]; }

bool Plane::is_particle_colliding(Particle *p) const {
    return ((p->m_Position - m_Origin) * m_Normal < m_Epsilon && m_Normal * p->m_Velocity < 0);
}

Collision Plane::compute_collision_details(Particle *p) const {
    // Backtracking is only needed if the particle has crossed the plane
    if ((p->m_Position - m_Origin) * m_Normal > 0) {
        return Collision{p, m_Normal, 0.0};
    }

    Vec2f plane_dir = Vec2f(-m_Normal[1], m_Normal[0]);
    double t = cross_product(m_Origin - p->m_PreviousPosition, plane_dir) /
               cross_product(p->m_Position - p->m_PreviousPosition, plane_dir);

    double backtracking = (1.0 - t);

    return Collision{p, m_Normal, backtracking};
}


void Plane::draw() {
    // Draw origin point
    // const double h = 0.03;
    // glColor3f(1.f, 0.f, 0.f);
    // glBegin(GL_QUADS);
    // glVertex2f(m_Origin[0] - h / 2.0, m_Origin[1] - h / 2.0);
    // glVertex2f(m_Origin[0] + h / 2.0, m_Origin[1] - h / 2.0);
    // glVertex2f(m_Origin[0] + h / 2.0, m_Origin[1] + h / 2.0);
    // glVertex2f(m_Origin[0] - h / 2.0, m_Origin[1] + h / 2.0);
    // glEnd();

    // Draw line
    float t = 1000.0f;
    Vec2f dir = Vec2f(-m_Normal[1], m_Normal[0]);
    Vec2f start = Vec2f(m_Origin[0] - t * dir[0], m_Origin[1] - t * dir[1]);
    Vec2f end = Vec2f(m_Origin[0] + t * dir[0], m_Origin[1] + t * dir[1]);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(start[0], start[1]);
    glVertex2f(end[0], end[1]);
    glEnd();

    // Draw box
    Vec2f p1 = start;
    Vec2f p2 = start - 0.1 * m_Normal;
    Vec2f p3 = end - 0.1 * m_Normal;
    Vec2f p4 = end;
    glColor3f(0.2f, 0.f, 0.f);
    glBegin(GL_QUADS);
    glVertex2f(p1[0], p1[1]);
    glVertex2f(p2[0], p2[1]);
    glVertex2f(p3[0], p3[1]);
    glVertex2f(p4[0], p4[1]);
    glEnd();
}
