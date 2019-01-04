#include "PathTracer.h"
#include "Intersection.h"

#define MAX_DEPTH 3
 XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal);
 XMVECTOR GetRefelectedDir(XMVECTOR rayDir, XMVECTOR planeNormal);
RVector3 PathTracer::GetPixelColor(RRay ray, const std::vector<class RGameObject*>& gameObjects, int depth)
{
	RVector3 ambientColor = { 0, 0, 0 };
	if (depth > MAX_DEPTH)
		return ambientColor;

	HitData hitData;
	if (Intersection::GetHitData(&hitData, ray, gameObjects))
	{
		if (hitData.hitObject->IsLight)
			return hitData.hitObject->pbrColor;

		XMVECTOR reflectionDir = GetRefelectedDir(ray.GetDir(), hitData.hitPlaneNormal);
		XMVECTOR randomDirVector = GetRandomHemiSphereDir(hitData.hitPlaneNormal);


		if (hitData.hitObject->pbrGlossy)
		{
			RVector3 reflectedRayColor = GetPixelColor(RRay(hitData.hitPoint, reflectionDir), gameObjects, depth + 1) *0.3;
			XMFLOAT3 dotResult;
			XMStoreFloat3(&dotResult, XMVector3Dot(reflectionDir, hitData.hitPlaneNormal));
			return hitData.hitObject->pbrColor + reflectedRayColor * dotResult.x;
		}
		else
		{
			RVector3 reflectedRayColor = GetPixelColor(RRay(hitData.hitPoint, reflectionDir), gameObjects, depth + 1) * 0.3 * 0.1;
			RVector3 blurryReflectedRayColor = GetPixelColor(RRay(hitData.hitPoint, randomDirVector), gameObjects, depth + 1) * 0.3 * 0.9;
			XMFLOAT3 relfectDotResult;
			XMStoreFloat3(&relfectDotResult, XMVector3Dot(reflectionDir, hitData.hitPlaneNormal));
			XMFLOAT3 randomDotResult;
			XMStoreFloat3(&randomDotResult, XMVector3Dot(randomDirVector, hitData.hitPlaneNormal));
			return hitData.hitObject->pbrColor + reflectedRayColor * relfectDotResult.x + blurryReflectedRayColor * randomDotResult.x;
		}

	}
	return ambientColor;
}

RVector3 GetReflectColor(RVector3 c1, RVector3 c2)
{
	return RVector3(c1.x * c2.x, c1.y * c2.y, c1.z * c2.z);
}

XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal)
{
	XMVECTOR randomDir = XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2)));
	XMFLOAT3 dotResult;
	XMStoreFloat3(&dotResult, XMVector3Dot(randomDir, planeNormal));
	if (dotResult.x < 0)
	{
		return GetRefelectedDir(randomDir, planeNormal);
	}
	return randomDir;
}

XMVECTOR GetRefelectedDir(XMVECTOR rayDir, XMVECTOR planeNormal)
{
	return XMVector3Normalize(rayDir + 2 * XMVectorMultiply(XMVector3Dot(-rayDir, planeNormal), planeNormal));
}
