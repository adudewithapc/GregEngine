#pragma once
#include "Primitive.h"
#include "Shader.h"

class SpritePrimitive : public Primitive
{
public:
    SpritePrimitive(const std::string& textureLocation);

    void Draw(const Vec2f& position) override;

private:
    //Transforms sprite's coordinates from absolute to relative to window
    std::unique_ptr<float> GetWindowCoordinates(int textureWidth, int textureHeight, int& elements) const;

    std::shared_ptr<Shader> shaderInstance;
    
    unsigned int textureID;
    unsigned int spriteVAO;

    //We only want to load this shader once
    inline static std::shared_ptr<Shader> SpriteShader = nullptr;
};
