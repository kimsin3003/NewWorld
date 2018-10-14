#include "RCameraManager.h"
#include "RCamera.h"


void RCameraManager::Initialize(float screenWidth, float screenHeight, float screenNear, float screenDepth)
{
	RCamera* mainCamera = new RCamera();
	mainCamera->Initialize(screenWidth, screenHeight, screenNear, screenDepth);
	m_cameras.emplace_back(mainCamera);
	m_currentCamera = mainCamera;
}

class RCamera* RCameraManager::GetCurrentCamera()
{
	return m_currentCamera;
}
