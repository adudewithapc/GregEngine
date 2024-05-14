#pragma once
#include <memory>
#include "2D/Sprite.h"
class GameObject
{
public:
	GameObject();

	void Update();
	void Draw();

	Vec2f Position;
private:
	Sprite sprite;
};

