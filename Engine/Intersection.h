#pragma once
#include "RRay.h"
#include "RGameObject.h"
#include "RMaterial.h"

struct HitData
{
	HitData() { hitObject = nullptr; }
	RGameObject* hitObject;
	XMVECTOR hitPoint;
	XMVECTOR hitPlaneNormal;
};

class Intersection
{
public:
	static bool GetHitData(HitData* hitData, RRay ray, std::vector<class RGameObject*> gameObjects);

private:
	static XMVECTOR GetIntersectPoint(XMVECTOR rayVector, XMVECTOR rayOrigin, XMVECTOR planeNormal, XMVECTOR planePoint);
	static bool CheckPointIsInTriangle(XMVECTOR point, XMVECTOR a, XMVECTOR b, XMVECTOR c);
};

