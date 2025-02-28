#include "Mouse.h"
#include "../Math/Vector.h"
#include <iostream>

Mouse::Mouse()
{
	POINT screenPos;
	GetCursorPos(&screenPos);
	RawPosition = Vec2i(screenPos.x, screenPos.y);
}

void Mouse::SetPosition(int x, int y)
{
	LastPosition = Position;
	Position = Vec2i(0, 0) + Vec2i(x, y);
}

void Mouse::MoveRawPosition(const int xMovement, const int yMovement)
{
	RawPosition += Vec2i(xMovement, yMovement);
}

Vec2i& Mouse::GetPosition()
{
	return Position;
}

Vec2i& Mouse::GetLastPosition()
{
	return LastPosition;
}

Vec2i Mouse::GetRawPosition()
{
	return RawPosition;
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