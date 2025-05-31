#include "TrianglePrimitive.h"

#include "Vertex.h"

namespace greg
{
TrianglePrimitive::TrianglePrimitive()
: vertices(std::vector<Vertex>(3)),
  indices(std::vector<size_t>(3))
{
    vertices[0] = Vertex(Vec2f(0.0f, -0.5f), Vec3f(1.0f, 0.0f, 0.0f));
    vertices[1] = Vertex(Vec2f(0.5f, 0.5f), Vec3f(0.0f, 1.0f, 0.0f));
    vertices[2] = Vertex(Vec2f(-0.5f, 0.5f), Vec3f(0.0f, 0.0f, 1.0f));

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
}

const std::vector<greg::Vertex>& TrianglePrimitive::GetVertices() const
{
    return vertices;
}

const std::vector<size_t>& TrianglePrimitive::GetIndices() const
{
    return indices;
}
}
