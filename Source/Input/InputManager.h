#pragma once

#include <Windows.h>

class Keyboard;
class Mouse;

class InputManager
{
public:
	InputManager();
	InputManager(const InputManager&) = delete;
	bool operator =(const InputManager&) = delete;
	~InputManager();
	
	bool RegisterInputDevices();

	void ReceiveKeyboardInput(const RAWKEYBOARD& keyboardInput);
	void MoveMouse(const WPARAM& virtualKeys, const int x, const int y);

private:
	Keyboard* keyboard;
	Mouse* mouse;
};