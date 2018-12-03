#pragma once
#include "RGameObject.h"
#include "RMath.h"

class RLight : public RGameObject
{
public:
	RLight() { isLight = true; }
	RVector3 color = { 0.8, 0.8, 0.8 };
	RVector3 dir = { -1,-1,1 };
};

