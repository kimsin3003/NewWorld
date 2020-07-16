#pragma once
#include <vector>
#include <queue>

class RInputManager
{
public:
	struct MouseMove
	{
	public:
		int x;
		int y;
	};
	void Initialize();
	void Reset();
	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);
	bool IsKeyDown(unsigned int key);
	void SetMousePosition(int x, int y);
	inline bool HasNewMouseMove() { return !m_mouseMoves.empty(); }
	MouseMove GetNewMouseMove() { 
		auto move = m_mouseMoves.front();
		m_mouseMoves.pop();
		return move; 
	}

private:
	bool m_keys[256];
	std::vector<int> m_pressedKeys;
	std::queue<MouseMove> m_mouseMoves;
	int m_mouseX;
	int m_mouseY;
};
