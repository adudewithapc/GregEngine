#include "GameObject.h"
#include <iostream>
#include "Time.h"

void GameObject::Update()
{
	std::cout << "Hi! The time is " << Time::GetTimeSinceStartup() << "\n";
}

void GameObject::Draw()
{
}
