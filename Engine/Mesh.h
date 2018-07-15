#pragma once
#include <directxmath.h>
#include <vector>

using namespace DirectX;
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 UV;
};

class Mesh
{
public:
	bool IsInitialized();
	void Initialize(struct ID3D11Device* device);
	inline void SetData(std::vector<struct Vertex> verticies, std::vector<unsigned int> indicies) { m_verticies = verticies; m_indicies = indicies; }
	void Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext);
	virtual ~Mesh();
	class Material*			Mat = nullptr;

private:
	void CreateVertexBuffer(struct ID3D11Device* device);
	void CreateIndexBuffer(struct ID3D11Device* device);
	std::vector<struct Vertex>	m_verticies;
	std::vector<unsigned int>	m_indicies;
	struct ID3D11Buffer*		m_vertexBuffer	= nullptr;
	struct ID3D11Buffer*		m_indexBuffer	= nullptr;
};