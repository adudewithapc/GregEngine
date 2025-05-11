#pragma once

#include "Component/Component.h"
#include "Rendering/Window.h"

class Pixel;
struct Color;

class PixelManager : public Component
{
public:
    PixelManager(GameObject* owner);

    void Update() override;

    Pixel* GetPixelAt(const Vec2i& point) const;
    void SetPixelAt(Pixel* pixel, const Vec2i& point);

    int GetWidth() const;
    int GetHeight() const;
private:
    void AddPixel(const Vec2i& position, const Color& color);

    const int width = Window::WindowWidth;
    const int height = Window::WindowHeight;

    float spawnTimer = 0;
    
    Pixel* pixels[Window::WindowWidth * Window::WindowHeight];
};
