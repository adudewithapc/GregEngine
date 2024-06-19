#pragma once

#include <vector>
#include "GameObject.h"

class Level
{
public:
	Level();
	virtual ~Level() = default;

	void Update();
	void Draw();

	GameObject* CreateObject();
private:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

