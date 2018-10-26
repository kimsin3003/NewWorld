#include "Room.h"

void Room::Start()
{
	SetPosition(0, 0, 10);
}

void Room::Tick(float deltaTime)
{
	static float rotX = 0;
	rotX += 100 * deltaTime;
	static float rotY = 0;
	rotY += 100 * deltaTime;
	SetRotation(90, rotY, 0);
}
