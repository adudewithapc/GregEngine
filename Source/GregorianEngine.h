#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Level.h"

class GregorianEngine
{
public:
	int Start();
	void Shutdown();

	Level& MakeLevel();

private:
	bool running = false;
	Level currentLevel;
};