#include "Sand.h"

#include "PixelManager.h"

Sand::Sand(GameObject* owner, PixelManager& pixelManager, const Vec2<int>& point)
:  Pixel(owner, pixelManager, point)
{
    
}

void Sand::Update()
{
    PixelManager& manager = GetPixelManager();

    if(GetPoint().y == 0)
        return;
    
    if(!manager.GetPixelAt(GetPoint() + Vec2i(0, -1)))
    {
        SetNewPoint(GetPoint() + Vec2i(0, -1));
        return;
    }

    if(!manager.GetPixelAt(GetPoint() + Vec2i(-1, -1)))
    {
        SetNewPoint(GetPoint() + Vec2i(-1, -1));
        return;
    }

    if(!manager.GetPixelAt(GetPoint() + Vec2i(1, -1)))
    {
        SetNewPoint(GetPoint() + Vec2i(1, -1));
    }
}
