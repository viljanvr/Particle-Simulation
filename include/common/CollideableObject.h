#pragma once

#include "DrawableObject.h"

#include <Particle.h>

struct Collision {
    Particle *p;
    Vec2f collisionNormal;

    // How much of (previous_position -> new_position) needs to be backtracked?
    double backtracking_factor;
};

class CollideableObject : public DrawableObject {
public:
    explicit CollideableObject(std::vector<Particle *> particles, double bounce);
    virtual ~CollideableObject() = default;
    Collision get_earliest_collision() const;
    virtual bool is_particle_colliding(Particle *p) const = 0;
    virtual Collision compute_collision_details(Particle *p) const = 0;
    void backtrack_particles(double backtracking_factor) const;
    void bounce_single_particle(const Collision &collision) const;

private:
    std::vector<Particle *> m_particles;
    const double m_bounce;
};
