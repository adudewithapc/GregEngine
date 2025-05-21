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

	void Update() override;
	
private:
	std::shared_ptr<SpritePrimitive> primitive;
};

