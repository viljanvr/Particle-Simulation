#pragma once


#include <vector>
#include "Constraint.h"
#include "Force.h"
#include "Particle.h"

void set_scene(int scene, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
               std::vector<Constraint *> &cVector, bool visualizeForces);
