#pragma once
#include <vector>

#include "Primitive.h"

namespace greg
{
class TrianglePrimitive : public Primitive
{
public:
    TrianglePrimitive();

    const std::vector<greg::Vertex>& GetVertices() const override;
    const std::vector<size_t>& GetIndices() const override;
private:
    std::vector<greg::Vertex> vertices;
    std::vector<size_t> indices;
};
}