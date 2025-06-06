﻿#include "PixelRenderer.h"

#include "../GameObject.h"

PixelRenderer::PixelRenderer(GameObject* owner, const Color& color, unsigned int pixelSize)
: Component(owner),
  primitive(color, pixelSize)
{
}

void PixelRenderer::Draw(RenderTarget& target)
{
    primitive.Draw(GetOwner()->Position);
}
