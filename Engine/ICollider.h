#pragma once
#include "RRay.h"
class ICollider
{
public:
	virtual bool Intersects(RRay ray, float& dist) = 0;
	virtual XMVECTOR GetNormal(XMVECTOR hitPoint) = 0;
};

