#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Level.h"
#include "Window.h"
#include "2D/Camera2D.h"

class GregorianEngine
{
public:
	GregorianEngine();

	GregorianEngine(const GregorianEngine&) = delete;
	GregorianEngine& operator =(const GregorianEngine&) = delete;

	GregorianEngine(GregorianEngine&&) = delete;
	GregorianEngine operator =(GregorianEngine&&) = delete;

	int Start(const std::string& windowTitle);
	void Shutdown();

	std::unique_ptr<Level>& MakeLevel();

	Level* GetCurrentLevel() const;

public:
	static GregorianEngine& Get();

private:
	bool running = false;
	std::unique_ptr<Level> currentLevel;

	Window window;
};