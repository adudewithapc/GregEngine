#include "ConstantMovement.h"
#include "../GameObject.h"
#include "../Time.h"
#include <iostream>

ConstantMovement::ConstantMovement(GameObject* owner, const Vec2f& movement) : Component(owner), movement(movement)
{}

void ConstantMovement::Update()
{
	gameObject->Position += movement * Time::GetDeltaTime();
}
