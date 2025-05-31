#include <iostream>

#include "GregorianEngine.h"
#include "Rendering/TrianglePrimitive.h"

int main(int argc, char* argv[])
{
    GregorianEngine& greg = GregorianEngine::Get();
    greg.SetLogFrameCounter(true);
    greg.GetRenderTarget().GetRenderer().AddPrimitive<greg::TrianglePrimitive>();
    greg.Run("Sandbox");
}
