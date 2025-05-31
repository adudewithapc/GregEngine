#pragma once

#include "../Math/Vector.h"
#include <vector>
#include "Vertex.h"

class Primitive
{
public:
    Primitive() = default;

    //These should not be default in the future
    Primitive(const Primitive& other) = default;
    Primitive& operator =(const Primitive& other) = default;

    Primitive(Primitive&& other) = default;
    Primitive& operator =(Primitive&& other) = default;
    
    virtual ~Primitive() = default;
    
    virtual void Draw() = 0;

    //TODO: Make these pure virtual
    virtual const std::vector<greg::Vertex>& GetVertices() const
    {
        return emptyVertices;
    }
    virtual const std::vector<uint32_t>& GetIndices() const
    {
        return emptyIndices;
    }
    
    Vec2f Position;

protected:
    const std::vector<greg::Vertex> emptyVertices = {};
    const std::vector<uint32_t> emptyIndices = {};
};
