#include "PixelManager.h"

#include "GregorianEngine.h"
#include "GregTime.h"
#include "Sand.h"
#include "Component/PixelRenderer.h"
#include "Debugging/ScopedTimer.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Rendering/Window.h"

PixelManager::PixelManager(GameObject* owner)
: Component(owner)
{
    for(size_t i = 0; i < width * height; i++)
    {
        pixels[i] = nullptr;
    }
    
    /*for(int i = 0; i < width; i++)
    {
        AddPixel(Vec2i(i, Window::WindowHeight / 2), Color(0, static_cast<float>(i) / width, (width - static_cast<float>(i)) / width));
    }*/
}

void PixelManager::Update()
{
    spawnTimer += Time::GetDeltaTime();

    if(spawnTimer >= 0.005f)
    {
        AddPixel(Vec2i(400, 300), Color(0.77f, 0.77f, 0.15f));
        spawnTimer = 0;
    }
}

Pixel* PixelManager::GetPixelAt(const Vec2i& point) const
{
    return pixels[greg::math::Clamp(point.x, 0, width - 1) + greg::math::Clamp(point.y, 0, height - 1) * width];
}

void PixelManager::SetPixelAt(Pixel* pixel, const Vec2i& point)
{
    pixels[point.x + point.y * width] = pixel;
}

int PixelManager::GetWidth() const
{
    return width;
}

int PixelManager::GetHeight() const
{
    return height;
}

void PixelManager::AddPixel(const Vec2i& position, const Color& color)
{
    //Don't add pixels at occupied locations
    if(GetPixelAt(position))
        return;
    
    std::shared_ptr<Level> currentLevel = GregorianEngine::Get().GetCurrentLevel().lock();

    if(!currentLevel)
        return;

    GameObject* pixelObject = currentLevel->CreateObject();

    pixelObject->AddComponent<PixelRenderer>(color, 1);
    pixelObject->Position = Vec2f(position.x, position.y);

    Pixel* pixel = pixelObject->AddComponent<Sand>(*this, position);

    SetPixelAt(pixel, position);
}
