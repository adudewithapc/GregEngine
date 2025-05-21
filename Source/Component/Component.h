#pragma once
#include "../Rendering/RenderTarget.h"

class GameObject;

class Component
{
public:
	Component(GameObject* owner);
	
	virtual ~Component() = default;

	virtual void Update();

	virtual void CollisionEntered(GameObject* other);
	virtual void CollisionExited(GameObject* other);

	GameObject* GetOwner() const;
private:
	GameObject* owner;
};

