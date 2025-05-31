#pragma once
#include "../Component.h"
#include "../../Rendering/TrianglePrimitive.h"

namespace greg
{
class TriangleRenderer : public Component
{
public:
    TriangleRenderer(GameObject* owner);

    void Update() override;

private:
    std::shared_ptr<greg::TrianglePrimitive> primitive;
};
}