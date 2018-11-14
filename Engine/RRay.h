#pragma once
#include "RMath.h"

class RRay
{
public:
	RRay(RVector3 origin, RVector3 dir) : m_origin(origin), m_dir(dir) {}
	RVector3 GetOrigin() { return m_origin; }
	RVector3 GetDir() { return m_dir; }

	RVector3 m_origin;
	RVector3 m_dir;
};

