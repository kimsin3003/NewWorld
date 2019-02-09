#include "Intersection.h"
#include "RRay.h"
#include "RGameObject.h"
#include "RMesh.h"
#include "PbrSphere.h"
#include "PbrPlane.h"
#include <DirectXCollision.h>

bool Intersection::GetHitData(HitData* hitData, RRay ray, std::vector<class RGameObject*> gameObjects)
{
	float minT = 10000000.0;
	for (auto gameObject : gameObjects)
	{
		if (gameObject->pbrFigure == PBRFIGURE::SPHERE)
		{
			PbrSphere* pbrSphere = (PbrSphere*)gameObject;
			BoundingSphere sphere;
			sphere.Center = pbrSphere->GetPos();
			sphere.Radius = pbrSphere->R;
			float t = 0.0f;
			if (sphere.Intersects(ray.GetOrigin(), ray.GetDir(), t))
			{
				if (t < minT);
				{
					XMVECTOR hitPoint = ray.GetOrigin() + ray.GetDir() * t;
					minT = t;
					hitData->hitObject = gameObject;
					hitData->hitPoint = hitPoint;
					hitData->hitPlaneNormal = XMVector3Normalize(hitData->hitPoint - XMLoadFloat3(&gameObject->GetPos()));
				}
			}
		}
		else if(gameObject->pbrFigure == PBRFIGURE::PLANE)
		{
			PbrPlane* pbrPlane = (PbrPlane*)gameObject;
			float t = 0.0f;

			XMVECTOR v1 = XMLoadFloat3(&pbrPlane->V1);
			XMVECTOR v2 = XMLoadFloat3(&pbrPlane->V2);
			XMVECTOR v3 = XMLoadFloat3(&pbrPlane->V3);
			XMVECTOR v4 = XMLoadFloat3(&pbrPlane->V4);
			if (DirectX::TriangleTests::Intersects(ray.GetOrigin(), ray.GetDir(), v1, v2, v3, t) 
				|| DirectX::TriangleTests::Intersects(ray.GetOrigin(), ray.GetDir(), v3, v4, v1, t))
			{
				if (t < minT)
				{
					minT = t;
					hitData->hitObject = gameObject;
					hitData->hitPoint = ray.GetOrigin() + ray.GetDir() * t;
					hitData->hitPlaneNormal = -XMVector3Normalize(XMVector3Cross((v1 - v2), (v3 - v2)));
				}
			}
		}
	}

	if (hitData->hitObject != nullptr)
		return true;
	return false;
}