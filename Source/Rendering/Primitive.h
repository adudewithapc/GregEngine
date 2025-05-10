#pragma once
#include "../Math/Vector.h"

class Primitive
{
public:
    virtual ~Primitive() = default;
    
    virtual void Draw(const Vec2f& position) = 0;
};
