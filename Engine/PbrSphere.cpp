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
