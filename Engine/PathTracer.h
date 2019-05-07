#pragma once
#include "RRay.h"
#include "RGameObject.h"
#include <vector>

class PathTracer
{
public:
	static RColor Trace(RRay ray, const std::vector<class PbrObject*>& pbrObjects, int depth);
};

