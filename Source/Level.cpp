#include "Level.h"
#include <iostream>
#include "2D/Physics2D.h"
#include "2D/Camera2D.h"

Level::Level() : gameObjects(std::vector<std::unique_ptr<GameObject>>())
{
}

void Level::Update()
{
	Physics2D::Tick();
	Camera2D::Get().Update();

	for(int i = gameObjects.size() - 1; i >= 0; i--)
	{
		gameObjects[i]->Update();
	}
}

void Level::Draw()
{
	for(int i = gameObjects.size() - 1; i >= 0; i--)
	{
		gameObjects[i]->Draw();
	}
}

GameObject* Level::CreateObject()
{
	gameObjects.push_back(std::make_unique<GameObject>(gameObjects.size()));
	return gameObjects.back().get();
}

void Level::DestroyObject(int objectIndex)
{
	gameObjects[objectIndex] = std::move(gameObjects[gameObjects.size() - 1]);
	//This is needed since the object index is actually its current position in the gameObjects list
	//This should probably be reworked in the future
	gameObjects[objectIndex]->SetIndex(objectIndex);
	gameObjects.erase(gameObjects.end() - 1);
}

