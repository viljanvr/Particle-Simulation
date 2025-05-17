#include "ScenePresets.h"

#include <AngularSpringForce.h>
#include <LinearDragForce.h>
#include <LinearForce.h>
#include <QuadraticDragForce.h>
#include "Plane.h"

#include "RodConstraintVar.h"

#include "CircularWireConstraint.h"
#include "GravitationalForce.h"
#include "RodConstraint.h"
#include "SpringForce.h"
#include <cmath>
#include "gfx/vec2.h"
#include "FixedEndpointSpringForce.h"

void three_body_problem_scene(std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
                              std::vector<Constraint *> &cVector, bool visualizeForces) {
    pVector.push_back(new Particle(Vec2f(-0.3, 0.0), visualizeForces, pVector.size(), 1.0, Vec2f(0.0, 0.0)));
    pVector.push_back(new Particle(Vec2f(0, 0.0), visualizeForces, pVector.size(), 10.0, Vec2f(0.0, -0.07)));
    pVector.push_back(new Particle(Vec2f(-0.4, 0.0), visualizeForces, pVector.size(), 5.0, Vec2f(0.0, 0.15)));

    fVector.push_back(new GravitationalForce(pVector, 0.001));
}

void set_scene(int scene, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
               std::vector<Constraint *> &cVector, std::vector<CollideableObject *> &oVector, bool visualizeForces) {
    const double dist = 0.2;
    const Vec2f center(0.0, 0.0);
    const Vec2f offset(0.0, dist);

    switch (scene) {
        case 1:
            pVector.push_back(new Particle(center + offset, visualizeForces, 0));
            pVector.push_back(new Particle(center + offset + Vec2f(0.001, dist), visualizeForces, 1));

            fVector.push_back(new LinearForce({pVector[0], pVector[1]}, Vec2f(0.00, -0.03)));

            cVector.push_back(new CircularWireConstraint(pVector[0], center, dist, 0));
            cVector.push_back(new RodConstraint(pVector[0], pVector[1], dist, 1));
            break;

        case 2:
            pVector.push_back(new Particle(center + 0 * offset, visualizeForces, 0));
            pVector.push_back(new Particle(center + 1 * offset, visualizeForces, 1));
            pVector.push_back(new Particle(center + 2 * offset, visualizeForces, 2));

            fVector.push_back(new SpringForce(pVector[0], pVector[1], 0.1, 1.0, 0.5));
            fVector.push_back(new SpringForce(pVector[1], pVector[2], 0.1, 1.0, 0.5));
            break;
        case 3: {
            const double total_height = 0.5;
            const size_t particles = 3;
            const double rod_length = total_height / particles;
            const Vec2f left(-0.5, 0.0);
            const Vec2f right(0.5, 0.0);

            std::vector<Particle *> right_particles;

            for (size_t i = 0; i < particles - 1; i++) {
                pVector.push_back(new Particle(left + (i + 1) * Vec2f(0, rod_length), visualizeForces, pVector.size()));
                pVector.push_back(
                        new Particle(right + (i + 1) * Vec2f(0, rod_length), visualizeForces, pVector.size()));
                right_particles.push_back(pVector.back());
            }
            pVector.push_back(
                    new Particle(left + particles * Vec2f(0.001, rod_length), visualizeForces, pVector.size()));
            pVector.push_back(
                    new Particle(right + particles * Vec2f(0.001, rod_length), visualizeForces, pVector.size()));

            fVector.push_back(new LinearForce(pVector, Vec2f(0.00, -0.03)));
            fVector.push_back(new QuadraticDragForce(right_particles, 1.0));

            cVector.push_back(new CircularWireConstraint(pVector[0], left, rod_length, cVector.size()));
            cVector.push_back(new CircularWireConstraint(pVector[1], right, rod_length, cVector.size()));
            for (size_t i = 1; i < particles; i++) {
                cVector.push_back(new RodConstraintVar(pVector[2 * i], pVector[2 * i - 2], rod_length, cVector.size()));
                cVector.push_back(
                        new RodConstraintVar(pVector[2 * i + 1], pVector[2 * i - 1], rod_length, cVector.size()));
            }

        } break;
        case 4: {
            constexpr double total_height = 0.8;
            constexpr size_t particles = 15;
            constexpr double rod_length = total_height / particles;

            for (size_t i = 0; i < particles; i++) {
                pVector.push_back(
                        new Particle(center + (i + 1) * Vec2f(0, rod_length), visualizeForces, pVector.size()));
            }

            for (size_t i = 2; i < particles; i++) {
                fVector.push_back(new AngularSpringForce(pVector[i - 2], pVector[i - 1], pVector[i], 90 + (i % 2) * 180,
                                                         0.1, 0.05));
            }

            for (size_t i = 1; i < particles; i++) {
                cVector.push_back(new RodConstraint(pVector[i], pVector[i - 1], rod_length, cVector.size()));
            }
        } break;
        case 5:
            three_body_problem_scene(pVector, fVector, cVector, visualizeForces);
            break;
        case 6:
            pVector.push_back(new Particle(center, visualizeForces, 0));
            oVector.push_back(new Plane(Vec2f(0.0, -0.5), Vec2f(0.0, 1.0), pVector, 0.001));
            fVector.push_back(new LinearForce({pVector[0]}, Vec2f(0.00, -0.03)));
            break;
        case 7: {
            Vec2f position = Vec2f(-0.75, 0.5);
            pVector.push_back(new Particle(position + offset, visualizeForces, 0));
            pVector.push_back(new Particle(position, visualizeForces, 1));
            pVector.push_back(new Particle(position - offset, visualizeForces, 2));
            oVector.push_back(new Plane(Vec2f(0.0, -0.5), Vec2f(0.5, 1.0), pVector, 0.001, 0.75));
            fVector.push_back(new LinearForce({pVector[0], pVector[1], pVector[2]}, Vec2f(0.00, -0.03)));
        } break;
		case 8:
			{
				constructCloth(8, 40, 0.045, true, false, pVector, fVector, cVector, visualizeForces);
                attachCloth(40, 0.045, pVector, fVector, cVector, visualizeForces);
			}
            break;

        default:
            pVector.push_back(new Particle(center, visualizeForces, 0));
            break;
    }
}


