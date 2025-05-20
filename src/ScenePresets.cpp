#include "ScenePresets.h"

#include <AngularSpringForce.h>
#include <CircularCollisionObject.h>
#include <LinearDragForce.h>
#include <LinearForce.h>
#include <QuadraticDragForce.h>
#include <cmath>
#include <cstring>

#include "CircularWireConstraint.h"
#include "FixedEndpointSpringForce.h"
#include "GravitationalForce.h"
#include "Plane.h"
#include "RodConstraint.h"
#include "RodConstraintVar.h"
#include "SpringForce.h"
#include "gfx/vec2.h"

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

std::string currentSceneName = "Unnamed Scene";

void three_body_problem_scene(std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
                              std::vector<Constraint *> &cVector, bool visualizeForces) {
    pVector.push_back(new Particle(Vec2f(-0.3, 0.0), visualizeForces, pVector.size(), 1.0, Vec2f(0.0, 0.0)));
    pVector.push_back(new Particle(Vec2f(0, 0.0), visualizeForces, pVector.size(), 10.0, Vec2f(0.0, -0.07)));
    pVector.push_back(new Particle(Vec2f(-0.4, 0.0), visualizeForces, pVector.size(), 5.0, Vec2f(0.0, 0.15)));

    fVector.push_back(new GravitationalForce(pVector, 0.001));
}

void hairy_head_scene(std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
                      std::vector<Constraint *> &cVector, std::vector<CollideableObject *> &oVector,
                      bool visualizeForces) {
    double head_radius = 0.3;
    size_t hair_count = 10;
    // which percentage of the head circumference is used to attach hairs?
    double sector = 0.3;
    double hair_segment_length = 0.12;
    size_t hair_segment_count = 7;
    double angle_between_hair_segments = 110;

    double first_hair_angle = -sector * PI;
    double last_hair_angle = sector * PI;
    double angle_between_hairs = (last_hair_angle - first_hair_angle) / (hair_count - 1.0);

    for (size_t i = 0; i < hair_count; i++) {
        double angle = first_hair_angle + i * angle_between_hairs;
        Vec2f head_delta = Vec2f(sin(angle) * head_radius, cos(angle) * head_radius);
        Vec2f segment_delta = Vec2f(sin(angle) * hair_segment_length, cos(angle) * hair_segment_length);

        for (size_t j = 0; j < hair_segment_count; j++) {
            pVector.push_back(new Particle(head_delta + (j + 1) * segment_delta, visualizeForces, pVector.size()));
            if (j == 0) {
                cVector.push_back(
                        new CircularWireConstraint(pVector.back(), head_delta, hair_segment_length, cVector.size()));
            } else {
                // fVector.push_back(new SpringForce(pVector.back(), pVector[pVector.size() - 2], hair_segment_length,
                // 100.0, 10.0));
                cVector.push_back(new RodConstraint(pVector.back(), pVector[pVector.size() - 2], hair_segment_length,
                                                    cVector.size()));
                if (j >= 3) {
                    double segment_angle = angle_between_hair_segments;
                    if (j % 2 == 0) {
                        segment_angle = 360 - segment_angle;
                    }
                    fVector.push_back(new AngularSpringForce(pVector[pVector.size() - 3], pVector[pVector.size() - 2],
                                                             pVector[pVector.size() - 1], segment_angle, 0.1, 0.05));
                }
            }
        }
    }

    fVector.push_back(new LinearForce(pVector, {0, -0.008}));
    oVector.push_back(new CircularCollisionObject({0, 0}, head_radius, pVector, 0.03, 0.4));
}

void collider_scene(
    std::vector<Particle *> &pVector,
    std::vector<Force *> &fVector,
    std::vector<Constraint *> &cVector,
    std::vector<CollideableObject * > &oVector,
    bool visualizeForces
) {
    currentSceneName = "Collisions with planes and circles";
    pVector.push_back(new Particle({0.0, 1.0}, visualizeForces, pVector.size()));
    pVector.push_back(new Particle({0.05, 1.1}, visualizeForces, pVector.size()));
    pVector.push_back(new Particle({-0.05, 1.2}, visualizeForces, pVector.size()));
    pVector.push_back(new Particle({0.07, 1.3}, visualizeForces, pVector.size()));
    pVector.push_back(new Particle({-0.07, 1.4}, visualizeForces, pVector.size()));
    fVector.push_back(new LinearForce(pVector, {0, -0.04}));
    fVector.push_back(new QuadraticDragForce(pVector, 0.5));
    oVector.push_back(new CircularCollisionObject({0.01, 0.0}, 0.2, pVector, 0.01, 0.7));
    oVector.push_back(new Plane({-0.5, -0.7}, {3.0, 1.0}, pVector, 0.01, 0.7));
    oVector.push_back(new Plane({0.2, -0.7}, {-1.5, 1.0}, pVector, 0.01, 0.7));
    oVector.push_back(new Plane({0.0, -0.6}, {0.15, 1.0}, pVector, 0.01, 0.7));
}

