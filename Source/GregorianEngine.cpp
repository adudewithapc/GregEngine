
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

	window->SetTitle(windowTitle);
	window->ResizeViewport(Window::WindowWidth, Window::WindowHeight);

	Time time;
	std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();

	running = true;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		time.Tick(deltaTime);

		window->GetRenderer().Clear(Vec4f(0, 0, 0, 1));

		currentLevel->Update();
		currentLevel->Draw(*window);

		window->GetRenderer().SwapBuffers();

		if(!window->ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
			running = false;

		previousTime = currentTime;
	}
}

void GregorianEngine::Shutdown()
{
	running = false;
}

std::unique_ptr<Level>& GregorianEngine::MakeLevel()
{
	currentLevel = std::make_unique<Level>();
	return currentLevel;
}

Level* GregorianEngine::GetCurrentLevel() const
{
	return currentLevel.get();
}

RenderTarget& GregorianEngine::GetRenderTarget() const
{
	return *window;
}

GregorianEngine& GregorianEngine::Get()
{
	static GregorianEngine engine;
	return engine;
}
