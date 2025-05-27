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

	void Run(const std::string& applicationName);
	void Shutdown();

	void SetLogFrameCounter(bool logFrameTime);
	void SetFramerateCap(unsigned int framerate);

	std::shared_ptr<Level> MakeLevel();
	std::weak_ptr<Level> GetCurrentLevel() const;

	RenderTarget& GetRenderTarget();
	const std::string& GetApplicationName() const;

public:
	static GregorianEngine& Get();
	static const char* GetEngineName();

private:
	GregorianEngine() = default;
	
	bool running = false;
	bool logFrameTime = true;
	float maxFrameTime = 0;
	std::shared_ptr<Level> currentLevel;
	std::string applicationName = "Greg Engine";

	Window window;
};