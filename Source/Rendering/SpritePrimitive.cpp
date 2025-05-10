#include "SpritePrimitive.h"

#include <glad.h>

#include "Window.h"
#include "../2D/Camera2D.h"

static const unsigned int spriteIndices[] = {
    0, 1, 3,
    1, 2, 3
};

static const float spriteVertices[] = {
//   x       y     z     tx    ty
     0.5f,   0.5f, 0.0f, 1.0f, 0.0f, //Top right
     0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, //Bottom right
    -0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, //Bottom left
    -0.5f,   0.5f, 0.0f, 0.0f, 0.0f  //Top left
};

SpritePrimitive::SpritePrimitive(const std::string& textureLocation)
    : image(textureLocation)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, image.GetChannels() == 4 ? GL_RGBA : GL_RGB, image.GetWidth(), image.GetHeight(), 0, image.GetChannels() == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image.GetData());
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenVertexArrays(1, &spriteVAO);
    glBindVertexArray(spriteVAO);

    unsigned int spriteVBO;
    glGenBuffers(1, &spriteVBO);
    glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertices), spriteVertices, GL_STATIC_DRAW);

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
        SpriteShader = std::make_shared<Shader>("Resources/Shader/sprite_vertex.shader", "Resources/Shader/sprite_fragment.shader");
    
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

    //TODO: Tie this into the rendering system, rather than using a hard coded render target
    //Requires the rendering system to be more complete
    Mat4x4f modelMatrix = mat4x4::Identity<float>.Translate(position).Scale(Vec3f(image.GetWidth(), image.GetHeight(), 1));
    Camera2D::Get().Draw(*shaderInstance, modelMatrix);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}