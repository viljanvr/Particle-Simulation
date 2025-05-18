#pragma once


#include <vector>
#include "CollideableObject.h"
#include "Constraint.h"
#include "Force.h"
#include "Particle.h"

void set_scene(int scene, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
               std::vector<Constraint *> &cVector, std::vector<CollideableObject *> &oVector, bool visualizeForces);

void constructCloth(Vec2f origin, size_t rows, size_t cols, double spacing, bool diagonal, bool useRods, std::vector<Particle *> &pVector,
                    std::vector<Force *> &fVector, std::vector<Constraint *> &cVector, bool visualizeForces);

void attachCloth(Vec2f origin, size_t rows, size_t cols, double spacing, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
                    std::vector<Constraint *> &cVector, bool visualizeForces);

void drawText(const char* text, float x, float y);