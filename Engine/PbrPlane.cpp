#include "PbrPlane.h"
#include <DirectXCollision.h>
#include "RRay.h"

bool PbrPlane::Intersects(RRay ray, float& dist)
{
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR v3 = XMLoadFloat3(&V3);
	XMVECTOR v4 = XMLoadFloat3(&V4);
	if (DirectX::TriangleTests::Intersects(ray.GetOrigin(), ray.GetDir(), v1, v2, v3, dist)
		|| DirectX::TriangleTests::Intersects(ray.GetOrigin(), ray.GetDir(), v3, v4, v1, dist))
	{
		return true;
	}
	return false;
}

DirectX::XMVECTOR PbrPlane::GetNormal()
{
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR v3 = XMLoadFloat3(&V3);
	return XMVector3Cross((v1 - v2), (v3 - v2));
}
