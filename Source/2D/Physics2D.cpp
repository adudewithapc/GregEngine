#include "Physics2D.h"
#include <iostream>

void Physics2D::AddCollider(BoxCollider2D* collider)
{
	colliders.push_back(collider);
}

void Physics2D::RemoveCollider(BoxCollider2D* collider)
{
	auto removedCollider = std::find(colliders.begin(), colliders.end(), collider);

	if (removedCollider == colliders.end())
		return;

	colliders[removedCollider - colliders.begin()] = colliders.back();
	colliders.erase(colliders.end() - 1);
}

void Physics2D::Tick()
{
	if(colliders.size() < 2)
	{
		return;
	}
	for(int i = 0; i < colliders.size() - 1; i++)
	{
		for(int j = i + 1; j < colliders.size() - 1; j++)
		{
			BoxCollider2D* collider1 = colliders[i];
			BoxCollider2D* collider2 = colliders[j];
			CollisionPair pair(collider1, collider2);
			if(collider1->Intersects(collider2))
			{
				if(std::find(currentCollisions.begin(), currentCollisions.end(), pair) == currentCollisions.end())
				{
					collider1->StartCollision(collider2);
					collider2->StartCollision(collider1);

					if(!collider1->GetOwner()->IsDestroyed() && !collider2->GetOwner()->IsDestroyed())
						currentCollisions.push_back(CollisionPair(collider1, collider2));
				}
			}
			else
			{
				std::vector<CollisionPair>::iterator iterator = std::find(currentCollisions.begin(), currentCollisions.end(), pair);
				if(iterator != currentCollisions.end())
				{
					collider1->EndCollision(collider2);
					collider2->EndCollision(collider1);
					currentCollisions.erase(iterator);
				}
			}
		}
	}
}

std::vector<BoxCollider2D*> Physics2D::colliders;
std::vector<CollisionPair> Physics2D::currentCollisions;

CollisionPair::CollisionPair(BoxCollider2D* col1, BoxCollider2D* col2) : Collider1(col1), Collider2(col2)
{
}

bool CollisionPair::operator==(const CollisionPair& other) const
{
	return Collider1 == other.Collider1 && Collider2 == other.Collider2;
}