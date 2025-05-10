#include <GregorianEngine.h>

#include <Component/PixelRenderer.h>
#include <Math/Color.h>

int main()
{
    std::shared_ptr<Level> level = GregorianEngine::Get().MakeLevel();

    GameObject* testObject = level->CreateObject();
    testObject->AddComponent<PixelRenderer>(Color(0, 1, 0));

    GregorianEngine::Get().Run("Sand Demo");
}
