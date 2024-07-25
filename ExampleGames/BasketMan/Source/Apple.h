#pragma once

#include <Component/Component.h>

class Apple : public Component
{
public:
	Apple(GameObject* owner);
	virtual void Update() override;

private:
	static constexpr float GRAVITY = 300;
};

