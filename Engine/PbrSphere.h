#pragma once
#include "RGameObject.h"
#include "ICollider.h"
#include "PbrObject.h"

class PbrSphere :
	public PbrObject, public ICollider
{
public:
	float R;

	virtual bool Intersects(RRay ray, float& dist) override;
	virtual XMVECTOR GetNormal(XMVECTOR hitPoint) override;

};

