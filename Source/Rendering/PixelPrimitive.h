#pragma once

#include "Primitive.h"
#include "Shader.h"
#include "../Math/Color.h"

class PixelPrimitive : Primitive
{
public:
    PixelPrimitive(const Color& color);
    
    void Draw(const Vec2<float>& position) override;

private:
    Color color;
    
    //Buffers only need to be set up once
    inline static unsigned int vertexArrayObject = 0;
    inline static unsigned int vertexBufferObject = 0;

    //Shader only needs to be initialized once
    inline static std::shared_ptr<Shader> PixelShader = nullptr;
    
    inline static bool initialized = false;
};
