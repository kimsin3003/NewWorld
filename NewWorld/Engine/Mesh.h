#pragma once
#include <directxmath.h>
#include <vector>
#include "Share/ResourceInfo.h"

using namespace DirectX;
struct Vertex
{
	Vertex() {};
	Vertex(const VertexInfo& vertexInfo) 
	{
		Pos.x = vertexInfo.Pos.x;
		Pos.y = vertexInfo.Pos.y;
		Pos.z = vertexInfo.Pos.z;
		UV.x = vertexInfo.UV.x;
		UV.y = vertexInfo.UV.y;
	}
	XMFLOAT3 Pos;
	XMFLOAT2 UV;
};

class Mesh
{
public:
	bool IsInitialized();
	void Initialize(struct ID3D11Device* device);
	void SetData(class MeshInfo* meshInfo);
	void Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext);
	virtual ~Mesh();
	class Material*			Mat = nullptr;
	std::vector<Vertex>	Verticies;
	std::vector<unsigned int>	Indicies;

private:
	void CreateVertexBuffer(struct ID3D11Device* device);
	void CreateIndexBuffer(struct ID3D11Device* device);
	struct ID3D11Buffer*		m_vertexBuffer	= nullptr;
	struct ID3D11Buffer*		m_indexBuffer	= nullptr;
};