#include "Level.h"
#include <iostream>
#include "2D/Physics2D.h"
#include "Component/ConstantMovement.h"

Level::Level()
{
	
}

Level::~Level()
{
	gameObjects.clear();
}

void Level::Update()
{
	Physics2D::Tick();

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