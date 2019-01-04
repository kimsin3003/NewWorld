#include "RCameraManager.h"
RCameraManager* CameraManager = new RCameraManager();

RCameraManager::~RCameraManager()
{
	for (auto camera : m_cameras)
	{
		delete camera;
	}
	m_cameras.clear();
}

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
