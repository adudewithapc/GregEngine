#pragma once

#include <Component/Component.h>
#include <vector>

class GameObject;

class PlayerScore : public Component
{
public:
	PlayerScore(GameObject* owner);

	virtual void CollisionEntered(GameObject* other) override;
	virtual void Update() override;

private:
	std::vector<GameObject*> applesToDestroy;
	int score = 0;
};

