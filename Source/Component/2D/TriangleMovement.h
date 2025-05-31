#pragma once
#include "../Component.h"

class TriangleMovement : public Component
{
public:
    TriangleMovement(GameObject* owner);

    void Update() override;

private:
    static constexpr float SPEED = 5.f;
};
