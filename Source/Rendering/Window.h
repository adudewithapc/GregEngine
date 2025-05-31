#pragma once

#include <Windows.h>

#include "RenderTarget.h"
#include "../Math/Vector.h"

class Window : public RenderTarget
{
public:
	Window();
	
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
	
	~Window() override;

	Renderer& GetRenderer() override;
	
	void CaptureCursor(bool capture);
	bool ProcessMessages();
	void SetTitle(const std::string& windowTitle);

	const Vec2i& GetFramebufferSize() const;

public:
	static Vec2f ViewToPixel(const Vec2f& view);
	static Vec2f PixelToView(const Vec2f& pixel);

	constexpr static int StartWindowWidth = 800;
	constexpr static int StartWindowHeight = 600;

	constexpr static int StartWindowX = 250;
	constexpr static int StartWindowY = 250;

	inline static int MouseStartingX = StartWindowX + StartWindowWidth / 2;
	inline static int MouseStartingY = StartWindowY + StartWindowHeight / 2;
private:
	void SetFramebufferSize(int width, int height);
	static LRESULT CALLBACK WindowProcedure(
		HWND hWnd,		//What window is the message coming from
		UINT message,	//Message type
		WPARAM wParam,	//Extra parameters
		LPARAM lParam	//Extra parameters
	);

private:
	void CreateRenderer();

	HINSTANCE hInstance;
	HWND windowHandle;
	HGLRC glContext;
	HDC hdc;

	inline static int LastWidth;
	inline static int LastHeight;

	Vec2i framebufferSize;

	std::unique_ptr<Renderer> renderer = nullptr;
};

