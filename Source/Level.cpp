#include "Level.h"
#include <iostream>
#include "2D/Physics2D.h"

void Level::Update()
{
	Physics2D::Tick();

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

GameObject& Level::CreateObject()
{
	return gameObjects.emplace_back();
}
