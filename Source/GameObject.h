#pragma once

#include <memory>
#include "2D/Sprite.h"
#include "Component.h"
#include <vector>

class GameObject
{
public:
	GameObject();

	void Initialize();

	void Update();
	void Draw();

	Vec2f Position;
private:
	std::vector<std::unique_ptr<Component>> components;
};