#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION

#include "GregorianEngine.h"
#include <chrono>
#include "Window.h"
#include <glad/glad.h>
#include <iostream>
#include "Time.h"
#include "Input/Keyboard.h"
#include "Input/Keyboard.h"

int GregorianEngine::Start()
{
	using namespace std::chrono;
	using FloatingSeconds = std::chrono::duration<float>;

	Window window;

	if(!gladLoadGL())
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	window.ResizeViewport(Window::WindowWidth, Window::WindowHeight);

	glEnable(GL_DEPTH_TEST);

	Time time;
	std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();

	running = true;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		time.Tick(deltaTime);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentLevel->Update();
		currentLevel->Draw();

		window.SwapBuffers();

		if(!window.ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
			running = false;

		previousTime = currentTime;
	}

	return 0;
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