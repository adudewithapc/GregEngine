
#include "GregorianEngine.h"
#include <chrono>
#include "GregTime.h"
#include "Debugging/Log.h"
#include "Input/Keyboard.h"
#include "Rendering/Window.h"

void GregorianEngine::Run(const std::string& windowTitle)
{
	greg::log::SetLevel(greg::log::Level::INFO);
	using FloatingSeconds = std::chrono::duration<float>;

	window.SetTitle(windowTitle);
	window.ResizeViewport(Window::WindowWidth, Window::WindowHeight);

	Time time;
	std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();

	running = true;
	float timeUntilNextFrame = maxFrameTime;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		timeUntilNextFrame -= deltaTime;
		
		time.Tick(deltaTime);

		if(logFrameTime)
			greg::log::Info("Frame Time", std::format("{}ms", deltaTime * 1000.f));

		if(timeUntilNextFrame > 0)
			continue;
		
		timeUntilNextFrame = maxFrameTime;
		currentLevel->Update();
		
		window.GetRenderer().Clear(Vec4f(0, 0, 0, 1));
		currentLevel->Draw(window);
		window.GetRenderer().SwapBuffers();

		if(!window.ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
			running = false;
		previousTime = currentTime;
	}
}

void GregorianEngine::Shutdown()
{
	running = false;
}

void GregorianEngine::SetLogFrameTime(bool logFrameTime)
{
	this->logFrameTime = logFrameTime;
}

void GregorianEngine::SetFramerateCap(unsigned int framerate)
{
	if(framerate == 0)
		maxFrameTime = 0;
	
	maxFrameTime = 1.0f / static_cast<float>(framerate);
}

std::shared_ptr<Level> GregorianEngine::MakeLevel()
{
	currentLevel = std::make_shared<Level>();
	return currentLevel;
}

std::weak_ptr<Level> GregorianEngine::GetCurrentLevel() const
{
	return std::weak_ptr(currentLevel);
}

RenderTarget& GregorianEngine::GetRenderTarget()
{
	return window;
}

GregorianEngine& GregorianEngine::Get()
{
	static GregorianEngine engine;
	return engine;
}
