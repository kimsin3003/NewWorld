#pragma once
#include <directxmath.h>
#include <vector>
#include "RMath.h"
using namespace DirectX;

class RGameObject
{
public:

	virtual void Start() {};
	virtual void Tick(double deltaTime) {};
	const XMMATRIX GetWorldMatrix();
	bool IsLight() { return isLight; }

	inline void SetPosition(float x, float y, float z) { m_position.x = x, m_position.y = y, m_position.z = z; }
	inline void SetRotation(float x, float y, float z) { m_rotation.x = x, m_rotation.y = y, m_rotation.z = z; }

	void SetResource(std::string filename);
	std::vector<class RMesh*> Meshes;
	std::string Name;
	RVector3 color;

protected:
	bool isLight = false;

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
};

