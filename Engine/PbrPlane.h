#pragma once
#include "RGameObject.h"
#include "ICollider.h"
#include <DirectXMath.h>
class PbrPlane :
	public RGameObject, ICollider
{
public:
	XMFLOAT3 V1;
	XMFLOAT3 V2;
	XMFLOAT3 V3;
	XMFLOAT3 V4;

	virtual bool Intersects(RRay ray, float& dist) override;
	DirectX::XMVECTOR GetNormal();

};

