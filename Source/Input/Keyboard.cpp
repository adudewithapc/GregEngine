#include "Keyboard.h"

Keyboard::Keyboard()
{
	for(Key& key : keys)
	{
		pressedKeys[key] = false;
	}
}

Keyboard::~Keyboard()
{
	pressedKeys.clear();
	keys.clear();
}

/*Keyboard:
	*
	* -----------------------------
	* Use rawInput->data.keyboard.Flags to determine key state:
	* 0 == key pressed
	* 1 == key released
	* -----------------------------
	*
	* -----------------------------
	* Can use various macros to compare VKey with
	* For instance, data.keyboard.VKey == VK_BACK evaluates whether the pressed button was backspace
	* -----------------------------
	*
	* -----------------------------
	* Swedish letters are variations of VK_OEM_#
	* -----------------------------
	*
	*/
void Keyboard::PressKey(const RAWKEYBOARD& keyboard)
{
	Key key = static_cast<Key>(keyboard.VKey);

	//Remove if we are not skipping registering all keys, which we should do
	if(!KeyIsRegistered(key))
	{
		return;
	}

	//Reverse expression, see function comment
	pressedKeys[key] = !keyboard.Flags;
}

bool Keyboard::IsKeyDown(const Key& key)
{
	return pressedKeys[key];
}

bool Keyboard::IsKeyUp(const Key& key)
{
	return !pressedKeys[key];
}


bool Keyboard::KeyIsRegistered(const Key keyID) const
{
	return std::find(keys.begin(), keys.end(), keyID) != keys.end();
}
