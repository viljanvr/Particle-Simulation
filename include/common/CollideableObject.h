#pragma once

#include "DrawableObject.h"

#include <Particle.h>
#include <vector>

struct Collision {
    Particle *p;

    // Remember to normalise this vector!
    Vec2f collisionNormal;

    // How much of needs to be backtracked? 1.0 means backtrack to previous position. 0.0 means do not backtrack.
    double backtracking_factor;
};

class CollideableObject : public DrawableObject {
public:
    explicit CollideableObject(double bounce);
    virtual ~CollideableObject() = default;
    // [[nodiscard]] std::vector<Collision> get_earliest_collisions() const;
    virtual bool is_particle_colliding(Particle *p) const = 0;
    virtual Collision compute_collision_details(Particle *p) const = 0;
    // void backtrack_particles(double backtracking_factor) const;
    void bounce_particle(const Collision &collision) const;

private:
    const double m_bounce;
};
