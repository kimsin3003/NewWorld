#include "PathTracer.h"
#include "Intersection.h"
#include <chrono>
#include <random>

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
		{
			return hitData.hitObject->pbrColor;
		}

		XMFLOAT3 dotResult;
		XMVECTOR randomDirVector = GetRandomHemiSphereDir(hitData.hitPlaneNormal);
		RVector3 randomIncomingLightColor = GetPixelColor(RRay(hitData.hitPoint, randomDirVector), gameObjects, depth + 1);
		XMStoreFloat3(&dotResult, XMVector3Dot(randomDirVector, hitData.hitPlaneNormal));

		float randomCos = dotResult.x;

		RVector3 diffuseColor = (hitData.hitObject->pbrColor + randomIncomingLightColor) * randomCos;
		return diffuseColor;

	}
	return ambientColor;
}

XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal)
{
	unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());

	// seeds the random number engine, the mersenne_twister_engine
	std::mt19937 generator(seed);

	// set a distribution range (1 - 100)
	std::uniform_int_distribution<int> distribution(1, RAND_MAX);

	XMVECTOR randomDir = XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(distribution(generator) - RAND_MAX / 2, distribution(generator) - RAND_MAX / 2, distribution(generator) - RAND_MAX / 2)));
	XMFLOAT3 dotResult;
	XMStoreFloat3(&dotResult, XMVector3Dot(randomDir, planeNormal));
	if (dotResult.x < 0)
	{
		XMVECTOR newDir = GetRefelectedDir(randomDir, planeNormal);
		XMStoreFloat3(&dotResult, XMVector3Dot(newDir, planeNormal));
		if(dotResult.x > 0)
			return newDir;
	}
	return randomDir;
}

XMVECTOR GetRefelectedDir(XMVECTOR rayDir, XMVECTOR planeNormal)
{
	return XMVector3Normalize(rayDir + 2 * XMVectorMultiply(XMVector3Dot(-rayDir, planeNormal), planeNormal));
}
