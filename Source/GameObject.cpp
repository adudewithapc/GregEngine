#include "GameObject.h"
#include "Time.h"
#include "Input/Keyboard.h"

GameObject::GameObject() : 
	sprite(Sprite("Textures/awesomeface.png"))
{
	
}

void GameObject::Update()
{
	float horizontal = Keyboard::IsKeyDown(Key::D) - Keyboard::IsKeyDown(Key::A);
	float vertical = Keyboard::IsKeyDown(Key::W) - Keyboard::IsKeyDown(Key::S);

	Vec2f movement(horizontal, vertical);

	Position += movement * Time::GetDeltaTime();
}

void GameObject::Draw()
{
	sprite.Render(Position);
}
