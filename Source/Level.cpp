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

	for(auto& gameObject : gameObjects)
	{
		gameObject->Update();
	}
}

void Level::Draw()
{
	for(auto& gameObject : gameObjects)
	{
		gameObject->Draw();
	}
}

GameObject* Level::CreateObject()
{
	gameObjects.push_back(std::make_unique<GameObject>());
	return gameObjects.back().get();
}
