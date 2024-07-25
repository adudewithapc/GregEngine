#pragma once
#include <Level.h>
class AppleSpawner : public Component
{
public:
	AppleSpawner(GameObject* owner, size_t width, float topOffset);


	virtual void Update() override;
private:
	void CreateApple();

	static constexpr float SPAWN_COOLDOWN = 1;
	int width;
	float topOffset;

	float spawnTimer = SPAWN_COOLDOWN;
};

