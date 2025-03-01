#include "Player.h"
#include <GameObject.h>
#include <Input/Keyboard.h>
#include <GregTime.h>

PlayerMovement::PlayerMovement(GameObject* owner) : Component(owner)
{
}

void PlayerMovement::Update()
{
	Move();
}

void PlayerMovement::Move()
{
	float xMovement = Keyboard::IsKeyDown(Key::D) - Keyboard::IsKeyDown(Key::A);
	GetOwner()->Position.x += xMovement * SPEED * Time::GetDeltaTime();
}
