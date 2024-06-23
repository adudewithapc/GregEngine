#include "BoxCollider2D.h"
#include <iostream>
#include "../../GameObject.h"
#include "../../2D/Physics2D.h"

BoxCollider2D::BoxCollider2D(GameObject* gameObject, const Vec2f& size) : Component(gameObject), Size(size)
{
	Physics2D::AddCollider(this);
}

BoxCollider2D::~BoxCollider2D()
{
	Physics2D::RemoveCollider(this);
}

void BoxCollider2D::Draw()
{
}

void BoxCollider2D::StartCollision(BoxCollider2D* other)
{
	GetOwner()->CollisionEntered(other->GetOwner());
}

void BoxCollider2D::EndCollision(BoxCollider2D* other)
{
	GetOwner()->CollisionExited(other->GetOwner());
}

bool BoxCollider2D::Intersects(BoxCollider2D* other)
{
	Vec2f& position = GetOwner()->Position;
	
	Vec2f& otherPosition = other->GetOwner()->Position;
	Vec2f& otherSize = other->Size;

	float left = position.x - Size.x;
	float right = position.x + Size.x;
	float up = position.y + Size.y;
	float down = position.y - Size.y;

	float otherLeft = otherPosition.x - otherSize.x;
	float otherRight = otherPosition.x + otherSize.x;
	float otherUp = otherPosition.y + otherSize.y;
	float otherDown = otherPosition.y - otherSize.y;

	if(left < otherRight &&
		right > otherLeft &&
		up > otherDown &&
		down < otherUp)
	{
		return true;
	}

	return false;
}
