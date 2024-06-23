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

	virtual void CollisionEntered(GameObject* other);
	virtual void CollisionExited(GameObject* other);

	GameObject* GetOwner() const;
private:
	GameObject* owner;
};