void set_scene(int scene, std::vector<Particle *> &pVector, std::vector<Force *> &fVector,
               std::vector<Constraint *> &cVector, std::vector<CollideableObject *> &oVector, bool visualizeForces) {
    const double dist = 0.2;
    const Vec2f center(0.0, 0.0);
    const Vec2f offset(0.0, dist);

    switch (scene) {
        case 1:
            currentSceneName = std::to_string(scene) + ". Pendulum";

            pVector.push_back(new Particle(center + offset, visualizeForces, 0));
            pVector.push_back(new Particle(center + offset + Vec2f(0.001, dist), visualizeForces, 1));

            fVector.push_back(new LinearForce({pVector[0], pVector[1]}, Vec2f(0.00, -0.03)));

            cVector.push_back(new CircularWireConstraint(pVector[0], center, dist, 0));
            cVector.push_back(new RodConstraint(pVector[0], pVector[1], dist, 1));
            break;

        case 2:
            currentSceneName = std::to_string(scene) + ". Double spring";
            pVector.push_back(new Particle(center + 0 * offset, visualizeForces, 0));
            pVector.push_back(new Particle(center + 1 * offset, visualizeForces, 1));
            pVector.push_back(new Particle(center + 2 * offset, visualizeForces, 2));

            fVector.push_back(new SpringForce(pVector[0], pVector[1], 0.1, 1.0, 0.5));
            fVector.push_back(new SpringForce(pVector[1], pVector[2], 0.1, 1.0, 0.5));
            break;
        case 3: {
            currentSceneName = std::to_string(scene) + ". None vs. Quadratic Drag Force";

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
            currentSceneName = std::to_string(scene) + ". Angular Springs";
            constexpr double total_height = 0.8;
            constexpr size_t particles = 15;
            constexpr double rod_length = total_height / particles;
            Vec2f offset(0.0, -0.3);

            for (size_t i = 0; i < particles; i++) {
                pVector.push_back(new Particle(center + (i + 1) * Vec2f(0, rod_length) + offset, visualizeForces,
                                               pVector.size()));
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
            currentSceneName = std::to_string(scene) + ". Three-Body Problem";
            three_body_problem_scene(pVector, fVector, cVector, visualizeForces);
            break;
        case 6:
            currentSceneName = std::to_string(scene) + ". Ground Collision";
            pVector.push_back(new Particle(center, visualizeForces, 0));
            oVector.push_back(new Plane(Vec2f(0.0, -0.5), Vec2f(0.0, 1.0), pVector));
            fVector.push_back(new LinearForce({pVector[0]}, Vec2f(0.00, -0.03)));
            break;
        case 7: {
            currentSceneName = std::to_string(scene) + ". Down-Hill Collision";
            Vec2f position = Vec2f(-0.75, 0.5);
            pVector.push_back(new Particle(position + offset, visualizeForces, 0));
            pVector.push_back(new Particle(position, visualizeForces, 1));
            pVector.push_back(new Particle(position - offset, visualizeForces, 2));
            oVector.push_back(new Plane(Vec2f(0.0, -0.5), Vec2f(0.5, 1.0), pVector, 0.001, 0.7));
            fVector.push_back(new LinearForce({pVector[0], pVector[1], pVector[2]}, Vec2f(0.00, -0.03)));
        } break;
        case 8: {
            currentSceneName = std::to_string(scene) + ". Cloth";
            constructCloth(Vec2f(0.0, 0.0), 8, 40, 0.045, true, false, pVector, fVector, cVector, visualizeForces);
            attachCloth(Vec2f(0.0, 0.0), 8, 40, 0.045, pVector, fVector, cVector, visualizeForces);
        } break;
        case 9: {
            currentSceneName = std::to_string(scene) + ". Hairy head";
            hairy_head_scene(pVector, fVector, cVector, oVector, visualizeForces);
            break;
        }

        case 0: {
            currentSceneName = std::to_string(scene) + ". Cloth collision";
            constructCloth(Vec2f(0.0, 0.0), 20, 5, 0.045, true, false, pVector, fVector, cVector, visualizeForces);
            oVector.push_back(new Plane(Vec2f(0.9, 0.0), Vec2f(-1.0, 0.0), pVector, 0.04, 0.5));
        } break;

        default:
            currentSceneName = "Default: Single Particle";
            pVector.push_back(new Particle(center, visualizeForces, 0));
            break;
    }
}


void constructCloth(Vec2f origin, size_t rows, size_t cols, double spacing, bool diagonal, bool useRods,
                    std::vector<Particle *> &pVector, std::vector<Force *> &fVector, std::vector<Constraint *> &cVector,
                    bool visualizeForces) {
    const Vec2f offset(-0.5 * cols * spacing, 0.5 * rows * spacing);

    for (size_t i = 0; i < rows; i++) { // Creating particle grid
        for (size_t j = 0; j < cols; j++) {
            Vec2f pos = origin + Vec2f(j * spacing, -(i * spacing)) + offset;
            pVector.push_back(new Particle(pos, visualizeForces, pVector.size()));
        }
    }

    size_t idx = 0;
    spacing += 0.001;
    double diagonalDist = std::sqrt(spacing * spacing + spacing * spacing);
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
                    fVector.push_back(
                            new SpringForce(pVector[idx], pVector[idx + 1], spacing, spring_stiffness, spring_damping));
                }
                if (diagonal && i != rows - 1) { // right-down diagonal
                    if (useRods) {
                        cVector.push_back(new RodConstraintVar(pVector[idx], pVector[idx + cols + 1], diagonalDist,
                                                               cVector.size()));
                    } else {
                        fVector.push_back(new SpringForce(pVector[idx], pVector[idx + cols + 1], diagonalDist,
                                                          spring_stiffness_diagonal, spring_damping_diagonal));
                    }
                }
            }
            if (i != rows - 1) { // Vertical
                if (useRods) {
                    cVector.push_back(new RodConstraintVar(pVector[idx], pVector[idx + cols], spacing, cVector.size()));
                } else {
                    fVector.push_back(new SpringForce(pVector[idx], pVector[idx + cols], spacing, spring_stiffness,
                                                      spring_damping));
                }
                if (diagonal && j != 0) { // left-down diagonal
                    if (useRods) {
                        cVector.push_back(new RodConstraintVar(pVector[idx], pVector[idx + cols - 1], diagonalDist,
                                                               cVector.size()));
                    } else {
                        fVector.push_back(new SpringForce(pVector[idx], pVector[idx + cols - 1], diagonalDist,
                                                          spring_stiffness_diagonal, spring_damping_diagonal));
                    }
                }
            }
        }
    }
}


