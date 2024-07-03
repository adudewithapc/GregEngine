#pragma once

class Time
{
public:
	static float GetDeltaTime();
	static float GetTimeSinceStartup();

	void Tick(float deltaTime);

private:
	static float deltaTime;
	inline static float timeSinceStartup = 0;
};