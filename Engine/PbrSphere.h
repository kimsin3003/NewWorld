#pragma once
#include "RGameObject.h"
#include "ICollider.h"
class PbrSphere :
	public RGameObject, ICollider
{
public:
	float R;

	virtual bool Intersects(RRay ray, float& dist) override;

};

