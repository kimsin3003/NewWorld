#pragma once
#include <directxmath.h>

using namespace DirectX;

class RCamera
{
public:
	RCamera();
	~RCamera();
	void Initialize(float screenWidth, float screenHeight, float  screenNear, float screenDepth);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetRotation() { return m_rotation; }

	const XMMATRIX& GetViewMatrix() { return m_viewMatrix; }
	const XMMATRIX& GetProjectionMatrix() { return m_projectionMatrix; }

private:
	void CreateViewMatrix();
	void CreateProjectionMatrix(float screenWidth, float screenHeight, float  screenNear, float screenDepth);
	XMFLOAT3 m_position;
	XMFLOAT3 m_lookAt;
	XMFLOAT3 m_up;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
};

