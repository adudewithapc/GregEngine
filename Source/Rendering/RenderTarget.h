#pragma once

#include "Renderer.h"

class RenderTarget
{
public:
    virtual ~RenderTarget() = default;
    
    virtual Renderer& GetRenderer() = 0;
};
