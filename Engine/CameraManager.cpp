#include "CameraManager.h"
#include "Camera.h"


void CameraManager::Initialize(float screenWidth, float screenHeight, float screenNear, float screenDepth)
{
	Camera* mainCamera = new Camera();
	mainCamera->Initialize(screenWidth, screenHeight, screenNear, screenDepth);
	m_cameras.emplace_back(mainCamera);
	m_currentCamera = mainCamera;
}

class Camera* CameraManager::GetCurrentCamera()
{
	return m_currentCamera;
}
