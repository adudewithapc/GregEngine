#include "InputManager.h"

#include <format>
#include <iostream>
#include <hidusage.h>
#include "Keyboard.h"
#include "Mouse.h"
#include "../Window.h"
#include "../Debugging/Log.h"

//Naming conflict with windows
#undef ERROR

InputManager::InputManager()
{
	keyboard = std::make_unique<Keyboard>();
	mouse = std::make_unique<Mouse>();
}

//Taken from https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
bool InputManager::RegisterInputDevices()
{
	static bool devicesRegistered = false;
	if(devicesRegistered)
	{
		greg::log::Error("Input", "Tried to register input devices when they've already been registered");
		return false;
	}

	RAWINPUTDEVICE inputDevices[4];

	//Keyboard
	inputDevices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	inputDevices[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	inputDevices[0].dwFlags = RIDEV_NOLEGACY;
	inputDevices[0].hwndTarget = 0;

	//Gamepad
	inputDevices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	inputDevices[1].usUsage = HID_USAGE_GENERIC_GAMEPAD;
	inputDevices[1].dwFlags = 0;
	inputDevices[1].hwndTarget = 0;

	//Joystick
	inputDevices[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
	inputDevices[2].usUsage = HID_USAGE_GENERIC_JOYSTICK;
	inputDevices[2].dwFlags = 0;
	inputDevices[2].hwndTarget = 0;

	//Mouse
	inputDevices[3].usUsagePage = HID_USAGE_PAGE_GENERIC;
	inputDevices[3].usUsage = HID_USAGE_GENERIC_MOUSE;
	inputDevices[3].dwFlags = RIDEV_NOLEGACY;
	inputDevices[3].hwndTarget = 0;

	if(!RegisterRawInputDevices(inputDevices, 4, sizeof(inputDevices[0])))
	{
		greg::log::Fatal("InputManager", std::format("Error {} while registering input devices.", GetLastError()));
		return false;
	}

	devicesRegistered = true;

	return true;
}

void InputManager::ReceiveKeyboardInput(const RAWKEYBOARD& keyboardInput)
{
	keyboard->PressKey(keyboardInput);
}

//https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove
//Coordinates are local to canvas
void InputManager::MoveWindowMouse(const int x, const int y)
{
	mouse->SetPosition(x, y);
}

void InputManager::MoveScreenMouse(const int xMovement, const int yMovement)
{
	mouse->MoveRawPosition(xMovement, yMovement);
}
