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
	for (auto gameObject : gameObjects)
	{
		ICollider* collider = nullptr;
		if (gameObject->pbrFigure == PBRFIGURE::SPHERE)
		{
			PbrSphere* pbrSphere = static_cast<PbrSphere*>(gameObject);
			collider = static_cast<ICollider*>(pbrSphere);
		}
		else if(gameObject->pbrFigure == PBRFIGURE::PLANE)
		{
			PbrPlane* pbrPlane = static_cast<PbrPlane*>(gameObject);
			collider = static_cast<ICollider*>(pbrPlane);
		}

		float dist = 0.0f;
		if (collider != nullptr && collider->Intersects(ray, dist))
		{
			if (dist < minDist && dist > 0.0f)
			{
				XMVECTOR hitPoint = ray.GetOrigin() + ray.GetDir() * dist;
				hitData->hitObject = gameObject;
				hitData->hitPoint = hitPoint;
				hitData->hitPlaneNormal = collider->GetNormal(hitPoint);
				minDist = dist;
			}
		}
	}

	if (hitData->hitObject != nullptr)
		return true;
	return false;
}