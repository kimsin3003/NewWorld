#include "Intersection.h"
#include "RRay.h"
#include "RGameObject.h"
#include "RMesh.h"

bool Intersection::GetHitData(HitData* hitData, RRay ray, std::vector<class RGameObject*> gameObjects)
{
	for (auto gameObject : gameObjects)
	{
		for (RMesh* mesh : gameObject->Meshes)
		{
			for (int i = 0; i < mesh->Indicies.size(); i += 3)
			{
				int aIndex = mesh->Indicies[i];
				int bIndex = mesh->Indicies[i + 1];
				int cIndex = mesh->Indicies[i + 2];
				XMFLOAT3 a = mesh->Verticies[aIndex].Pos;
				XMFLOAT3 b = mesh->Verticies[bIndex].Pos;
				XMFLOAT3 c = mesh->Verticies[cIndex].Pos;

				XMVECTOR center = (XMLoadFloat3(&a) + XMLoadFloat3(&b) + XMLoadFloat3(&c)) / 3;
				XMFLOAT3 centerInWorldCoord;
				XMStoreFloat3(&centerInWorldCoord, XMVector3TransformCoord(center, gameObject->GetWorldMatrix()));

				RVector3 rayOrigin = ray.GetOrigin();
				RVector3 rayDir = ray.GetDir();

				RVector3 rayOriginToCenter = RVector3(centerInWorldCoord) - rayOrigin;
				if (rayOriginToCenter * rayDir < 0.001f)
				{
					hitData->hitObject = gameObject;
					return true;
				}
			}
		}
	}

	return false;
}
