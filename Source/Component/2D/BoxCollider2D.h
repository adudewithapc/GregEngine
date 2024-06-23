#pragma once

#include "../../Math/Vector.h"
#include "../../GameObject.h"
#include "../Component.h"

class BoxCollider2D : public Component
{
public:
	BoxCollider2D(GameObject* gameObject, const Vec2f& size);
	virtual ~BoxCollider2D() override;

	virtual void Draw() override;
	void StartCollision(BoxCollider2D* other);
	void EndCollision(BoxCollider2D* other);
	bool Intersects(BoxCollider2D* other);

	Vec2f Size;
};

