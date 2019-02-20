#include "Engine/RContext.h"
#include "GameManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	MSG msg;
	bool done;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;

	SystemManager systemManager;
	GameManager* gameManger = new GameManager();
	systemManager.Initialize(gameManger);
	RInputManager::Initialize();
// 	while (!done)
// 	{
// 		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
// 		{
// 			TranslateMessage(&msg);
// 			DispatchMessage(&msg);
// 		}
// 
// 		if (msg.message == WM_QUIT)
// 		{
// 			done = true;
// 		}
// 		else
// 		{
// 			systemManager.Tick();
// 		}
// 
// 	}
	return 0;
}
