#include <iostream>
#include "Window.h"

#include <format>

#include "../Debugging/Log.h"
#include "../Input/InputManager.h"
#include "../Math/Vector.h"

#if GREG_OPENGL
#include "API/OpenGLRenderer.h"
#include <glad.h>
#elif GREG_VULKAN
#include "API/Vulkan/VulkanRenderer.h"
#endif

static const wchar_t* CLASS_NAME = L"Greg Engine";

static InputManager* input;

Window::Window() :
	hInstance(GetModuleHandle(nullptr))
{
	//Register window class
	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProcedure;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClass(&wndClass);

	//Define window style (title bar, minimize button, close button)
	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_VISIBLE;

	RECT rect;
	rect.left = StartWindowX;
	rect.top = StartWindowY;
	rect.right = rect.left + StartWindowWidth;
	rect.bottom = rect.top + StartWindowHeight;

	LastWidth = StartWindowWidth;
	LastHeight = StartWindowHeight;


	//Make the window dimensions define the actual canvas, as opposed to the decorations (like the title bar)
	AdjustWindowRect(&rect, style, false);

	//Settings for window
	windowHandle = CreateWindowEx(
		0,						//Dialog style (N/A)
		CLASS_NAME,				//Class name
		L"Greg Engine",			//Window title
		style,					//Decorations
		rect.left,				//Left position
		rect.top,				//Top position
		rect.right - rect.left, //Width
		rect.bottom - rect.top, //Height
		nullptr,				//Window parent (N/A)
		nullptr,				//Menu handle (N/A)
		hInstance,				//Instance handle
		nullptr					//Optional parameters (N/A)
	);

	SetPropW(windowHandle, L"GregWindow", this);

	input = new InputManager();
	input->RegisterInputDevices();

	ShowWindow(windowHandle, SW_SHOW);
	CaptureCursor(true);
	SetCursorPos(MouseStartingX, MouseStartingY);
	input->MoveWindowMouse(StartWindowWidth / 2, StartWindowHeight / 2);

	SetFramebufferSize(StartWindowWidth, StartWindowHeight);
	
	hdc = GetDC(windowHandle);

	CreateRenderer();
}

Window::~Window()
{
	CaptureCursor(false);
	UnregisterClass(CLASS_NAME, hInstance);
	delete input;
}

Renderer& Window::GetRenderer()
{
	return *renderer;
}

void Window::CaptureCursor(bool capture)
{
	if(capture)
	{
		SetCapture(windowHandle);
		RECT clipRect;
		clipRect.left = StartWindowX;
		clipRect.top = StartWindowY;
		clipRect.right = clipRect.left + StartWindowWidth;
		clipRect.bottom = clipRect.top + StartWindowHeight;
		ClipCursor(&clipRect);
	}
	else
	{
		ReleaseCapture();
		ClipCursor(nullptr);
	}
	
	ShowCursor(!capture);
}

bool Window::ProcessMessages()
{
	MSG message;

	while(PeekMessage(&message, nullptr, 0u, 0u, PM_REMOVE))
	{
		if(message.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return true;
}

void Window::SetTitle(const std::string& windowTitle)
{
	if(!SetWindowTextA(windowHandle, windowTitle.c_str()))
	{
		greg::log::Fatal("Window", std::format("Error while trying to set window title to \"{}\". Error: {}", windowTitle, GetLastError()));
	}
}

const Vec2i& Window::GetFramebufferSize() const
{
	return framebufferSize;
}

void Window::SetFramebufferSize(int width, int height)
{
#if GREG_OPENGL
	glViewport(0, 0, width, height);
#endif

	framebufferSize = Vec2i(width, height);
}

void Window::Test()
{
	RECT rect = { 0, 0, 1200, 1200 };
	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | CS_OWNDC | WS_MAXIMIZEBOX;
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
	AdjustWindowRectExForDpi(&rect, style, false, exStyle, GetDpiForWindow(windowHandle));

	SetWindowPos(windowHandle, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
}

Vec2f Window::ViewToPixel(const Vec2f& view)
{
	return Vec2f(view.x * StartWindowWidth, view.y * StartWindowHeight);
}

Vec2f Window::PixelToView(const Vec2f& pixel)
{
	return Vec2f(pixel.x / StartWindowWidth, pixel.y / StartWindowHeight);
}

void Window::CreateRenderer()
{
#if GREG_OPENGL
	renderer = std::make_unique<OpenGLRenderer>(hdc);
#elif GREG_VULKAN
	renderer = std::make_unique<greg::vulkan::VulkanRenderer>(hdc, hInstance, windowHandle);
#endif
}

LRESULT CALLBACK Window::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* window = static_cast<Window*>(GetPropW(hWnd, L"GregWindow"));

	if(window == nullptr || window->windowHandle != hWnd)
		return DefWindowProc(hWnd, message, wParam, lParam);
	
	switch(message)
	{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			//Quit application
			PostQuitMessage(0);
			return 0;
		//Window is forced out of focus
		case WM_CANCELMODE:
			return 0;
		case WM_SIZE:
		{
			int newWidth = LOWORD(lParam);
			int newHeight = HIWORD(lParam);

			if(newWidth == LastWidth && newHeight == LastHeight)
			{
				break;
			}

			window->SetFramebufferSize(newWidth, newHeight);
			break;
		}
		case WM_SIZING:
		{
			return 0;
		}
		//Input code taken from https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
		case WM_INPUT:
		{
			//Returns 0 if window is focused, 1 if unfocused
			if(GET_RAWINPUT_CODE_WPARAM(wParam))
			{
				break;
			}

			UINT dataSize = 0;

			//RID_INPUT gives us identifying data (i.e are we on keyboard?), while RID_HEADER just gives raw binary
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));
			LPBYTE rawData = new BYTE[dataSize];
			if(rawData == nullptr)
			{
				return 0;
			}

			if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawData, &dataSize, sizeof(RAWINPUTHEADER)) != dataSize)
			{
				greg::log::Error("Window", "GetRawInputData returns incorrect size!");
			}

			RAWINPUT* rawInput = (RAWINPUT*)rawData;

			//Keyboard input?
			if(rawInput->header.dwType == RIM_TYPEKEYBOARD)
			{
				input->ReceiveKeyboardInput(rawInput->data.keyboard);
			}

			//Mouse input?
			if(rawInput->header.dwType == RIM_TYPEMOUSE)
			{
				int deltaX = rawInput->data.mouse.lLastX;
				int deltaY = rawInput->data.mouse.lLastY;

				input->MoveScreenMouse(deltaX, deltaY);
			}

			delete[] rawData;
			break;
		}
		case WM_MOUSEMOVE:
		{
			//Coordinates are local to canvas
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			input->MoveWindowMouse(x, y);
			break;
		}
		case WM_SETFOCUS:
		{
			SetCapture(hWnd);
			RECT clipRect;
			clipRect.left = StartWindowX;
			clipRect.top = StartWindowY;
			clipRect.right = clipRect.left + StartWindowWidth;
			clipRect.bottom = clipRect.top + StartWindowHeight;
			ClipCursor(&clipRect);
			break;
		}
		case WM_KILLFOCUS:
		{
			ReleaseCapture();
			ClipCursor(nullptr);
			break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}