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

bool Intersection::GetHitData(HitData* hitData, RRay ray, std::vector<class PbrObject*> gameObjects)
{
	float maxDist = 10000.0;
	for (auto gameObject : gameObjects)
	{
		ICollider* collider = nullptr;
		if (gameObject->PbrColliderType == PBRColliderType::SPHERE)
		{
			PbrSphere* pbrSphere = static_cast<PbrSphere*>(gameObject);
			collider = static_cast<ICollider*>(pbrSphere);
		}
		else if (gameObject->PbrColliderType == PBRColliderType::PLANE)
		{
			PbrPlane* pbrPlane = static_cast<PbrPlane*>(gameObject);
			collider = static_cast<ICollider*>(pbrPlane);
		}
		else
			continue;

		float dist = 0.0f;
		if (collider != nullptr && collider->Intersects(ray, dist))
		{
			if (dist < maxDist && dist > 0.01f)
			{
				XMVECTOR hitPoint = ray.GetOrigin() + ray.GetDir() * dist;
				XMVECTOR normal = collider->GetNormal(hitPoint);
				XMFLOAT3 dotResult;
				XMStoreFloat3(&dotResult, XMVector3Dot(ray.GetDir(), normal));

				if (dotResult.x > 0) //물체 내부에서 밖으로
					hitData->refractionRatio = gameObject->Material.RefractionRate;
				else
					hitData->refractionRatio = 1 / gameObject->Material.RefractionRate;

				hitData->hitObject = gameObject;
				hitData->hitPoint = hitPoint;
				hitData->hitPlaneNormal = normal;
				hitData->dist = dist;
				maxDist = dist;
			}
		}
	}

	if (hitData->hitObject != nullptr)
		return true;
	return false;
}