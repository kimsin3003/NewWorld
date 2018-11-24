#pragma once
#include "RGameObject.h"
#include "RMath.h"

class RLight : public RGameObject
{
public:
	RLight() { isLight = true; }
	RVector3 color = { 1,1,1 };
	RVector3 dir = { -1,-1,1 };
};

