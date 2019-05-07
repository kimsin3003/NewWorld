#include "PathTracer.h"
#include "Intersection.h"
#include <chrono>
#include <random>
#include "PbrObject.h"
#include "RMath.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>


#define MAX_DEPTH 3
XMVECTOR GetRandomHemiSphereDir(XMVECTOR planeNormal);
RColor GetMaterialColor(RColor c1, RColor c2);
XMVECTOR GetRandomDir();
XMVECTOR GetConeDir(XMVECTOR normal, float maxAngle);
float GetCos(XMVECTOR v1, XMVECTOR v2);
RColor Embient = { 0.0f, 0.0f, 0.0f };

RColor PathTracer::Trace(RRay ray, const std::vector<class PbrObject*>& pbrObjects, int depth)
{

	HitData hitData;
	if (Intersection::GetHitData(&hitData, ray, pbrObjects))
	{
		PbrMaterial material = hitData.hitObject->Material;
		if (material.IsLight)
		{
			return material.Emittance;
		}
		else if (depth == MAX_DEPTH)
			return Embient;

		if (material.IsTransparent)
		{
			XMVECTOR refractedDirVector = DirectX::XMVector3Refract(ray.GetDir(), hitData.hitPlaneNormal, hitData.refractionRatio);
			if (DirectX::XMVector3Equal(refractedDirVector, DirectX::XMVectorZero()))
				return Embient;
			RColor refractedIncomingLightColor = Trace(RRay(hitData.hitPoint, refractedDirVector), pbrObjects, depth + 1);
			return refractedIncomingLightColor * 0.9f;;
		}
		else
		{
			XMVECTOR randomDir = GetRandomHemiSphereDir(hitData.hitPlaneNormal);
			XMVECTOR reflectedDir = GetConeDir(XMVector3Reflect(ray.GetDir(), hitData.hitPlaneNormal), 90 * material.Roughness);
			RColor randomDirIncome = Trace(RRay(hitData.hitPoint, randomDir), pbrObjects, depth + 1);
			RColor reflectDirIncome = Trace(RRay(hitData.hitPoint, reflectedDir), pbrObjects, depth + 1);

			XMFLOAT3 result;
			XMStoreFloat3(&result, XMVector3AngleBetweenNormals(-ray.GetDir(), hitData.hitPlaneNormal));
			float inAngle = result.x;
			XMStoreFloat3(&result, XMVector3AngleBetweenNormals(randomDir, hitData.hitPlaneNormal));
			float randomDirOutAngle = result.x;
			XMStoreFloat3(&result, XMVector3AngleBetweenNormals(reflectedDir, hitData.hitPlaneNormal));
			float reflectDirOutAngle = result.x;

			float PDF = 1.0f / (M_PI); //probability density function value
			RColor diffuseColor = material.Reflectance * randomDirIncome * cos(randomDirOutAngle) * (1 - material.ReflectRate);
			RColor specularColor = material.Reflectance * reflectDirIncome * material.SpecularPBRT(inAngle, reflectDirOutAngle) / PDF * material.ReflectRate;

			return material.Emittance + diffuseColor + specularColor;

// 			XMVECTOR specularLightDir = XMVector3Reflect(ray.GetDir(), hitData.hitPlaneNormal);
// 			specularLightDir = GetConeDir(specularLightDir, 5);
// 
// 			float cos = GetCos(randomDir, hitData.hitPlaneNormal);
// 			if (hitData.hitObject->IsTransparent)
// 				cos = -cos;
// 			RColor reflection = Trace(RRay(hitData.hitPoint, randomDir), pbrObjects, depth + 1) * cos;
// 			RColor specularLightColor = Trace(RRay(hitData.hitPoint, specularLightDir), pbrObjects, depth + 1);
// 
// 			float diffuseRate = hitData.hitObject->DiffuseRate;
// 			reflection = GetMaterialColor(hitData.hitObject->DiffuseConstant, reflection);
// 			specularLightColor = GetMaterialColor(hitData.hitObject->SpecularConstant, specularLightColor);
// 			RColor reflectedColor = reflection * diffuseRate * cos + specularLightColor * (1- diffuseRate);
// 			return hitData.hitObject->Emittance + reflectedColor;
		}
	}
	return Embient;
}



RColor GetMaterialColor(RColor c1, RColor c2)
{
	return RColor(c1.x * c2.x, c1.y * c2.y, c1.z * c2.z);
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
	XMVECTOR randomDir = GetRandomHemiSphereDir(normal);
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