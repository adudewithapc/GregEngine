#include "Pixel.h"

#include "GameObject.h"
#include "PixelManager.h"
#include "Math/Math.h"

Pixel::Pixel(GameObject* owner, PixelManager& pixelManager, const Vec2i& point)
: Component(owner),
  pixelManager(pixelManager),
  point(point)
{
}

PixelManager& Pixel::GetPixelManager() const
{
    return pixelManager;
}

void Pixel::SetNewPoint(Vec2i newPoint)
{
    newPoint = Vec2i(greg::math::Clamp(newPoint.x, 0, pixelManager.GetWidth() - 1), greg::math::Clamp(newPoint.y, 0, pixelManager.GetHeight() - 1));
    pixelManager.SetPixelAt(nullptr, point);
    pixelManager.SetPixelAt(this, newPoint);
    point = newPoint;
    GetOwner()->Position = Vec2f(newPoint.x, newPoint.y);
}

const Vec2<int>& Pixel::GetPoint() const
{
    return point;
}
