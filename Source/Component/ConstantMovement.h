#pragma once

#include "Component.h"
#include "../Math/Vector.h"

class ConstantMovement : public Component
{
public:
	ConstantMovement(GameObject* owner, const Vec2f& movement);
	void Update() override;

private:
	Vec2f movement;
};

