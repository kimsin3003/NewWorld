#include "PathTracer.h"
#include "Intersection.h"
#include "RLight.h"

RVector3 PathTracer::GetPixelColor(RRay ray, const std::vector<class RGameObject*>& gameObjects, int depth)
{
	if (depth > MAX_DEPTH)
		return ambientColor;
	RVector3 ambientColor = { 1.0f, 1.0f, 1.0f };

	HitData hitData;
	if (Intersection::GetHitData(&hitData, ray, gameObjects))
	{
		if (hitData.hitObject->IsLight())
		{
			RLight* light = static_cast<RLight*>(hitData.hitObject);
			return light->color * (light->dir * ray.GetDir()) / (light->dir.size());
		}
		else
		{
			return { 1.0f, 0, 0 };
		}
	}
	return ambientColor;
}
