#pragma once
#include <vector>
class RCameraManager
{
public:
	void Initialize(float screenWidth, float screenHeight, float  screenNear, float screenDepth);
	class RCamera* GetCurrentCamera();

	std::vector<class RCamera*> m_cameras;
	class RCamera* m_currentCamera;
};

