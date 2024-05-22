#pragma once

#include <stb_image.h>
#include <string>
#include <memory>
#include "../Math/Vector.h"
#include "../Component.h"

class Shader;

class Sprite : public Component
{
public:
	Sprite(GameObject* owner, const std::string& textureLocation);
	~Sprite() override;

	virtual void Draw() override;
private:
    float* GetScreenProportions(const float textureWidth, const float textureHeight, int& elements) const;

	unsigned int textureID;
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int quadEBO;

	float* proportions;

	static std::unique_ptr<Shader> SpriteShader;
};

