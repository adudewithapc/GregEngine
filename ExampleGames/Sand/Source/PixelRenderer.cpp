#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(GameObject* owner, const Color& color) : Component(owner)
{
}

void PixelRenderer::Draw(RenderTarget& target)
{
    primitive.Draw(Vec2f(0.8f, 0));
}
