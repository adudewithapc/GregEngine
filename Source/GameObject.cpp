#include "GameObject.h"

void GameObject::Update()
{
	for(const auto& component : components)
	{
		component->Update();
	}
}

void GameObject::Draw()
{
	for(const auto& component : components)
	{
		component->Draw();
	}
}