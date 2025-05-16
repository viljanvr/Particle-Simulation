#pragma once


#include <vector>
#include "CollideableObject.h"
#include "Constraint.h"
#include "Force.h"
#include "Particle.h"

void set_scene(int scene, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
               std::vector<Constraint *> &cVector, std::vector<CollideableObject *> &oVector, bool visualizeForces);
