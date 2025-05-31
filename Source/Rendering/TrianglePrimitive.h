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
    const std::vector<uint32_t>& GetIndices() const override;
    
    //Temp
    void Draw() override {}
private:
    std::vector<greg::Vertex> vertices;
    std::vector<uint32_t> indices;
};
}