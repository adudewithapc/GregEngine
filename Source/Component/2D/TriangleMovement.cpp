#include "TriangleMovement.h"

#include "../../GameObject.h"
#include "../../GregTime.h"
#include "../../Input/Keyboard.h"

TriangleMovement::TriangleMovement(GameObject* owner)
: Component(owner) {}

void TriangleMovement::Update()
{
    GetOwner()->Position += Vec2f(Keyboard::IsKeyDown(Key::D) - Keyboard::IsKeyDown(Key::A), Keyboard::IsKeyDown(Key::S) - Keyboard::IsKeyDown(Key::W)) * SPEED * Time::GetDeltaTime();
}
