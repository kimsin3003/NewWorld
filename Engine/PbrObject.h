#pragma once
#include <directxmath.h>
#include "RMath.h"
using namespace DirectX;

enum class PBRColliderType
{
	NONE,
	SPHERE,
	PLANE
};

class PbrObject
{
public:
	inline XMFLOAT3 GetPos() { return m_position; }
	inline XMFLOAT3 GetRotation() { return m_rotation; }
	inline void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }
	inline void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	bool IsLight = false;
	RVector3 Emittance;
	float DiffuseRate = 1.0f;
	float AbsorbRate = 0.7f;
	RVector3 Reflectance;
	PBRColliderType PbrColliderType;
	bool PbrTransparent = false;
	float RefractionRate = 1.f;

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
};

