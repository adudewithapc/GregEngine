#include <iostream>
#include "Window.h"

#include <format>
#include <glad.h>

#include "../Debugging/Log.h"
#include "../Input/InputManager.h"
#include "../Math/Vector.h"

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

	RegisterClass(&wndClass);

	//Define window style (title bar, minimize button, close button)
	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | CS_OWNDC | WS_MAXIMIZEBOX;

	RECT rect;
	rect.left = WindowX;
	rect.top = WindowY;
	rect.right = rect.left + WindowWidth;
	rect.bottom = rect.top + WindowHeight;

	LastWidth = WindowWidth;
	LastHeight = WindowHeight;


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

	input = new InputManager();
	input->RegisterInputDevices();

	ShowWindow(windowHandle, SW_SHOW);
	CaptureCursor(true);
	SetCursorPos(MouseStartingX, MouseStartingY);
	input->MoveWindowMouse(WindowWidth / 2, WindowHeight / 2);

	SetupRendering();
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
		clipRect.left = WindowX;
		clipRect.top = WindowY;
		clipRect.right = clipRect.left + WindowWidth;
		clipRect.bottom = clipRect.top + WindowHeight;
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

Vec2f Window::ViewToPixel(const Vec2f& view)
{
	return Vec2f(view.x * WindowWidth, view.y * WindowHeight);
}

Vec2f Window::PixelToView(const Vec2f& pixel)
{
	return Vec2f(pixel.x / WindowWidth, pixel.y / WindowHeight);
}

void Window::ResizeViewport(int width, int height)
{
	glViewport(0, 0, width, height);
}


void Window::SetupRendering()
{
	//Taken from https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_%28WGL%29
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	hdc = GetDC(windowHandle);
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	renderer = std::make_unique<Renderer>(hdc);
}

LRESULT CALLBACK Window::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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

			Window::ResizeViewport(newWidth, newHeight);
			break;
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
			clipRect.left = WindowX;
			clipRect.top = WindowY;
			clipRect.right = clipRect.left + WindowWidth;
			clipRect.bottom = clipRect.top + WindowHeight;
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