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
#include "gfx/vec2.h"

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
        default:

            pVector.push_back(new Particle(center, visualizeForces, 0));
            break;
    }
}
