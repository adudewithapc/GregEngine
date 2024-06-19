#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Level.h"

class GregorianEngine
{
public:
	int Start();
	void Shutdown();

	std::unique_ptr<Level>& MakeLevel();

private:
	bool running = false;
	std::unique_ptr<Level> currentLevel;
};