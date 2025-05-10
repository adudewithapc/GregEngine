#pragma once

#include "Component/Component.h"
#include "Rendering/Window.h"

struct Color;

class PixelManager : public Component
{
public:
    PixelManager(GameObject* owner);
    
    void Update() override;

private:
    void AddPixel(const Vec2i& position, const Color& color);

    GameObject* GetPixelAt(const Vec2i& point) const;
    void SetPixelAt(GameObject* pixel, const Vec2i& point);

    const int width = Window::WindowWidth;
    const int height = Window::WindowHeight;
    
    GameObject* pixels[Window::WindowWidth * Window::WindowHeight];
};
