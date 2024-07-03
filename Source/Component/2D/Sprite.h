#pragma once

#include <string>
#include <memory>
#include "../../GameObject.h"

class Shader;

class Sprite : public Component
{
public:
	Sprite(GameObject* owner, const std::string& textureLocation);

	virtual void Draw() override;
private:
	std::unique_ptr<float> GetScreenProportions(const float textureWidth, const float textureHeight, int& elements) const;

	unsigned int textureID;
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int quadEBO;

	static std::unique_ptr<Shader> SpriteShader;
};

