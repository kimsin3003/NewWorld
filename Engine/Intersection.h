#pragma once
#include "RRay.h"
#include "RGameObject.h"
#include "RMaterial.h"

struct HitData
{
	HitData();
	const RGameObject* hitObject;
	RMaterial mat;
	RVector3 hitPoint;
	RVector3 hitPlaneNormal;
};

class Intersection
{
public:
	static HitData GetHitData(RRay ray, RGameObject* gameObject);
};

