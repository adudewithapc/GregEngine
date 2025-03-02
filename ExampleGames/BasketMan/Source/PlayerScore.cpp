#include "PlayerScore.h"
#include <GameObject.h>
#include <iostream>

PlayerScore::PlayerScore(GameObject* owner)
: Component(owner),
  collectAudio("Resources/Audio/collect.mp3")
{
}

void PlayerScore::CollisionEntered(GameObject* other)
{
	applesToDestroy.push_back(other);
	system("cls");
	std::cout << ++score << std::flush;
	collectAudio.Play();
}

void PlayerScore::Update()
{
	if (applesToDestroy.empty())
		return;

	for (GameObject* apple : applesToDestroy)
	{
		apple->Destroy();
	}
	applesToDestroy.clear();
}
