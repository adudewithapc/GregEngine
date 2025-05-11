#include "PixelManager.h"

#include "GregorianEngine.h"
#include "Component/PixelRenderer.h"
#include "Debugging/ScopedTimer.h"
#include "Math/Color.h"
#include "Rendering/Window.h"

PixelManager::PixelManager(GameObject* owner)
: Component(owner)
{
    for(size_t i = 0; i < width * height; i++)
    {
        pixels[i] = nullptr;
    }
    
    for(int i = 0; i < width; i++)
    {
        AddPixel(Vec2i(i, Window::WindowHeight / 2), Color(0, static_cast<float>(i) / width, (width - static_cast<float>(i)) / width));
    }
}

void PixelManager::Update()
{
    for(int i = 0; i < width * height; i++)
    {
        GameObject* current = pixels[i];
        if(current == nullptr)
            continue;
        
        Vec2f newPos = current->Position;

        if(i == static_cast<int>(newPos.x * newPos.y))
            continue;
        
        pixels[i] = nullptr;
        SetPixelAt(current, Vec2i(newPos.x, newPos.y));
    }
}

GameObject* PixelManager::GetPixelAt(const Vec2i& point) const
{
    return pixels[point.x + point.y * width];
}

void PixelManager::SetPixelAt(GameObject* pixel, const Vec2i& point)
{
    pixels[point.x + point.y * width] = pixel;
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
