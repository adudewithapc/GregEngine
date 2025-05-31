#pragma once

#include "../Math/Vector.h"
#include "../Math/Color.h"

namespace greg
{
struct Vertex
{
public:
    Vertex(const Vec2f& position, const Color& color);
    
    Vec2f position;
    Color color;
};
}