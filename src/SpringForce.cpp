#include "SpringForce.h"

#include <linearSolver.h>

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

SpringForce::SpringForce(Particle *p1, Particle * p2, double dist, double ks, double kd) :
  m_p1(p1), m_p2(p2), m_dist(dist), m_ks(ks), m_kd(kd) {}

void SpringForce::applyForce() {
  Vec2f l = m_p1->m_Position - m_p2->m_Position;
  Vec2f l_dot = m_p1->m_Velocity - m_p2->m_Velocity;
  Vec2f fp1 = -(m_ks * (norm(l) - m_dist) + m_kd * (l_dot * l / norm(l))) * (l / norm(l));
  m_p1->m_Forces += fp1;
  m_p2->m_Forces -= fp1;
}

void SpringForce::draw()
{
  glBegin( GL_LINES );
  glColor3f(0.6, 0.7, 0.8);
  glVertex2f( m_p1->m_Position[0], m_p1->m_Position[1] );
  glColor3f(0.6, 0.7, 0.8);
  glVertex2f( m_p2->m_Position[0], m_p2->m_Position[1] );
  glEnd();
}
