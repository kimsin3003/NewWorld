#pragma once
#include <vector>
class CameraManager
{
public:
	void Initialize(float screenWidth, float screenHeight, float  screenNear, float screenDepth);
	class Camera* GetCurrentCamera();

	std::vector<class Camera*> m_cameras;
	class Camera* m_currentCamera;
};