void constructCloth(size_t rows, size_t cols, double spacing, bool diagonal, bool useRods, std::vector<Particle *> &pVector,
                     std::vector<Force *> &fVector, std::vector<Constraint *> &cVector, bool visualizeForces) {
    const double offset = 0.5 * cols * spacing;
    for (size_t i = 0; i < rows; i++) { // Creating particle grid
        for (size_t j = 0; j < cols; j++) {
            pVector.push_back(new Particle(Vec2f(j * spacing - offset, -(i * spacing)), visualizeForces, pVector.size()));
        }
    }

    size_t idx = 0;
    spacing += 0.001;
    double diagonalDist = std::sqrt(spacing*spacing + spacing*spacing);
    double spring_stiffness = 8.0;
    double spring_damping = 0.5;
    double spring_stiffness_diagonal = 8.0;
    double spring_damping_diagonal = 0.5;
    // Adding springs/rods, diagonals are optional
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            idx = i * cols + j;
            if (j != cols - 1) {
                if (useRods) {
                	cVector.push_back(new RodConstraintVar(pVector[idx], pVector[idx + 1], spacing, cVector.size()));
                } else {
                	fVector.push_back(new SpringForce(pVector[idx], pVector[idx + 1], spacing, spring_stiffness, spring_damping));
                }
                if (diagonal && i != rows - 1) {	// right-down diagonal
                    if (useRods) {
                    	cVector.push_back(new RodConstraintVar(pVector[idx], pVector[idx + cols + 1], diagonalDist, cVector.size()));
                    } else {
                    	fVector.push_back(new SpringForce(pVector[idx], pVector[idx + cols + 1], diagonalDist, spring_stiffness_diagonal, spring_damping_diagonal));
                    }
            	}
            }
            if (i != rows - 1) {	// Vertical
				if (useRods) {
                	cVector.push_back(new RodConstraintVar(pVector[idx], pVector[idx + cols], spacing, cVector.size()));
                } else {
                	fVector.push_back(new SpringForce(pVector[idx], pVector[idx + cols], spacing, spring_stiffness, spring_damping));
                }
				if (diagonal && j != 0) {	// left-down diagonal
                    if (useRods) {
                    	cVector.push_back(new RodConstraintVar(pVector[idx], pVector[idx + cols - 1], diagonalDist, cVector.size()));
                    } else {
                    	fVector.push_back(new SpringForce(pVector[idx], pVector[idx + cols - 1], diagonalDist, spring_stiffness_diagonal, spring_damping_diagonal));
                    }
				}
            }
        }
    }
}


void attachCloth(size_t cols, double spacing, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
                 std::vector<Constraint *> &cVector, bool visualizeForces){
    const double centerOffset = 0.5 * cols * spacing;
    const double circleRadius = 0.001;
    const double supportHeight = 2 * spacing;  // Height relative to cloth
    const size_t nrSupport = 5; // Amount of constraint support particel
    const size_t interPos =  cols / nrSupport; // For dividing the supports uniformly

    for (size_t i = 0; i < nrSupport; i++) {
        double center = (i + 0.5) * interPos;   // Center of this support particle
        size_t idx1 = static_cast<size_t>(center - 0.5);
        size_t idx2 = static_cast<size_t>(center + 0.5);

        // Clamp to valid indices
        idx1 = std::min(idx1, pVector.size() - 1);
        idx2 = std::min(idx2, pVector.size() - 1);

        Vec2f pos((idx1 + idx2) * 0.5 * spacing - centerOffset, supportHeight);

        double euclidean = std::sqrt(
        (pVector[idx1]->m_ConstructPos[0] - pos[0]) * (pVector[idx1]->m_ConstructPos[0] - pos[0]) +
            (pVector[idx1]->m_ConstructPos[1] - pos[1]) * (pVector[idx1]->m_ConstructPos[1] - pos[1])
        );
        // std::cout << pVector[idx1]->m_ConstructPos << " " << pos <<  " " << euclidean << std::endl;
        // Connecting to bottom two particles
        fVector.push_back(new FixedEndpointSpringForce(pVector[idx1], pos, 0, 10.0, 0.1));
        fVector.push_back(new FixedEndpointSpringForce(pVector[idx2], pos, 0, 10.0, 0.1));

        fVector.push_back(new LinearForce(pVector, Vec2f(0.0, -0.01)));
        //Wind
        fVector.push_back(new QuadraticDragForce(pVector, 2.0));
    }
}