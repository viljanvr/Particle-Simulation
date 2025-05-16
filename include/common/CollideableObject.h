#pragma once

#include "DrawableObject.h"
class CollideableObject : public DrawableObject {
public:
    virtual ~CollideableObject() = default;
    virtual void detect_collision() = 0;
};
