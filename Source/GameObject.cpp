#include "GameObject.h"
#include "Time.h"
#include "Input/Keyboard.h"

GameObject::GameObject() : Position(Vec2f(0.0f, 0.0f))//, test(0)
{
	
}

void GameObject::Initialize()
{
	components.push_back(std::make_unique<Sprite>(this, "Textures/awesomeface.png"));
}

void GameObject::Update()
{
	float horizontal = Keyboard::IsKeyDown(Key::D) - Keyboard::IsKeyDown(Key::A);
	float vertical = Keyboard::IsKeyDown(Key::W) - Keyboard::IsKeyDown(Key::S);

	Vec2f movement(horizontal, vertical);
	
	Position += movement * Time::GetDeltaTime();

	for(const auto&  component : components)
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
