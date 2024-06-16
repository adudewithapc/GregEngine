#pragma once

#include <memory>

class GameObject;

class Component
{
public:
	Component(GameObject* owner);
	virtual ~Component() = default;

	virtual void Update();
	virtual void Draw();

	GameObject* gameObject;
};
