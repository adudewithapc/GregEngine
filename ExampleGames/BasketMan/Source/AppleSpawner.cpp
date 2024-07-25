#include "AppleSpawner.h"
#include <Component/2D/Sprite.h>
#include "Apple.h"
#include <GregTime.h>
#include <GregorianEngine.h>
#include <Component/2D/BoxCollider2D.h>

AppleSpawner::AppleSpawner(GameObject* owner, size_t width, float topOffset) : Component(owner), width(width), topOffset(topOffset)
{
}

void AppleSpawner::CreateApple()
{
	GameObject* apple = GregorianEngine::Get().GetCurrentLevel()->CreateObject();
	
	apple->AddComponent<Sprite>("Textures/apple.png");
	
	apple->AddComponent<Apple>();
	apple->Position = Vec2f(-width + rand() % (width * 2), topOffset);

	apple->AddComponent<BoxCollider2D>(Vec2f(32, 32));
}

void AppleSpawner::Update()
{
	spawnTimer -= Time::GetDeltaTime();

	if (spawnTimer <= 0)
	{
		spawnTimer = SPAWN_COOLDOWN;
		CreateApple();
	}
}
