#pragma once
#include <directxmath.h>

using namespace DirectX;
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Col;
};

class Mesh
{
public:
	bool IsInitialized();
	bool Initialize(struct ID3D11Device* device);
	void Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext);
	virtual ~Mesh();
	class Material*			Mat = nullptr;

private:
	void CreateVertexBuffer(struct ID3D11Device* device);
	bool CreateIndexBuffer(struct ID3D11Device* device);
	struct ID3D11Buffer*		m_vertexBuffer	= nullptr;
	struct ID3D11Buffer*		m_indexBuffer	= nullptr;
};