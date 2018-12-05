#include "PathTracer.h"
#include "Intersection.h"
#include "RLight.h"

#define MAX_DEPTH 5

RVector3 PathTracer::GetPixelColor(RRay ray, const std::vector<class RGameObject*>& gameObjects, int depth)
{
	RVector3 ambientColor = { 0, 0, 0 };
	if (depth > MAX_DEPTH)
		return ambientColor;

	HitData hitData;
	if (Intersection::GetHitData(&hitData, ray, gameObjects))
	{
		XMVECTOR reflectionDir = XMVector3Normalize(ray.GetDir() + 2 * -ray.GetDir() * hitData.hitPlaneNormal * hitData.hitPlaneNormal);
		RVector3 averageColor(0,0,0);
		XMVECTOR randomDirVector;
		XMFLOAT3 ramdomDir = XMFLOAT3(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2);
		randomDirVector = XMVector3Normalize(XMLoadFloat3(&ramdomDir));
		RVector3 reflectedRayColor = GetPixelColor(RRay(hitData.hitPoint, reflectionDir), gameObjects, depth + 1)* 0.2;
		RVector3 randomRayColor = GetPixelColor(RRay(hitData.hitPoint, randomDirVector), gameObjects, depth + 1)* 0.4;
		return hitData.hitObject->pbrColor + randomRayColor;
	}
	else
	{
		XMVECTOR lightDir = XMVector3Normalize(XMVectorSet(-1.0f, -1.0f, 1.0f, 0));
		XMFLOAT3 dotVector;
		XMStoreFloat3(&dotVector, XMVector3Dot(ray.GetDir(), -lightDir));
		float dotProduct = dotVector.x;
		if (dotProduct >= 0)
		{
			return RVector3(255, 255, 255) * dotProduct;
		}
		return ambientColor;
	}
}
