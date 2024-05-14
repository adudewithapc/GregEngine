#pragma once

#include <stb_image.h>
#include <string>
#include <memory>
#include "../Math/Vector.h"

class Shader;

class Sprite
{
public:
	Sprite(const std::string& textureLocation);

	void Render(const Vec2f& position);
private:
	unsigned int textureID;
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int quadEBO;

	static std::unique_ptr<Shader> SpriteShader;
};

