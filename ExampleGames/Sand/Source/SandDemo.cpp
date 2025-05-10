#include <GregorianEngine.h>

#include <Component/PixelRenderer.h>
#include <Math/Color.h>

#include "Component/2D/BoxCollider2D.h"

int main()
{
    std::shared_ptr<Level> level = GregorianEngine::Get().MakeLevel();

    for(int i = 0; i < Window::WindowWidth; i++)
    {
        GameObject* pixel = level->CreateObject();

        pixel->AddComponent<PixelRenderer>(Color(0, static_cast<float>(i) / Window::WindowWidth, (Window::WindowWidth - static_cast<float>(i)) / Window::WindowWidth), 1);
        pixel->Position = Vec2f(i, 300);
    }

    GregorianEngine::Get().SetLogFrameTime(true);
    GregorianEngine::Get().Run("Sand Demo");
}
