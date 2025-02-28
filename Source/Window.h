#pragma once

#include <Windows.h>
#include "Math/Vector.h"

class Window
{
public:
	Window();
	
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
	
	~Window();

	void CaptureCursor(bool capture);
	bool ProcessMessages();
	void SwapBuffers();
	void SetTitle(const std::string& windowTitle);

public:
	static Vec2f ViewToPixel(const Vec2f& view);
	static Vec2f PixelToView(const Vec2f& pixel);
	static void ResizeViewport(int width, int height);

	constexpr static int WindowWidth = 800;
	constexpr static int WindowHeight = 600;

	constexpr static int WindowX = 250;
	constexpr static int WindowY = 250;

	inline static int MouseStartingX = WindowX + WindowWidth / 2;
	inline static int MouseStartingY = WindowY + WindowHeight / 2;
private:
	void CreateGLContext();

	static LRESULT CALLBACK WindowProcedure(
		HWND hWnd,		//What window is the message coming from
		UINT message,	//Message type
		WPARAM wParam,	//Extra parameters
		LPARAM lParam	//Extra parameters
	);

	HINSTANCE hInstance;
	HWND windowHandle;
	HGLRC glContext;
	HDC hdc;

	inline static int LastWidth;
	inline static int LastHeight;
};

