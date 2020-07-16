#include "CameraMover.h"
#include "Engine/RContext.h"
#include "Engine/RCamera.h"

void CameraMover::Start()
{
}

void CameraMover::Tick(double deltaTime)
{
	float speed = 0.001f;
	if (InputManager->IsKeyDown(0x77) || InputManager->IsKeyDown(VK_UP))
	{
		auto curPos = CameraManager->GetCurrentCamera()->GetPosition();
		CameraManager->GetCurrentCamera()->SetPosition(curPos.x, curPos.y, curPos.z + speed);
	}
	else if (InputManager->IsKeyDown(0x61) || InputManager->IsKeyDown(VK_LEFT))
	{
		auto curPos = CameraManager->GetCurrentCamera()->GetPosition();
		CameraManager->GetCurrentCamera()->SetPosition(curPos.x - speed, curPos.y, curPos.z);
	}
	else if (InputManager->IsKeyDown(0x73) || InputManager->IsKeyDown(VK_DOWN))
	{
		auto curPos = CameraManager->GetCurrentCamera()->GetPosition();
		CameraManager->GetCurrentCamera()->SetPosition(curPos.x, curPos.y, curPos.z - speed);
	}
	else if (InputManager->IsKeyDown(0x64) || InputManager->IsKeyDown(VK_RIGHT))
	{
		auto curPos = CameraManager->GetCurrentCamera()->GetPosition();
		CameraManager->GetCurrentCamera()->SetPosition(curPos.x + speed, curPos.y, curPos.z);
	}

	if (InputManager->HasNewMouseMove())
	{
		float speed = 0.1f;
		RInputManager::MouseMove move = InputManager->GetNewMouseMove();
		auto curRot = CameraManager->GetCurrentCamera()->GetRotation();
		CameraManager->GetCurrentCamera()->SetRotation(curRot.x + speed * move.y, curRot.y + speed * move.x, curRot.z);
	}
}
