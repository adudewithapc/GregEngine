#include "PixelPrimitive.h"

#include <glad.h>

#include "../2D/Camera2D.h"

PixelPrimitive::PixelPrimitive(const Color& color, unsigned int pixelSize)
: color(color),
  pixelSize(pixelSize)
{
    //Initialization only needs to happen once
    if(initialized)
        return;

    glEnable(GL_PROGRAM_POINT_SIZE);

    PixelShader = std::make_shared<Shader>("Resources/Shader/pixel_vertex.shader", "Resources/Shader/pixel_fragment.shader");

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glBindVertexArray(0);
    
    initialized = true;
}

void PixelPrimitive::Draw()
{
    PixelShader->Use();

    Mat4x4f modelMatrix = mat4x4::Identity<float>.Translate(Position);
    Camera2D::Get().Draw(*PixelShader, modelMatrix);
    
    glBindVertexArray(vertexArrayObject);

    PixelShader->SetColor("color", color);
    PixelShader->SetInt("pixelSize", pixelSize);
    
    glDrawArrays(GL_POINTS, 0, 1);
}