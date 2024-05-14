#pragma once

#include <vector>
#include "GameObject.h"

class Level
{
public:
	Level();

	void Update();
	void Draw();
private:
	std::vector<GameObject> gameObjects;
};

