#include "PathTracer.h"
#include "Intersection.h"
#include "RLight.h"

RVector3 PathTracer::GetPixelColor(RRay ray, std::vector<class RGameObject*> gameObjects, int depth)
{
// 	if (depth > MAX_DEPTH)
// 		return ambientColor;
	RVector3 ambientColor = { 1.0f, 0.1f, 0.1f };
// 	for (auto gameObject : gameObjects)
// 	{
// 		HitData hitData = Intersection::GetHitData(ray, gameObject);
// 		if (hitData.hitObject == nullptr)
// 		{
// 			return ambientColor;
// 		}
// 		else if (const RLight* light = dynamic_cast<const RLight*>(hitData.hitObject))
// 		{
// 			return light->color * (light->dir * ray.GetDir()) / (light->dir.size());
// 		}
// 		else
// 		{
// 			float emmisionRate = hitData.mat.;
// 			hitData.hitPlaneNormal
// 		}
// 	}
	return ambientColor;
}
