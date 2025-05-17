#pragma once
#include <Particle.h>
#include <vector>


class Force {
public:
    // Describes how much the force on p1 change if we modify the values of p2.
    struct JacobianEntry {
        Particle *const p1, *const p2;
        // How does the x force for p1 change if we modify the values of p2.
        double xx, xy;
        // How does the y force for p1 change if we modify the values of p2.
        double yx, yy;
    };

    virtual ~Force() = default;
    virtual void applyForce() = 0;
    virtual void draw() = 0;
    virtual std::vector<JacobianEntry> getJv() {
        throw std::runtime_error("Not implemented");
    };
    virtual std::vector<JacobianEntry> getJx() {
        throw std::runtime_error("Not implemented");
    }
};
