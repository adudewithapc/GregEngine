#include "Time.h"

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

float Time::deltaTime = 0;