#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* owner) : gameObject(owner)
{
}

Component::~Component()
{
}

void Component::Update()
{
}

void Component::Draw()
{
}
