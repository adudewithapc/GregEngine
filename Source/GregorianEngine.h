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

	int Start(const std::string& windowTitle);
	void Shutdown();

	std::unique_ptr<Level>& MakeLevel();
	Level* GetCurrentLevel() const;

	RenderTarget& GetRenderTarget() const;

public:
	static GregorianEngine& Get();

private:
	GregorianEngine() = default;
	
	bool running = false;
	std::unique_ptr<Level> currentLevel;

	std::unique_ptr<Window> window = std::make_unique<Window>();
};