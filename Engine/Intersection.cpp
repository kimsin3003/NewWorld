#include "Intersection.h"
#include "RRay.h"
#include "RGameObject.h"
#include "RMesh.h"
#include "PbrSphere.h"
#include "PbrPlane.h"
#include <cmath>

bool isfinite(float arg)
{
	return arg == arg &&
		arg != std::numeric_limits<float>::infinity() &&
		arg != -std::numeric_limits<float>::infinity();
}

bool Intersection::GetHitData(HitData* hitData, RRay ray, std::vector<class RGameObject*> gameObjects)
{
	float minDist = 10000.0;
	float dist = 0.0f;
	for (auto gameObject : gameObjects)
	{
		if (gameObject->pbrFigure == PBRFIGURE::SPHERE)
		{
			PbrSphere* pbrSphere = (PbrSphere*)gameObject;
			if (pbrSphere->Intersects(ray, dist))
			{
				if (!isfinite(dist) && !std::isnan(dist) && dist < minDist && dist > 0.0);
				{
					XMVECTOR hitPoint = ray.GetOrigin() + ray.GetDir() * dist;
					hitData->hitObject = gameObject;
					hitData->hitPoint = hitPoint;
					hitData->hitPlaneNormal = XMVector3Normalize(hitPoint - XMLoadFloat3(&gameObject->GetPos()));
					minDist = dist;
				}
			}
		}
		else if(gameObject->pbrFigure == PBRFIGURE::PLANE)
		{
			PbrPlane* pbrPlane = (PbrPlane*)gameObject;
			if (pbrPlane->Intersects(ray, dist))
			{
				if (!isnan(dist) && !isinf(dist) && dist < minDist)
				{
					hitData->hitObject = gameObject;
					hitData->hitPoint = ray.GetOrigin() + ray.GetDir() * dist;
					hitData->hitPlaneNormal = -XMVector3Normalize(pbrPlane->GetNormal());
					minDist = dist;
				}
			}
		}
	}

	if (hitData->hitObject != nullptr)
		return true;
	return false;
}