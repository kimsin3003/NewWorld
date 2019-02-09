#pragma once
#include <directxmath.h>
#include <vector>
#include "RMath.h"
using namespace DirectX;

enum class PBRColliderType
{
	NONE,
	SPHERE,
	PLANE
};

class RGameObject
{
public:
	~RGameObject();
	virtual void Start() {};
	virtual void Tick(double deltaTime) {};
	const XMMATRIX GetWorldMatrix();
	
	inline XMFLOAT3 GetPos() { return m_position; }
	inline XMFLOAT3 GetRotation() { return m_rotation; }
	inline void SetPosition(float x, float y, float z) { m_position.x = x, m_position.y = y, m_position.z = z; }
	inline void SetRotation(float x, float y, float z) { m_rotation.x = x, m_rotation.y = y, m_rotation.z = z; }

	void SetResource(std::string filename);
	std::vector<class RMesh*> Meshes;
	std::string Name;
	bool IsLight = false;
	RVector3 emittance;
	RVector3 reflectance;
	PBRColliderType pbrColliderType;
	bool pbrTransparent = false;
	float refractionRate = 1.f;

protected:

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
};

