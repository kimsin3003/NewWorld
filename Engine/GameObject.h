#pragma once
#include <directxmath.h>
#include <vector>
using namespace DirectX;

class GameObject
{
public:

	GameObject(int index) : m_index(index) {};
	virtual ~GameObject();

	virtual void Start();
	virtual void Tick();
	inline int GetIndex() { return m_index; }
	const XMMATRIX GetWorldMatrix();

	inline void SetPosition(float x, float y, float z) { m_position.x = x, m_position.y = y, m_position.z = z; }
	inline void SetRotation(float x, float y, float z) { m_rotation.x = x, m_rotation.y = y, m_rotation.z = z; }
	std::vector<class Mesh*> Meshes;
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	int m_index;
};

