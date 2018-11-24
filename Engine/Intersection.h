#pragma once
#include "RRay.h"
#include "RGameObject.h"
#include "RMaterial.h"

struct HitData
{
	HitData() {};
	RGameObject* hitObject;
	RVector3 hitPoint;
	RVector3 hitPlaneNormal;
};

class Intersection
{
public:
	static bool GetHitData(HitData* hitData, RRay ray, std::vector<class RGameObject*> gameObjects);
};

