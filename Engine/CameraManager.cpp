#include "CameraManager.h"
#include "Camera.h"


void CameraManager::Initialize()
{
	Camera* mainCamera = new Camera();
	m_cameras.emplace_back(mainCamera);
	m_currentCamera = mainCamera;
}

class Camera* CameraManager::GetCurrentCamera()
{
	return m_currentCamera;
}
