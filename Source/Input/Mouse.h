#pragma once

#include "../Math/Vector.h"

//This requires a unique event for each button
//Not worth the effort for now...
/*enum MouseButton
{
	Left = MK_LBUTTON,
	Middle = MK_MBUTTON,
	Right = MK_RBUTTON,
	Forward = MK_XBUTTON2,
	Back = MK_XBUTTON1
};*/

class Mouse
{
public:
	Mouse();
	Mouse(const Mouse&) = delete;
	bool operator =(const Mouse&) = delete;

	void SetPosition(const int x, const int y);
	void MoveRawPosition(const int xMovement, const int yMovement);

	static Vec2i& GetPosition();

	//Returns the mouse position from the previous frame
	static Vec2i& GetLastPosition();

	static Vec2i GetRawPosition();
private:
	inline static Vec2i LastPosition;
	inline static Vec2i Position;

	inline static Vec2i RawPosition;
};

