#pragma once
#include <windows.h>
#include "Logger.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 0.1f;

class SystemManager
{
public:
	~SystemManager() { Logger::Finalize(); }
	void Initialize();
	void Tick();

private:
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	class RRenderer* m_renderer = nullptr;
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
