
#include "GregorianEngine.h"
#include <chrono>
#include "GregTime.h"
#include "Debugging/Log.h"
#include "Input/Keyboard.h"
#include "Math/Color.h"
#include "Rendering/Window.h"

void GregorianEngine::Run(const std::string& applicationName)
{
	greg::log::SetLevel(greg::log::Level::INFO);
	this->applicationName = applicationName;
	using FloatingSeconds = std::chrono::duration<float>;

	window.SetTitle(applicationName);

	Time time;
	std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();

	running = true;
	float timeUntilNextFrame = maxFrameTime;
	
	size_t frameCount = 0;
	double timeUntilNextFramePrint = 1.0;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		timeUntilNextFrame -= deltaTime;
		
		time.Tick(deltaTime);
		
		if(!window.ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
			running = false;
		
		if(logFrameTime)
		{
			frameCount++;
			timeUntilNextFramePrint -= deltaTime;

			if(timeUntilNextFramePrint <= 0)
			{
				timeUntilNextFramePrint = 1;
				greg::log::Info("Frame count", std::format("{} fps", frameCount));
				frameCount = 0;
			}
		}

		if(timeUntilNextFrame > 0)
			continue;
		
		timeUntilNextFrame = maxFrameTime;
		if(currentLevel)
			currentLevel->Update();
		
		window.GetRenderer().Render(Color(0, 0, 0, 1));

		previousTime = currentTime;
	}
}

void GregorianEngine::Shutdown()
{
	running = false;
}

void GregorianEngine::SetLogFrameCounter(bool logFrameTime)
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

const std::string& GregorianEngine::GetApplicationName() const
{
	return applicationName;
}

Window& GregorianEngine::GetWindow()
{
	return window;
}

const Window& GregorianEngine::GetWindow() const
{
	return window;
}

GregorianEngine& GregorianEngine::Get()
{
	static GregorianEngine engine;
	return engine;
}

const char* GregorianEngine::GetEngineName()
{
	return "Greg Engine";
}
