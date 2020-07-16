#pragma comment(lib,"d3dcompiler.lib")
#include "SystemManager.h"
#include "RRenderer.h"
#include "RObjectManager.h"
#include "RCameraManager.h"
#include "Logger.h"
#include "RInputManager.h"
#include "RContext.h"
#include "IGameManager.h"



void SystemManager::Initialize(IGameManager* gameManager)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	int windowstate;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	LPCWSTR m_applicationName = L"NewWorld";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.

	int screenWidth = 1600, screenHeight = 900;
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
		windowstate = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	}
	else
	{

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		windowstate = WS_OVERLAPPEDWINDOW;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		m_applicationName,
		m_applicationName,
		windowstate,
		posX,
		posY,
		screenWidth,
		screenHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	Logger::Initialize("log.txt");
	Logger::Log("·Î±ë ½ÃÀÛ");

	m_lastTime = std::chrono::system_clock::now();

	ObjectManager = new RObjectManager();
	if(ObjectManager)
		ObjectManager->Initialize();

	CameraManager = new RCameraManager();
	if (CameraManager)
	{
		CameraManager->Initialize(1920, 1080, SCREEN_NEAR, SCREEN_DEPTH);
	}

	InputManager = new RInputManager();
	if (InputManager)
	{
		InputManager->Initialize();
	}

	m_renderer = new RRenderer();
	if(m_renderer)
		m_renderer->Initialize(m_hwnd, screenWidth, screenHeight);

	m_gameManager = gameManager;
	if (m_gameManager)
		m_gameManager->Initialize();

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			InputManager->KeyDown((unsigned int)wparam);
		}
		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			InputManager->KeyUp((unsigned int)wparam);
		}
		case WM_CHAR:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			InputManager->KeyDown((unsigned int)wparam);
		}
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			InputManager->SetMousePosition(x, y);
		}
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void SystemManager::Tick()
{
	auto now = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = now - m_lastTime;
	m_lastTime = now;
	if (m_gameManager)
		m_gameManager->Tick(diff.count());
	if(ObjectManager)
		ObjectManager->Tick(diff.count());
	if(m_renderer)
		m_renderer->Tick(CameraManager, ObjectManager, diff.count());
}

