#include "RRay.h"
#include "RContext.h"
#include "RCameraManager.h"

RRay::RRay(int screenX, int screenY, int screenWith, int screenHeight)
{
	RCamera* camera = CameraManager->GetCurrentCamera();
	float nearPlaneToHalfScreenHeight = tan(camera->GetFov() / 2);
	float nearPlaneToHalfScreenWidth = nearPlaneToHalfScreenHeight * camera->GetScreenAspect(); //aspect : w/h

	float vx = (2.0f * screenX / screenWith - 1.0f) * nearPlaneToHalfScreenWidth;
	float vy = (-2.0f * screenY / screenHeight + 1.0f) * nearPlaneToHalfScreenHeight;

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX P = camera->GetProjectionMatrix();
	XMVECTOR det = XMMatrixDeterminant(P);

	XMMATRIX V = camera->GetViewMatrix();    
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	m_origin = XMVector3TransformCoord(rayOrigin, invView);
	m_dir = XMVector3Normalize(XMVector3TransformNormal(rayDir, invView));
}

