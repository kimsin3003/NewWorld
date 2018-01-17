#pragma once
#include <vector>

class InputManger
{
public:
	void Initialize();
	void Reset();
	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);
	bool IsKeyDown(unsigned int key);

private:
	bool m_keys[256];
	std::vector<int> m_pressedKeys;
};

