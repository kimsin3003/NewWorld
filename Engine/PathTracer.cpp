#include "PathTracer.h"
#include "Intersection.h"
#include <chrono>
#include <random>
#include "PbrObject.h"
#include "RMath.h"

#define MAX_DEPTH 3
XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal);
RVector3 GetAbsorbedColor(RVector3 c1, RVector3 c2);
XMVECTOR GetRandomDir();
XMVECTOR GetConeDir(XMVECTOR normal, float maxAngle);
float GetCos(XMVECTOR v1, XMVECTOR v2);
RVector3 Embient = { 0.0f, 0.0f, 0.0f };

RVector3 PathTracer::GetPixelColor(RRay ray, const std::vector<class PbrObject*>& pbrObjects, int depth)
{

	HitData hitData;
	if (Intersection::GetHitData(&hitData, ray, pbrObjects))
	{
		if (hitData.hitObject->IsLight)
		{
			return hitData.hitObject->Emittance;
		}
		else if (depth == MAX_DEPTH)
			return Embient;

		if (hitData.hitObject->PbrTransparent)
		{
			XMVECTOR refractedDirVector = DirectX::XMVector3Refract(ray.GetDir(), hitData.hitPlaneNormal, hitData.refractionRatio);
			if (DirectX::XMVector3Equal(refractedDirVector, DirectX::XMVectorZero()))
				return Embient;
			RVector3 refractedIncomingLightColor = GetPixelColor(RRay(hitData.hitPoint, refractedDirVector), pbrObjects, depth + 1);
			return refractedIncomingLightColor * 0.9f;;
		}
		else
		{

			bool isDiffused = (double)rand() / (RAND_MAX) < hitData.hitObject->DiffuseRate ? true : false;
			XMVECTOR incomingLightDir;

			if (hitData.hitObject->PbrColliderType == PBRColliderType::PLANE)
			{
				incomingLightDir = GetRandomHemiSphereDir(hitData.hitPlaneNormal);
			}
			else
			{
				incomingLightDir = XMVector3Reflect(ray.GetDir(), hitData.hitPlaneNormal);
				incomingLightDir = GetConeDir(incomingLightDir, 2);
			}

			RVector3 incomingLightColor = GetPixelColor(RRay(hitData.hitPoint, incomingLightDir), pbrObjects, depth + 1);

			float cos = GetCos(incomingLightDir, hitData.hitPlaneNormal);
			if (hitData.hitObject->PbrTransparent)
				cos = -cos;


			bool isAbsorbed = (double)rand() / (RAND_MAX) < hitData.hitObject->AbsorbRate ? true : false;
			RVector3 reflectedColor = isAbsorbed ? GetAbsorbedColor(hitData.hitObject->Reflectance, incomingLightColor) * cos : incomingLightColor;
			RVector3 diffuseColor = hitData.hitObject->Emittance + reflectedColor;
			return diffuseColor * 0.9f;
		}
	}
	return Embient;
}

float GetCos(XMVECTOR v1, XMVECTOR v2)
{
	XMFLOAT3 dotResult;
	XMStoreFloat3(&dotResult, XMVector3Dot(v1, v2));
	return dotResult.x;
}

RVector3 GetAbsorbedColor(RVector3 c1, RVector3 c2)
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

XMVECTOR GetConeDir(XMVECTOR normal, float maxAngle)
{
	XMVECTOR randomDir = GetRandomDir();
	XMVECTOR angleToRotate = XMVector3AngleBetweenNormals(normal, randomDir) * maxAngle / 90.0f;
	XMVECTOR axis = XMVector3Cross(normal, randomDir);

	XMFLOAT3 angle;
	XMStoreFloat3(&angle, angleToRotate);
	XMVECTOR rotation = XMQuaternionRotationAxis(axis, angle.x);
	XMVECTOR result = XMVector3Rotate(normal, rotation);
	return result;
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