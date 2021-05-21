#pragma once
#include <directxmath.h>
#include <vector>
#include "RVector.h"

using namespace DirectX;
using RVector2 = XMFLOAT2;
struct RVertex
{
	RVector3 Pos;
	std::vector<RVector2> UV;
	RVector3 Normal;
};

class RMesh
{
public:
	bool IsInitialized();
	void Initialize(struct ID3D11Device* device);
	void Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext);
	virtual ~RMesh();
	class RMaterial*			Mat = nullptr;
	std::vector<RVertex>	Verticies;
	std::vector<unsigned int>	Indicies;

private:
	void CreateVertexBuffer(struct ID3D11Device* device);
	void CreateIndexBuffer(struct ID3D11Device* device);
	struct ID3D11Buffer*		m_vertexBuffer	= nullptr;
	struct ID3D11Buffer*		m_indexBuffer	= nullptr;
};