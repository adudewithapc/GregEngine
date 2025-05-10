#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(GameObject* owner, const Color& color)
: Component(owner),
  primitive(color)
{
}

void PixelRenderer::Draw(RenderTarget& target)
{
    primitive.Draw(Vec2f(300.f, 0));
}
