#include <GregorianEngine.h>

#include <Component/PixelRenderer.h>

#include "PixelManager.h"

int main()
{
    std::shared_ptr<Level> level = GregorianEngine::Get().MakeLevel();

    GameObject* pixelManager = level->CreateObject();

    pixelManager->AddComponent<PixelManager>();

    GregorianEngine::Get().SetLogFrameTime(true);
    GregorianEngine::Get().SetFramerateCap(144);
    GregorianEngine::Get().Run("Sand Demo");
}
