#include "RRay.h"
#include "RContext.h"
#include "RCameraManager.h"

RRay::RRay(int screenX, int screenY, int screenWith, int screenHeight)
{
#define _XM_NO_INTRINSICS_
	RCamera* camara = CameraManager->GetCurrentCamera();

	float vx = (2.0f * screenX / screenWith - 1.0f);
	float vy = (-2.0f * screenY / screenHeight + 1.0f);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX V = camara->GetViewMatrix();

	XMVECTOR originInWorldCoord = XMVector3TransformCoord(rayOrigin, V);
	XMVECTOR dirInWorldCoord = XMVector3TransformNormal(rayDir, V);

	if (screenX == screenWith / 2 && screenY == screenHeight / 2)
		printf("asdf");

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, originInWorldCoord);
	m_origin = temp;
	XMStoreFloat3(&temp, XMVector3Normalize(dirInWorldCoord));
	m_dir = temp;
}

