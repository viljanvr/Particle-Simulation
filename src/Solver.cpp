#include "Particle.h"
#include "Force.h"
#include <vector>

#define DAMP 0.98f
#define RAND (((rand()%2000)/1000.f)-1.f)
void simulation_step( std::vector<Particle*> pVector, std::vector<Force*> fVector, float dt )
{
	int ii, size = pVector.size();

	// Recalculate forces
    for (auto p : pVector) {
		p->m_Forces = Vec2f(0.0, 0.0);
	}
	for (auto f : fVector) {
		f->applyForce();
	}

	for(ii=0; ii<size; ii++)
	{
		pVector[ii]->m_Position += dt*pVector[ii]->m_Velocity;	// Euler
//		pVector[ii]->m_Velocity = DAMP*pVector[ii]->m_Velocity + Vec2f(RAND,RAND) * 0.005;	// Random movement
		pVector[ii]->m_Velocity = pVector[ii]->m_Velocity + dt * (pVector[ii]->m_Forces / pVector[ii]->m_Mass);
	}


}

