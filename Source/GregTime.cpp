#include "GregTime.h"

float Time::GetDeltaTime()
{
    return deltaTime;
}

float Time::GetTimeSinceStartup()
{
    return timeSinceStartup;
}

void Time::Tick(float inDeltaTime)
{
    deltaTime = inDeltaTime;
    timeSinceStartup += deltaTime;
}