#pragma once

#include <vector>
#include "GameObject.h"

class Level
{
public:
	Level();
	virtual ~Level() = default;

	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;

	Level(Level&&) = default;
	Level& operator=(Level&&) = default;

	void Update();
	void Draw(RenderTarget& target);

	GameObject* CreateObject();
	void DestroyObject(int objectIndex);
private:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

