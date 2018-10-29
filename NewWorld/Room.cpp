#include "Room.h"
void Room::Start()
{
}

void Room::Tick(double deltaTime)
{
	static float rotX = 0;
	rotX += (10 * deltaTime);
	if (rotX > 360)
		rotX -= 360;

	static double rotY = 0;
	rotY += (10 * deltaTime);
	if (rotY > 360)
		rotY -= 360;
	SetRotation(0, rotY, 0);
}
