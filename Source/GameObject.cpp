#include "GameObject.h"
#include "Time.h"
#include "Input/Keyboard.h"
#include <iostream>
#include "Component/2D/BoxCollider2D.h"
#include "Component/2D/Sprite.h"

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