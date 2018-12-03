#include "Intersection.h"
#include "RRay.h"
#include "RGameObject.h"
#include "RMesh.h"
#include <DirectXCollision.h>

bool Intersection::GetHitData(HitData* hitData, RRay ray, std::vector<class RGameObject*> gameObjects)
{
	float minT = 10000000.0;
	for (auto gameObject : gameObjects)
	{
		XMMATRIX worldMatrix = gameObject->GetWorldMatrix();
		for (RMesh* mesh : gameObject->Meshes)
		{
			for (int i = 0; i < mesh->Indicies.size(); i += 3)
			{
				int aIndex = mesh->Indicies[i];
				int bIndex = mesh->Indicies[i + 1];
				int cIndex = mesh->Indicies[i + 2];
				XMVECTOR a = XMVector3TransformCoord(XMLoadFloat3(&mesh->Verticies[aIndex].Pos), worldMatrix);
				XMVECTOR b = XMVector3TransformCoord(XMLoadFloat3(&mesh->Verticies[bIndex].Pos), worldMatrix);
				XMVECTOR c = XMVector3TransformCoord(XMLoadFloat3(&mesh->Verticies[cIndex].Pos), worldMatrix);
				
				float t = 0.0f;
				if (DirectX::TriangleTests::Intersects(ray.GetOrigin(), ray.GetDir(), a, b, c, t))
				{
					if (t < minT)
					{
						minT = t;
						hitData->hitObject = gameObject;
						hitData->hitPlaneNormal = XMVector3Normalize(XMVector3Cross((a - b), (a - c)));
						hitData->hitPoint = ray.GetOrigin() + ray.GetDir() * t;
					}
				}
			}
		}
	}

	if (hitData->hitObject != nullptr)
		return true;
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
	//�������� ���� �� �������� �̷���� �� ���͵��� �̷�� ���� ���� 360���� ������ �ﰢ�� ������ ���� �ƴϴ�.
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