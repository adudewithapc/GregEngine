#include "GregorianEngine.h"
#include <chrono>
#include "Source/Window.h"
#include <glad/glad.h>
#include <iostream>
#include "Source/Time.h"
#include "Source/Input/Keyboard.h"
#include "Source/Input/Keyboard.h"

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

	bool running = true;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		time.Tick(deltaTime);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.SwapBuffers();

		if(!window.ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
			running = false;

		previousTime = currentTime;
	}
}
