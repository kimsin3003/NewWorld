#pragma once
#include "RRay.h"
#include "RGameObject.h"
#include <vector>

#define MAX_DEPTH 3

const RVector3 nullColor( 0.f, 0.f, 0.f);
const RVector3 ambientColor = { 0.1f, 0.1f, 0.1f };
class PathTracer
{
public:
	static RVector3 GetPixelColor(RRay ray, const std::vector<class RGameObject*>& gameObjects, int depth);
};

