#pragma once

#include <vector>
#include "GameObject.h"

class Level
{
public:
	Level() = default;
	virtual ~Level() = default;

	Level(const Level&) = delete;
	Level& operator =(const Level&) = delete;

	void Update();
	void Draw();

	GameObject& CreateObject();
private:
	std::vector<GameObject> gameObjects;
};

