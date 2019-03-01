#include "PathTracer.h"
#include "Intersection.h"
#include <chrono>
#include <random>
#include "PbrObject.h"

#define MAX_DEPTH 3
XMVECTOR GetRefractedDir(XMVECTOR rayDir, XMVECTOR planeNormal, float refractionRatio);
XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal);
XMVECTOR GetRefelectedDir(XMVECTOR rayDir, XMVECTOR planeNormal);
RVector3 GetReflectColor(RVector3 c1, RVector3 c2);
XMVECTOR GetRandomDir();

RVector3 PathTracer::GetPixelColor(RRay ray, const std::vector<class PbrObject*>& pbrObjects, int depth)
{
	RVector3 Black = { 0, 0, 0 };
	if (depth > MAX_DEPTH)
		return Black;

	HitData hitData;
	if (Intersection::GetHitData(&hitData, ray, pbrObjects))
	{
		if (hitData.hitObject->IsLight)
		{
			return hitData.hitObject->emittance;
		}

		XMFLOAT3 dotResult;
		XMVECTOR randomDirVector;
		if (hitData.hitObject->pbrTransparent)
			randomDirVector = GetRefractedDir(ray.GetDir(), hitData.hitPlaneNormal, hitData.refractionRatio);
		else
			randomDirVector = GetRandomHemiSphereDir(hitData.hitPlaneNormal);

		RVector3 randomIncomingLightColor = GetPixelColor(RRay(hitData.hitPoint, randomDirVector), pbrObjects, depth + 1);
		XMStoreFloat3(&dotResult, XMVector3Dot(randomDirVector, hitData.hitPlaneNormal));

		float randomCos = dotResult.x;
		if (hitData.hitObject->pbrTransparent)
			randomCos = -randomCos;

		XMFLOAT3 dist;
		XMStoreFloat3(&dist, XMVector3Length(hitData.hitPoint - ray.GetOrigin()));

		RVector3 diffuseColor = hitData.hitObject->emittance + GetReflectColor(hitData.hitObject->reflectance, randomIncomingLightColor) * randomCos;// / dist.x;
		return diffuseColor;

	}
	return Black;
}

RVector3 GetReflectColor(RVector3 c1, RVector3 c2)
{
	return RVector3(c1.x * c2.x, c1.y * c2.y, c1.z * c2.z);
}

XMVECTOR GetRandomDir()
{
	unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());

	// seeds the random number engine, the mersenne_twister_engine
	std::mt19937 generator(seed);

	// set a distribution range (1 - 100)
	std::uniform_int_distribution<int> distribution(1, RAND_MAX);

	return XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(distribution(generator) - RAND_MAX / 2, distribution(generator) - RAND_MAX / 2, distribution(generator) - RAND_MAX / 2)));
}

XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal)
{
	XMVECTOR randomDir = GetRandomDir();
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

XMVECTOR GetRefractedDir(XMVECTOR rayDir, XMVECTOR planeNormal, float refractionRatio)
{
	XMFLOAT3 dotResult;
	XMVECTOR rayDirNormal = refractionRatio > 1 ? -planeNormal : planeNormal;
	XMStoreFloat3(&dotResult, XMVector3Dot(rayDir, rayDirNormal));
	float rayNormalCos = dotResult.x;
	XMVECTOR raydirProjectedToNormal = rayDirNormal * rayNormalCos;
	XMVECTOR refractedDir = (rayDir - raydirProjectedToNormal) / refractionRatio +
		sqrt(1 - pow(refractionRatio, 2) * (1 - pow(rayNormalCos, 2))) * rayDirNormal;
	return XMVector3Normalize(refractedDir);
}
