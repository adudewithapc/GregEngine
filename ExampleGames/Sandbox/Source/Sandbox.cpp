#include <iostream>

#include "GregorianEngine.h"

int main(int argc, char* argv[])
{
    GregorianEngine& greg = GregorianEngine::Get();
    greg.SetLogFrameCounter(true);
    greg.Run("Sandbox");
}
