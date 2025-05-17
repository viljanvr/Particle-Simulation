#pragma once


#include <vector>
#include "Constraint.h"
#include "Force.h"
#include "Particle.h"

void set_scene(int scene, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
               std::vector<Constraint *> &cVector, bool visualizeForces);

void constructCloth(size_t rows, size_t cols, double spacing, bool diagonal, bool useRods, std::vector<Particle *> &pVector, std::vector<Force *> &fVector, std::vector<Constraint *> &cVector, bool visualizeForces);

void attachCloth(size_t cols, double spacing, std::vector<Particle *> &pVector, std::vector<Force *> &fVector, std::vector<Constraint *> &cVector, bool visualizeForces);