#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"
#include "Logger.h"

bool Mesh::IsInitialized()
{ 
	return m_vertexBuffer && m_indexBuffer;
}

bool Mesh::Initialize(ID3D11Device* device)
{
	if (!m_vertexBuffer)
	{
		if (!CreateVertexBuffer(device))
			return false;
	}
	if(!m_indexBuffer)
	{
		if (!CreateIndexBuffer(device))
			return false;
	}
	return true;
}

void Mesh::Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

	if (IsInitialized())
		Initialize(device);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* const buffers[] = { m_vertexBuffer };
	deviceContext->IASetVertexBuffers(0, sizeof(buffers) / sizeof(ID3D11Buffer), buffers, &stride, &offset);
	deviceContext->IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::CreateVertexBuffer(struct ID3D11Device* device)
{

	Vertex	vertices[] =
	{
		{
			XMFLOAT3(-0.5f, 0.0f, 0.f),
			XMFLOAT3(0.0f, 0.0f, 0.5f)
		},
		{
			XMFLOAT3(0.5f, 0.f, 0.f),
			XMFLOAT3(0.5f, 0.0f, 0.0f)
		},
		{
			XMFLOAT3(0, 0.5f, 0.f),
			XMFLOAT3(0.0f, 0.5f, 0.0f)
		}
	};
	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * 3;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT hr = S_OK;
	hr = device->CreateBuffer(&bufferDesc, &InitData, &m_vertexBuffer);

	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
}

bool Mesh::CreateIndexBuffer(ID3D11Device* device)
{

	// Create indices.
	unsigned int indices[] = { 0, 2, 1 };

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * 3;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	HRESULT hr = S_OK;
	hr = device->CreateBuffer(&bufferDesc, &InitData, &m_indexBuffer);
	if (FAILED(hr))
		return false;
	return true;
}


Mesh::~Mesh()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
}