void attachCloth(Vec2f origin, size_t rows, size_t cols, double spacing, std::vector<Particle *> &pVector,
                 std::vector<Force *> &fVector, std::vector<Constraint *> &cVector, bool visualizeForces) {
    // const double centerOffset = 0.5 * cols * spacing;
    const Vec2f offset(-0.5 * cols * spacing, 0.5 * rows * spacing);
    const double supportHeight = 2 * spacing; // Height relative to cloth
    const size_t nrSupport = 5; // Amount of constraint support particel
    const size_t interPos = cols / nrSupport; // For dividing the supports uniformly

    for (size_t i = 0; i < nrSupport; i++) {
        double center = (i + 0.5) * interPos; // Center of this support particle
        size_t idx1 = static_cast<size_t>(center - 0.5);
        size_t idx2 = static_cast<size_t>(center + 0.5);

        // Clamp to valid indices
        idx1 = std::min(idx1, pVector.size() - 1);
        idx2 = std::min(idx2, pVector.size() - 1);

        Vec2f pos = origin + Vec2f((idx1 + idx2) * 0.5 * spacing, supportHeight) + offset;

        // std::cout << pVector[idx1]->m_ConstructPos << " " << pos <<  " " << euclidean << std::endl;
        // Connecting to bottom two particles
        fVector.push_back(new FixedEndpointSpringForce(pVector[idx1], pos, 0, 10.0, 0.1));
        fVector.push_back(new FixedEndpointSpringForce(pVector[idx2], pos, 0, 10.0, 0.1));

        fVector.push_back(new LinearForce(pVector, Vec2f(0.0, -0.01)));
        // Wind
        fVector.push_back(new QuadraticDragForce(pVector, 2.0));
    }
}

void drawText(const char *text, float x, float y) {
    // glClear(GL_COLOR_BUFFER_BIT);
    // // glBegin(GL_LINES);
    // glColor3f(1.0f, 1.0f, 1.0f);
    // glRasterPos2f(x, y);
    // for (size_t i = 0; i < strlen(text); ++i) {
    //     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    // }
    // // glEnd();
    // glutSwapBuffers();
    glBegin(GL_LINES);
    glColor3f(0.8, 0.7, 0.6);
    glVertex2f(0.0, 0.0);
    glColor3f(0.8, 0.7, 0.6);
    glVertex2f(0.0, 1.0);
    glEnd();
}
