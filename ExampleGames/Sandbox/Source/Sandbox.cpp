#include <iostream>

#include "GregorianEngine.h"
#include "Component/2D/TriangleMovement.h"
#include "Component/2D/TriangleRenderer.h"
#include "Rendering/TrianglePrimitive.h"

int main(int argc, char* argv[])
{
    GregorianEngine& greg = GregorianEngine::Get();
    greg.SetLogFrameCounter(true);

    std::shared_ptr<Level> level = greg.MakeLevel();
    GameObject* movingTriangle = level->CreateObject();
    movingTriangle->AddComponent<greg::TriangleRenderer>();
    movingTriangle->AddComponent<TriangleMovement>();
    
    greg.Run("Sandbox");
}
