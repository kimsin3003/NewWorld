#pragma once
#include <directxmath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetRotation() { return m_rotation; }

	void Tick();
	const XMMATRIX& GetViewMatrix();

private:
	void CreateViewMatrix();
	void CreateProjectionMatrix();
	XMFLOAT3 m_position;
	XMFLOAT3 m_lookAt;
	XMFLOAT3 m_up;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
};

