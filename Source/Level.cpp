#include "Level.h"
#include <iostream>
#include "2D/Physics2D.h"
#include "Component/ConstantMovement.h"

Level::Level()
{
	std::unique_ptr<GameObject> leftSmiley = std::make_unique<GameObject>();
	//leftSmiley->Position.x = -512;

	/*std::unique_ptr<GameObject> rightSmiley = std::make_unique<GameObject>();
	rightSmiley->Position.y = 512;
	ConstantMovement* newComponent = rightSmiley->AddComponent<ConstantMovement>(Vec2f(300, 0));*/

	gameObjects.push_back(std::move(leftSmiley));
	//gameObjects.push_back(std::move(rightSmiley));
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