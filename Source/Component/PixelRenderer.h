#pragma once
#include "Component.h"
#include "../Rendering/PixelPrimitive.h"

struct Color;

class PixelRenderer : public Component
{
public:
    PixelRenderer(GameObject* owner, const Color& color, unsigned int pixelSize = 1);

    PixelRenderer(const PixelRenderer& other) = default;
    PixelRenderer& operator=(const PixelRenderer& other) = default;

    PixelRenderer(PixelRenderer&& other) = default;
    PixelRenderer& operator=(PixelRenderer&& other) = default;
    
private:
    PixelPrimitive primitive;
};
