#pragma once
#include <vector>
class CameraManager
{
public:
	void Initialize();
	class Camera* GetCurrentCamera();

	std::vector<class Camera*> m_cameras;
	class Camera* m_currentCamera;
};

