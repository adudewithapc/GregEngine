#pragma once
#include "Component/Component.h"

class PixelManager;

class Pixel : public Component
{
public:
    Pixel(GameObject* owner, PixelManager& pixelManager, const Vec2i& point);

    
protected:
    PixelManager& GetPixelManager() const;

    void SetNewPoint(Vec2i newPoint);
    const Vec2i& GetPoint() const;
    
private:
    Vec2i point;
    PixelManager& pixelManager;
};
