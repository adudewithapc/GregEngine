#include "Component.h"
#include "../GameObject.h"

Component::Component(GameObject* owner) : owner(owner)
{
}

void Component::Update()
{
}

void Component::Draw()
{
}

GameObject* Component::GetOwner() const
{
    return owner;
}
