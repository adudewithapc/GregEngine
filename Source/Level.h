#pragma once

#include <vector>
#include "GameObject.h"

class Level
{
public:
	Level();
	~Level();

	void Update();
	void Draw();
private:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

