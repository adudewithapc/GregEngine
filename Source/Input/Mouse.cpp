#include "Mouse.h"
#include "../Math/Vector.h"

/*Mouse::Mouse()
{
	for(MouseButton& button : buttons)
	{
		pressedButtons[button] = false;
	}
}

Mouse::~Mouse()
{
	pressedButtons.clear();
}*/


void Mouse::SetPosition(const WPARAM& pressedButtons, int x, int y)
{
	Position = Vec2i(0, 0) + Vec2i(x, y);
}

Vec2i Mouse::GetPosition()
{
	return Position;
}

/*bool Mouse::IsButtonDown(const MouseButton& button)
{
	return pressedButtons[button];
}

bool Mouse::IsButtonUp(const MouseButton& button)
{
	return !pressedButtons;
}
*/