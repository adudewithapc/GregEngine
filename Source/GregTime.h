//Named GregTime due to conflict with c-standard library "time.h"
#pragma once

class Time
{
public:
	static float GetDeltaTime();
	static float GetTimeSinceStartup();

	void Tick(float deltaTime);

private:
	inline static float deltaTime = 0;
	inline static float timeSinceStartup = 0;
};