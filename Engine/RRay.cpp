#include "RRay.h"
#include "RContext.h"
#include "RCameraManager.h"

RRay::RRay(int screenX, int screenY, int screenWith, int screenHeight)
{
	float vx = (+2.0f * screenX / screenWith - 1.0f);
	float vy = (2.0f * screenY / screenHeight - 1.0f);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	RCamera* camara = CameraManager->GetCurrentCamera();
	
	XMMATRIX P = camara->GetProjectionMatrix();;
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(P), P);
	XMMATRIX V = camara->GetViewMatrix();;
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX toWorld = XMMatrixMultiply(invProj, invView);
	XMVECTOR originInWorldCoord = XMVector3TransformCoord(rayOrigin, toWorld);
	XMVECTOR dirInWorldCoord = XMVector3TransformCoord(rayDir, toWorld);

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, originInWorldCoord);
	m_origin = temp;
	XMStoreFloat3(&temp, dirInWorldCoord);
	m_dir = temp;
}

