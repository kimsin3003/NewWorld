#pragma once
#include <vector>
#include <queue>
#include <Keyboard.h>
#include <Mouse.h>
#include <windows.h>

using namespace DirectX;

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
	bool IsKeyDown(Keyboard::Keys key);
	DirectX::Mouse::State GetMouseState();

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
};
