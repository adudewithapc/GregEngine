#pragma once

#include <Component/Component.h>

class PlayerMovement : public Component
{
public:
	PlayerMovement(GameObject* owner);
	virtual void Update() override;
	
private:
	void Move();
	static constexpr float SPEED = 500.0f;
};