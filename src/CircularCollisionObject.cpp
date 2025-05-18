#include "CircularCollisionObject.h"
#include <iostream>
#include "gfx/vec2.h"

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

CircularCollisionObject::CircularCollisionObject(const Vec2f &origin, double radius, std::vector<Particle *> particles,
                                                 double epsilon, double bounce) :
    CollideableObject(std::move(particles), bounce), m_Origin(origin), m_Radius(radius), m_Epsilon(epsilon) {}

bool CircularCollisionObject::is_particle_colliding(Particle *p) const {
    return (
        norm(p->m_Position - m_Origin) - m_Radius < m_Epsilon
        && (p->m_Position - m_Origin) * p->m_Velocity < 0
    );
}

Collision CircularCollisionObject::compute_collision_details(Particle *p) const {
    Vec2f normal = (p->m_Position - m_Origin) / norm(p->m_Position - m_Origin);
    // Backtracking is only needed if the particle has crossed the circle
    if ((p->m_Position - m_Origin) * normal > 0) {
        return Collision {p, normal, 0.0};
    }

    Vec2f f = (p->m_PreviousPosition - m_Origin);
    Vec2f d = (p->m_Position - p->m_PreviousPosition);

    double A = d * d;
    double B = 2 * f * d;
    double C = f * f - m_Radius * m_Radius;

    double t = (-B + sqrt(B * B - 4 * A * C)) / (2 * A);
    if (t < 0 || t > 1) {
        t = (-B - sqrt(B * B - 4 * A * C)) / (2 * A);
    }

    Vec2f collision_point = p->m_PreviousPosition + t * d;
    Vec2f collision_normal = collision_point - m_Origin;

    return Collision {p, collision_normal, t};


}

void CircularCollisionObject::draw() {
    // Draw Circle
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 0.0, 0.0);
    for (int i = 0; i < 360; i = i + 18) {
        float degInRad = i * PI / 180;
        glVertex2f(m_Origin[0] + cos(degInRad) * m_Radius, m_Origin[1] + sin(degInRad) * m_Radius);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.0, 0.0);
    for (int i = 0; i < 360; i = i + 18) {
        float degInRad = i * PI / 180;
        glVertex2f(m_Origin[0] + cos(degInRad) * m_Radius, m_Origin[1] + sin(degInRad) * m_Radius);
    }
    glEnd();
}
