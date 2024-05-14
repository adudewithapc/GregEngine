#include "Level.h"

Level::Level()
{
	GameObject test;
	gameObjects.push_back(test);
}

void Level::Update()
{
	for(GameObject& gameObject : gameObjects)
	{
		gameObject.Update();
	}
}

void Level::Draw()
{
	for(GameObject& gameObject : gameObjects)
	{
		gameObject.Draw();
	}
}
