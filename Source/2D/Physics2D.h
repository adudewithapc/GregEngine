#pragma once
#include <vector>
#include <memory>
#include "../Component/2D/BoxCollider2D.h"

struct CollisionPair
{
	CollisionPair() = delete;
	CollisionPair(BoxCollider2D* col1, BoxCollider2D* col2);
	BoxCollider2D *Collider1, *Collider2;

	bool operator ==(const CollisionPair& other) const;
};

class Physics2D
{
public:
	Physics2D() = delete;
	Physics2D(const Physics2D&) = delete;
	Physics2D(Physics2D&&) = delete;
	Physics2D& operator =(const Physics2D&) = delete;

	static void AddCollider(BoxCollider2D* collider);
	static void Tick();

private:
	static std::vector<BoxCollider2D*> colliders;
	static std::vector<CollisionPair> currentCollisions;
};