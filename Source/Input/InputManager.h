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
	void MoveWindowMouse(const WPARAM& virtualKeys, const int x, const int y);
	void MoveScreenMouse(const int xMovement, const int yMovement);

private:
	Keyboard* keyboard;
	Mouse* mouse;
};