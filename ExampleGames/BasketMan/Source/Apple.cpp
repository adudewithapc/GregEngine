#include "Apple.h"
#include <GameObject.h>
#include <GregTime.h>
#include <Rendering/Window.h>

Apple::Apple(GameObject* owner) : Component(owner)
{
}

void Apple::Update()
{
	GetOwner()->Position.y -= GRAVITY * Time::GetDeltaTime();
	if (GetOwner()->Position.y < -Window::WindowHeight)
	{
		GetOwner()->Destroy();
	}
}
