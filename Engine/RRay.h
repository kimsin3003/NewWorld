#pragma once
#include "RMath.h"

class RRay
{
public:
	RRay(int screenX, int screenY, int screenWith, int screenHeight); // for screen to world
	RRay(RVector3 origin, RVector3 dir); // for world to world
	RVector3 GetOrigin() { return m_origin; }
	RVector3 GetDir() { return m_dir; }

private:
	RVector3 m_origin;
	RVector3 m_dir;
};

