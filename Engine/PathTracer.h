#pragma once
#include "RRay.h"
#include "RGameObject.h"
#include <vector>

class PathTracer
{
public:
	static RVector3 GetPixelColor(RRay ray, const std::vector<class RGameObject*>& gameObjects, int depth);
};

