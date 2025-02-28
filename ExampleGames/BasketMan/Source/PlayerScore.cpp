#include "PlayerScore.h"
#include <GameObject.h>
#include <iostream>

PlayerScore::PlayerScore(GameObject* owner) : Component(owner)
{
}

void PlayerScore::CollisionEntered(GameObject* other)
{
	applesToDestroy.push_back(other);
	system("cls");
	std::cout << ++score << std::flush;
}

void PlayerScore::Update()
{
	if (applesToDestroy.size() == 0)
		return;

	for (GameObject* apple : applesToDestroy)
	{
		apple->Destroy();
	}
	applesToDestroy.clear();
}
