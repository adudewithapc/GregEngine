#include "Component.h"
#include "../GameObject.h"

Component::Component(GameObject* owner) : owner(owner)
{
}

void Component::Update()
{
}

void Component::Draw(RenderTarget& target)
{
}

void Component::CollisionEntered(GameObject* other)
{
}

void Component::CollisionExited(GameObject* other)
{
}

GameObject* Component::GetOwner() const
{
    return owner;
}
