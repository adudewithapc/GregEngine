#include "PixelManager.h"

#include "GregorianEngine.h"
#include "Component/PixelRenderer.h"
#include "Debugging/ScopedTimer.h"
#include "Math/Color.h"
#include "Rendering/Window.h"

PixelManager::PixelManager(GameObject* owner)
: Component(owner)
{
    ScopedTimer timer("PixelManagerInitialization");
    for(size_t i = 0; i < Window::WindowWidth * Window::WindowHeight; i++)
    {
        pixels[i] = nullptr;
    }
    
    for(int i = 0; i < Window::WindowWidth; i++)
    {
        AddPixel(Vec2i(i, Window::WindowHeight / 2), Color(0, static_cast<float>(i) / Window::WindowWidth, (Window::WindowWidth - static_cast<float>(i)) / Window::WindowWidth));
    }
}

void PixelManager::Update()
{
    Component::Update();
}

GameObject* PixelManager::GetPixelAt(const Vec2<int>& point) const
{
    return pixels[point.x + point.y * Window::WindowWidth];
}

void PixelManager::SetPixelAt(GameObject* pixel, const Vec2<int>& point)
{
    pixels[point.x + point.y * Window::WindowWidth] = pixel;
}

void PixelManager::AddPixel(const Vec2i& position, const Color& color)
{
    //Don't add pixels at occupied locations
    if(GetPixelAt(position))
        return;
    
    std::shared_ptr<Level> currentLevel = GregorianEngine::Get().GetCurrentLevel().lock();

    if(!currentLevel)
        return;

    GameObject* pixel = currentLevel->CreateObject();

    pixel->AddComponent<PixelRenderer>(color, 1);
    pixel->Position = Vec2f(position.x, position.y);

    SetPixelAt(pixel, position);
}
