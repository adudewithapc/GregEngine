#include "SpritePrimitive.h"

#include <glad.h>

#include "Window.h"
#include "../2D/Camera2D.h"
#include "../Textures/Image.h"

static const unsigned int spriteIndices[] = {
    0, 1, 3,
    1, 2, 3
};

SpritePrimitive::SpritePrimitive(const std::string& textureLocation)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    Image image(textureLocation);

    glTexImage2D(GL_TEXTURE_2D, 0, image.GetChannels() == 4 ? GL_RGBA : GL_RGB, image.GetWidth(), image.GetHeight(), 0, image.GetChannels() == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image.GetData());
    glGenerateMipmap(GL_TEXTURE_2D);

    int vertexElements;
    const std::unique_ptr<float> coordinates = GetWindowCoordinates(image.GetWidth(), image.GetHeight(), vertexElements);
    const float* const coordinatesPointer = coordinates.get();

    glGenVertexArrays(1, &spriteVAO);
    glBindVertexArray(spriteVAO);

    unsigned int spriteVBO;
    glGenBuffers(1, &spriteVBO);
    glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordinatesPointer) * vertexElements, coordinatesPointer, GL_STATIC_DRAW);

    unsigned int spriteEBO;
    glGenBuffers(1, &spriteEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spriteEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spriteIndices), spriteIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if(!SpriteShader)
        SpriteShader = std::make_shared<Shader>("Resources/Shader/vertex.shader", "Resources/Shader/fragment.shader");
    
    shaderInstance = SpriteShader;
    shaderInstance->Use();

    glUniform1i(glGetUniformLocation(shaderInstance->programID, "texture1"), 0);
}

void SpritePrimitive::Draw(const Vec2f& position)
{
    shaderInstance->Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(spriteVAO);

    Camera2D::Get().Draw(*shaderInstance, Window::PixelToView(position));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::unique_ptr<float> SpritePrimitive::GetWindowCoordinates(int textureWidth, int textureHeight, int& elements) const
{
    elements = 20;
    const float widthOnWindow = static_cast<float>(textureWidth) / Window::WindowWidth;
    const float heightOnWindow = static_cast<float>(textureHeight) / Window::WindowHeight;
    float* coordinatesRelativeToWindow = new float[elements] {
        //x              y               z tx ty
        widthOnWindow,   heightOnWindow, 0, 1, 0, //Top right
        widthOnWindow,  -heightOnWindow, 0, 1, 1, //Bottom right
        -widthOnWindow, -heightOnWindow, 0, 0, 1, //Bottom left
        -widthOnWindow,  heightOnWindow, 0, 0, 0  //Top left
    };
    std::unique_ptr<float> ret(coordinatesRelativeToWindow);
    return std::move(ret);
}
