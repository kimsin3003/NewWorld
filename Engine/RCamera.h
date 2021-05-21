#pragma once
#include <directxmath.h>
#include "RVector.h"

using namespace DirectX;

class RCamera
{
public:
	RCamera();
	~RCamera();
	void Initialize(float screenWidth, float screenHeight, float  screenNear, float screenDepth);

	void SetPosition(float x, float y, float z);
	void SetPosition(RVector3 pos);
	void SetRotation(float x, float y, float z);

	RVector3 GetDirection() { return RVector3(m_dir); }
	RVector3 GetRight() { return RVector3(m_right); }
	RVector3 GetPosition() { return RVector3(m_position); }
	RVector3 GetRotation() { return RVector3(m_rotation); }

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
	XMFLOAT3 m_dir;
	XMFLOAT3 m_right;
};

