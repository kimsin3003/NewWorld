#include <d3d11.h>
#include <string>
#include "RMesh.h"
#include "RMaterial.h"
#include "Logger.h"

bool RMesh::IsInitialized()
{ 
	return m_vertexBuffer && m_indexBuffer;
}

void RMesh::Initialize(struct ID3D11Device* device)
{
	if (!m_vertexBuffer)
	{
		CreateVertexBuffer(device);

		Logger::Log("VertexBuffer 持失 失因");
	}
	if(!m_indexBuffer)
	{
		CreateIndexBuffer(device);

		Logger::Log("IndexBuffer 持失 失因");
	}
}

void RMesh::Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if (!IsInitialized())
		Initialize(device);
	UINT stride = sizeof(RVertex);
	UINT offset = 0;
	ID3D11Buffer* const buffers[] = { m_vertexBuffer };
	int numOfBuffers = sizeof(buffers) / sizeof(ID3D11Buffer);
	deviceContext->IASetVertexBuffers(0, numOfBuffers, buffers, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexed(Indicies.size(), 0, 0);
}

void RMesh::CreateVertexBuffer(struct ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = Verticies.size() * sizeof(RVertex);
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

void RMesh::CreateIndexBuffer(struct ID3D11Device* device)
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


RMesh::~RMesh()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
	delete Mat;
}
