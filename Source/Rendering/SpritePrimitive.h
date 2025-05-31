#pragma once

#include "Primitive.h"
#include "Shader.h"
#include "../Textures/Image.h"

class SpritePrimitive : public Primitive
{
public:
    SpritePrimitive(const std::string& textureLocation);

    void Draw() override;

private:
    static void SetupBuffers();

    std::shared_ptr<Shader> shaderInstance;
    unsigned int textureID;
    Image image;
    
    inline static unsigned int spriteVAO;

    //We only want to load this shader once
    inline static std::shared_ptr<Shader> SpriteShader = nullptr;
    inline static bool PrimitiveInitialized = false;

};
