#pragma once
#include "Pixel.h"
#include "PixelManager.h"

class Sand : public Pixel
{
public:
    Sand(GameObject* owner, PixelManager& pixelManager, const Vec2i& point);
    
    void Update() override;
};
