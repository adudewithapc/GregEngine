#pragma once

#include <memory>
#include <Windows.h>

class Keyboard;
class Mouse;

class InputManager
{
public:
	InputManager();
	
	InputManager(const InputManager&) = delete;
	bool operator =(const InputManager&) = delete;
	
	bool RegisterInputDevices();

	void ReceiveKeyboardInput(const RAWKEYBOARD& keyboardInput);
	void MoveWindowMouse(const int x, const int y);
	void MoveScreenMouse(const int xMovement, const int yMovement);

private:
	std::unique_ptr<Keyboard> keyboard;
	std::unique_ptr<Mouse> mouse;
};