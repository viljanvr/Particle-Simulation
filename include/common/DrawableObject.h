#pragma once

class DrawableObject {
public:
    virtual ~DrawableObject() = default;
    virtual void draw() = 0;
};
