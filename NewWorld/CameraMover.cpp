#include "CameraMover.h"
#include "Engine/RContext.h"
#include "Engine/RCamera.h"
#include <Mouse.h>

void CameraMover::Start()
{
}

void CameraMover::Tick(double deltaTime)
{
	float speed = 100 * deltaTime;
	RCamera* camera = CameraManager->GetCurrentCamera();
	auto curPos = camera->GetPosition();
	auto curDir = camera->GetDirection();
	auto curRight = camera->GetRight();
	if (InputManager->IsKeyDown(Keyboard::W))
	{
		RVector3 moveDist = curDir * speed;
		camera->SetPosition(curPos + moveDist);
	}
	else if (InputManager->IsKeyDown(Keyboard::A))
	{
		RVector3 moveDist = curRight * speed;
		camera->SetPosition(curPos - moveDist);
	}
	else if (InputManager->IsKeyDown(Keyboard::S))
	{
		RVector3 moveDist = curDir * speed;
		camera->SetPosition(curPos.x, curPos.y, curPos.z - speed);
	}
	else if (InputManager->IsKeyDown(Keyboard::D))
	{
		RVector3 moveDist = curRight * speed;
		camera->SetPosition(curPos + moveDist);
	}
	else if (InputManager->IsKeyDown(Keyboard::E))
	{
		camera->SetPosition(curPos.x, curPos.y + speed, curPos.z);
	}
	else if (InputManager->IsKeyDown(Keyboard::Q))
	{
		camera->SetPosition(curPos.x, curPos.y - speed, curPos.z);
	}
	auto state = InputManager->GetMouseState();

	if (state.positionMode == Mouse::MODE_RELATIVE)
	{
		float speed = 0.1f;
		auto curRot = camera->GetRotation();
		camera->SetRotation(curRot.x + speed * state.y, curRot.y + speed * state.x, curRot.z);
	}
}
