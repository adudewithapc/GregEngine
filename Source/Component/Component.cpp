#include "Component.h"
#include "../GameObject.h"

Component::Component(GameObject* owner) : gameObject(owner)
{
}

void Component::Update()
{
}

void Component::Draw()
{
}
