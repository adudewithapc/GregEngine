#pragma once

#include <Windows.h>
#include <map>
#include <vector>

enum Key : int
{
	Esc = VK_ESCAPE,
	Space = VK_SPACE,
	LeftControl = VK_CONTROL,
	A = 0x41,
	D = 0x44,
	S = 0x53,
	W = 0x57,
};

class Keyboard
{
public:
	Keyboard();
	Keyboard(const Keyboard&) = delete;
	bool operator =(const Keyboard&) = delete;
	~Keyboard();

	void PressKey(const RAWKEYBOARD& keyboard);

	static bool IsKeyDown(const Key& key);
	static bool IsKeyUp(const Key& key);

private:
	//I'm too lazy to handle all possible keys
	//Definitely remove this if laziness dissappears
	bool KeyIsRegistered(const int keyID);

	inline static std::map<Key, bool> pressedKeys;
	inline static std::vector<Key> keys = { Esc, Space, LeftControl, A, D, S, W };
};