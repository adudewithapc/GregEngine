#include "GameObject.h"
#include "Time.h"
#include "Input/Keyboard.h"
#include <iostream>
#include "Component/2D/BoxCollider2D.h"
#include "Component/2D/Sprite.h"

GameObject::GameObject()
{
	components.push_back(std::make_unique<Sprite>(this, "Textures/awesomeface.png"));
	components.push_back(std::make_unique<BoxCollider2D>(this, Vec2f(512, 512)));
}

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