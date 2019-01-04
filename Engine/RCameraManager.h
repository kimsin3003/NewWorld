#pragma once
#include <vector>
#include "RCamera.h"
class RCameraManager
{
public:
	~RCameraManager();
	void Initialize(float screenWidth, float screenHeight, float  screenNear, float screenDepth);
	RCamera* GetCurrentCamera();

	std::vector<RCamera*> m_cameras;
	RCamera* m_currentCamera;
};

extern RCameraManager* CameraManager;
