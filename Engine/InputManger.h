#pragma once
#include <vector>

class InputManager
{
public:
	static void Initialize();
	static void Reset();
	static void KeyDown(unsigned int input);
	static void KeyUp(unsigned int input);
	static bool IsKeyDown(unsigned int key);

private:
	static bool* m_keys;
	static std::vector<int> m_pressedKeys;
};
