#pragma once
#include <vector>

class Force {
public:
    virtual ~Force() = default;
    virtual void applyForce() = 0;
    virtual void draw() = 0;
};
