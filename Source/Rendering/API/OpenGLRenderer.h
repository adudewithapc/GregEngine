#pragma once
#include <set>

#include "../Renderer.h"

struct Color;

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer(const HDC& hdc);

    ~OpenGLRenderer() override;

    void Render(const Color& clearColor) override;
    
private:
    void CreatePixelFormat();

protected:
    void SetupPrimitive(std::shared_ptr<Primitive> primitive) override;

private:
    std::vector<std::weak_ptr<Primitive>> primitivesToClear;
    HGLRC glContext;
};
