#pragma once
#include "../Math/Vector.h"

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
    
    Vec2f Position;
};
