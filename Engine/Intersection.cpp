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
				XMVECTOR a = XMLoadFloat3(&mesh->Verticies[aIndex].Pos);
				XMVECTOR b = XMLoadFloat3(&mesh->Verticies[bIndex].Pos);
				XMVECTOR c = XMLoadFloat3(&mesh->Verticies[cIndex].Pos);

				XMVECTOR planeVector = XMVector3Cross(a- b, a - c);
				XMVECTOR planeVectorWorldCoord = XMVector3TransformCoord(planeVector, gameObject->GetWorldMatrix());
				planeVectorWorldCoord = XMVector3Normalize(planeVectorWorldCoord);
				XMVECTOR aPositionInWorldCoord;
				aPositionInWorldCoord = XMVector3TransformCoord(a, gameObject->GetWorldMatrix());

				XMFLOAT3 rayOrigin(ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z);
				XMFLOAT3 rayDir(ray.GetDir().x, ray.GetDir().y, ray.GetDir().z);

				XMVECTOR intersection = GetIntersectPoint(XMLoadFloat3(&rayDir), XMLoadFloat3(&rayOrigin), planeVectorWorldCoord, aPositionInWorldCoord);
				if (CheckPointIsInTriangle(intersection, a, b, c))
				{
					hitData->hitObject = gameObject;
					return true;
				}
			}
		}
	}

	return false;
}

XMVECTOR Intersection::GetIntersectPoint(XMVECTOR rayVector, XMVECTOR rayOrigin, XMVECTOR planeNormal, XMVECTOR planePoint) {
	XMVECTOR diff = rayOrigin - planePoint;
	float prod1 = XMVectorGetX(XMVector3Dot(diff, planeNormal));
	double prod2 = XMVectorGetX(XMVector3Dot(rayVector, planeNormal));
	double prod3 = prod1 / prod2;
	return rayOrigin - rayVector * prod3;
}

bool Intersection::CheckPointIsInTriangle(XMVECTOR point, XMVECTOR a, XMVECTOR b, XMVECTOR c)
{
	//접점으로 부터 세 점까지로 이루어진 세 백터들이 이루는 각의 합이 360보다 작으면 삼각형 내부의 점이 아니다.
	XMVECTOR v1 = a - point;
	XMVECTOR v2 = b - point;
	XMVECTOR v3 = c - point;

	double v1v2Angle = XMVectorGetX(XMVector3AngleBetweenVectors(v1, v2));
	double v2v3Angle = XMVectorGetX(XMVector3AngleBetweenVectors(v2, v3));
	double v3v1Angle = XMVectorGetX(XMVector3AngleBetweenVectors(v3, v1));

	double sumToDegree = (v1v2Angle + v2v3Angle + v3v1Angle) * XM_1DIVPI * 180;

	if (sumToDegree >= 340)
		return true;
	else
		return false;
}