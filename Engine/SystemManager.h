#pragma once
#include <windows.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class SystemManager
{
public:
	bool Tick();
	void Initialize();

private:
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	class ObjectManager* m_objectManager = nullptr;
	class Renderer* m_renderer = nullptr;
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
