#pragma once
#include "RGameObject.h"
#include "ICollider.h"
class PbrSphere :
	public RGameObject, public ICollider
{
public:
	float R;

	virtual bool Intersects(RRay ray, float& dist) override;


	virtual XMVECTOR GetNormal(XMVECTOR hitPoint) override;

};

