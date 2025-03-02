#pragma once

#include <string>
#include <memory>
#include "../../GameObject.h"
#include "../../Rendering/SpritePrimitive.h"

class Shader;

class Sprite : public Component
{
public:
	Sprite(GameObject* owner, const std::string& textureLocation);

	virtual void Draw(RenderTarget& target) override;
private:
	SpritePrimitive primitive;

	static std::unique_ptr<Shader> SpriteShader;
};

