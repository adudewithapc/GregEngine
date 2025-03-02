#pragma once
#include "../Math/Vector.h"

class Primitive
{
public:
    virtual void Draw(const Vec2f& position) = 0;
};
