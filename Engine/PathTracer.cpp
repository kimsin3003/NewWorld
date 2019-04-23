#include "PathTracer.h"
#include "Intersection.h"
#include <chrono>
#include <random>
#include "PbrObject.h"

#define MAX_DEPTH 3
XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal);
RVector3 GetReflectedColor(RVector3 c1, RVector3 c2);
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
			return hitData.hitObject->Emittance;
		}

		if (hitData.hitObject->PbrTransparent)
		{
			XMVECTOR refractedDirVector = DirectX::XMVector3Refract(ray.GetDir(), hitData.hitPlaneNormal, hitData.refractionRatio);
			if (DirectX::XMVector3Equal(refractedDirVector, DirectX::XMVectorZero()))
				return Black;
			RVector3 refractedIncomingLightColor = GetPixelColor(RRay(hitData.hitPoint, refractedDirVector), pbrObjects, depth + 1);
			return refractedIncomingLightColor;
		}
		else
		{

			bool isDiffused = (double)rand() / (RAND_MAX) < hitData.hitObject->DiffuseRate ? true : false;
			XMVECTOR incomingLightDir;

			if (isDiffused)
			{
				incomingLightDir = GetRandomHemiSphereDir(hitData.hitPlaneNormal);
			}
			else
			{
				incomingLightDir = XMVector3Reflect(ray.GetDir(), hitData.hitPlaneNormal);
			}

			XMFLOAT3 dotResult;
			RVector3 incomingLightColor = GetPixelColor(RRay(hitData.hitPoint, incomingLightDir), pbrObjects, depth + 1);
			XMStoreFloat3(&dotResult, XMVector3Dot(incomingLightDir, hitData.hitPlaneNormal));

			float randomCos = dotResult.x;
			if (hitData.hitObject->PbrTransparent)
				randomCos = -randomCos; 

			XMFLOAT3 dist;
			XMStoreFloat3(&dist, XMVector3Length(hitData.hitPoint - ray.GetOrigin()));

			bool isAbsorbed = (double)rand() / (RAND_MAX) < hitData.hitObject->AbsorbRate ? true : false;
			RVector3 reflectedColor = isAbsorbed ? GetReflectedColor(hitData.hitObject->Reflectance, incomingLightColor) : incomingLightColor;
			RVector3 diffuseColor = hitData.hitObject->Emittance + reflectedColor;// *randomCos;
			return diffuseColor;
		}
	}
	return Black;
}

RVector3 GetReflectedColor(RVector3 c1, RVector3 c2)
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
		XMVECTOR newDir = XMVector3Reflect(randomDir, planeNormal);
		XMStoreFloat3(&dotResult, XMVector3Dot(newDir, planeNormal));
		if(dotResult.x > 0)
			return newDir;
	}
	return randomDir;
}