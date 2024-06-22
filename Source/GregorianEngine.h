#pragma once

#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION

#define WIN32_LEAN_AND_MEAN
#include "Level.h"
#include "Window.h"
#include "2D/Camera2D.h"

class GregorianEngine
{
public:
	GregorianEngine();

	int Start();
	void Shutdown();

	std::unique_ptr<Level>& MakeLevel();

private:
	bool running = false;
	std::unique_ptr<Level> currentLevel;

	Window window;
};