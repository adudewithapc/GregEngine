#pragma once
#include "Component/Component.h"
#include "Rendering/PixelPrimitive.h"

struct Color;

class PixelRenderer : public Component
{
public:
    PixelRenderer(GameObject* owner, const Color& color);

    PixelRenderer(const PixelRenderer& other) = default;
    PixelRenderer& operator=(const PixelRenderer& other) = default;

    PixelRenderer(PixelRenderer&& other) = default;
    PixelRenderer& operator=(PixelRenderer&& other) = default;

    void Draw(RenderTarget& target) override;
    
private:
    PixelPrimitive primitive;
};
