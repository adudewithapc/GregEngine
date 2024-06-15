#pragma once

#include "../../Math/Vector.h"
#include "../../GameObject.h"
#include "../Component.h"

class BoxCollider2D : public Component
{
public:
	BoxCollider2D(GameObject* gameObject, const Vec2f& size);

	virtual void Draw() override;
	void CollisionEntered(BoxCollider2D* other);
	void CollisionExited(BoxCollider2D* other);
	bool Intersects(BoxCollider2D* other);

	Vec2f Size;
};

