#pragma once
#include "../Math/Vector.h"
#include "Windows.h"

class Renderer
{
public:
    Renderer(const HDC& deviceContext);

    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    ~Renderer();

    void Clear(Vec4f color);
    void SwapBuffers();

private:
    HGLRC glContext;
    HDC deviceContext;
};
