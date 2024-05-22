#include "Level.h"
#include <iostream>

Level::Level()
{
	gameObjects.push_back(GameObject());

	for(auto& gameObject : gameObjects)
	{
		gameObject.Initialize();
	}
}

Level::~Level()
{
	gameObjects.clear();
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
