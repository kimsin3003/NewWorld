#pragma once
#include "RGameObject.h"
#include "RMath.h"

class RLight : public RGameObject
{
public:
	RVector3 color = { 1,1,1 };
	RVector3 dir = { -1,-1,1 };
};

