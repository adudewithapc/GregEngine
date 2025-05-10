#pragma once

#include "Level.h"
#include "Rendering/Window.h"
#include "2D/Camera2D.h"

class GregorianEngine
{
public:
	GregorianEngine(const GregorianEngine&) = delete;
	GregorianEngine& operator =(const GregorianEngine&) = delete;

	GregorianEngine(GregorianEngine&&) = delete;
	GregorianEngine operator =(GregorianEngine&&) = delete;

	void Run(const std::string& windowTitle);
	void Shutdown();

	void SetLogFrameTime(bool logFrameTime);

	std::shared_ptr<Level> MakeLevel();
	std::weak_ptr<Level> GetCurrentLevel() const;

	RenderTarget& GetRenderTarget();

public:
	static GregorianEngine& Get();

private:
	GregorianEngine() = default;
	
	bool running = false;
	bool logFrameTime = false;
	std::shared_ptr<Level> currentLevel;

	Window window;
};