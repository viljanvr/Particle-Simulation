#include "ScenePresets.h"
#include "CircularWireConstraint.h"
#include "GravityForce.h"
#include "RodConstraint.h"
#include "SpringForce.h"

void set_scene(int scene, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
               std::vector<Constraint *> &cVector) {
    const double dist = 0.2;
    const Vec2f center(0.0, 0.0);
    const Vec2f offset(0.0, dist);

    switch (scene) {
        case 1:
            pVector.push_back(new Particle(center + offset, 0));
            pVector.push_back(new Particle(center + offset + Vec2f(0.001, dist), 1));

            fVector.push_back(new GravityForce({pVector[0], pVector[1]}, Vec2f(0.00, -0.03)));

            cVector.push_back(new CircularWireConstraint(pVector[0], center, dist, 0));
            cVector.push_back(new RodConstraint(pVector[0], pVector[1], dist, 1));
            break;

        case 2:
            pVector.push_back(new Particle(center + 0 * offset, 0));
            pVector.push_back(new Particle(center + 1 * offset, 1));
            pVector.push_back(new Particle(center + 2 * offset, 2));

            fVector.push_back(new SpringForce(pVector[0], pVector[1], 0.1, 1.0, 1.0));
            fVector.push_back(new SpringForce(pVector[1], pVector[2], 0.1, 1.0, 1.0));
            break;
        case 3:
            {
                const double total_height = 0.8;
                const size_t particles = 5;
                const double rod_length = total_height / particles;

                for (size_t i = 0; i < particles - 1; i++) {
                    pVector.push_back(new Particle(center + (i + 1) * Vec2f(0, rod_length), i));
                }
                pVector.push_back(new Particle(center + particles * Vec2f(0.001, rod_length), particles - 1));

                fVector.push_back(new GravityForce(pVector, Vec2f(0.00, -0.03)));

                cVector.push_back(new CircularWireConstraint(pVector[0], center, rod_length, 0));
                for (size_t i = 1; i < particles; i++) {
                    cVector.push_back(new RodConstraint(pVector[i], pVector[i - 1], rod_length, i));
                }

            }
            break;


        default:
            pVector.push_back(new Particle(center, 0));
            break;
    }
}
