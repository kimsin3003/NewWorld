#include "PbrSphere.h"
#include <DirectXCollision.h>

bool PbrSphere::Intersects(RRay ray, float& dist)
{
	BoundingSphere sphere;
	sphere.Center = GetPos();
	sphere.Radius = R;
	if (sphere.Intersects(ray.GetOrigin(), ray.GetDir(), dist))
	{
		return true;
	}
	return false;
}

DirectX::XMVECTOR PbrSphere::GetNormal(XMVECTOR hitPoint)
{
	return XMVector3Normalize(hitPoint - XMLoadFloat3(&GetPos()));
}
