#pragma once

#include <Windows.h>
#include "Input/InputManager.h"

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
	~Window();

	bool ProcessMessages();
	void SwapBuffers();

	static void ResizeViewport(int width, int height);

	constexpr static int WindowWidth = 800;
	constexpr static int WindowHeight = 600;
private:
	void CreateGLContext();

	static LRESULT CALLBACK WindowProcedure(
		HWND hWnd,		//What window is the message coming from
		UINT message,	//Message type
		WPARAM wParam,	//Extra parameters
		LPARAM lParam	//Extra parameters
	);

	HINSTANCE hInstance;
	HWND hWnd;
	HGLRC glContext;
	HDC hdc;

	inline static int LastWidth;
	inline static int LastHeight;
};

