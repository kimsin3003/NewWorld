#pragma once
#include <windows.h>
#include "Logger.h"
#include <chrono>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 0.1f;

class SystemManager
{
public:
	void Initialize(class IGameManager* gameManager);
	void Tick();
	const class Renderer* GetRenderer() { return m_renderer; }

private:
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	std::chrono::time_point<std::chrono::system_clock> m_lastTime;
	class Renderer* m_renderer = nullptr;
	class IGameManager* m_gameManager = nullptr;
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
