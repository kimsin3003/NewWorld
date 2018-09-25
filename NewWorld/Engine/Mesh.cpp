#include <d3d11.h>
#include <string>
#include "Mesh.h"
#include "Material.h"
#include "Logger.h"

bool Mesh::IsInitialized()
{ 
	return m_vertexBuffer && m_indexBuffer;
}

void Mesh::Initialize(struct ID3D11Device* device)
{
	if (!m_vertexBuffer)
	{
		CreateVertexBuffer(device);

		Logger::Log("VertexBuffer ���� ����");
	}
	if(!m_indexBuffer)
	{
		CreateIndexBuffer(device);

		Logger::Log("IndexBuffer ���� ����");
	}
}

void Mesh::SetData(MeshInfo* meshInfo)
{
	for (const VertexInfo& vertexInfo : meshInfo->Verticies)
	{
		Verticies.emplace_back(vertexInfo);
	}

	for (unsigned int index : meshInfo->Indicies)
	{
		Indicies.emplace_back(index);
	}
}

void Mesh::Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if (!IsInitialized())
		Initialize(device);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* const buffers[] = { m_vertexBuffer };
	int numOfBuffers = sizeof(buffers) / sizeof(ID3D11Buffer);
	deviceContext->IASetVertexBuffers(0, numOfBuffers, buffers, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexed(Indicies.size(), 0, 0);
}

void Mesh::CreateVertexBuffer(struct ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = Verticies.size() * sizeof(Vertex);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &(Verticies.front());
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

void Mesh::CreateIndexBuffer(struct ID3D11Device* device)
{
	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = Indicies.size() * sizeof(int);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &(Indicies.front());
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	HRESULT hr = S_OK;
	hr = device->CreateBuffer(&bufferDesc, &initData, &m_indexBuffer);
	if (FAILED(hr))
	{
		Logger::Log(hr);
	}
}


Mesh::~Mesh()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
}