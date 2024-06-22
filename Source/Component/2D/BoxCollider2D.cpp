#include "BoxCollider2D.h"
#include <iostream>
#include "../../GameObject.h"
#include "../../2D/Physics2D.h"

BoxCollider2D::BoxCollider2D(GameObject* gameObject, const Vec2f& size) : Component(gameObject), Size(size)
{
	Physics2D::AddCollider(this);
}

void BoxCollider2D::Draw()
{
}

void BoxCollider2D::CollisionEntered(BoxCollider2D* other)
{
	std::cout << "Collision entered\n";
}

void BoxCollider2D::CollisionExited(BoxCollider2D* other)
{
	std::cout << "Collision exited\n";
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
