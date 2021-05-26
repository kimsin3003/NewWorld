#pragma once
#include <directxmath.h>
#include <vector>
#include <string>
using namespace DirectX;

class RGameObject
{
public:

	virtual ~RGameObject();

	virtual void Start() {};
	virtual void Tick(double deltaTime) {};
	const XMMATRIX GetWorldMatrix();

	inline void SetPosition(float x, float y, float z) { m_position.x = x, m_position.y = y, m_position.z = z; }
	inline void SetRotation(float x, float y, float z) { m_rotation.x = x, m_rotation.y = y, m_rotation.z = z; }

	void SetResource(std::string filenPath);
	std::vector<class RMesh*> Meshes;
private:
	void ProcessNode(const struct aiNode* node, const struct aiScene* scene, std::string path);
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
};
