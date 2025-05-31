#include "Vertex.h"

namespace greg
{
Vertex::Vertex()
  : position(Vec2f(0, 0)),
    color(Color(0, 0, 0, 0))
{
  
}

Vertex::Vertex(const Vec2<float>& position, const Color& color)
: position(position),
  color(color) {}
}
