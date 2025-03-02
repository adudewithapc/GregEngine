#include <GregorianEngine.h>
#include "PlayerMovement.h"
#include <Component/2D/Sprite.h>
#include <Math/Matrix/Mat3x3.h>
#include "AppleSpawner.h"
#include <Component/2D/BoxCollider2D.h>
#include "PlayerScore.h"

int main()
{
	std::unique_ptr<Level>& level = GregorianEngine::Get().MakeLevel();

	GameObject* player = level->CreateObject();
	
	player->AddComponent<PlayerMovement>();
	player->AddComponent<Sprite>("Resources/Textures/basket_man.png");
	player->Position = Vec2f(0, -389);

	player->AddComponent<BoxCollider2D>(Vec2f(115, 222));

	player->AddComponent<PlayerScore>();

	GameObject* appleSpawner = level->CreateObject();
	appleSpawner->AddComponent<AppleSpawner>(800, 600);

	return GregorianEngine::Get().Start("Basket Man");
}
