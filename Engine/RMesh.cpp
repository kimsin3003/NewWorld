#include <d3d11.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
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

		Logger::Log("VertexBuffer 积己 己傍");
	}
	if(!m_indexBuffer)
	{
		CreateIndexBuffer(device);

		Logger::Log("IndexBuffer 积己 己傍");
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

void RMesh::Load(const struct aiMesh* mesh, const struct aiScene* scene, std::string path)
{
	std::vector<RVertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		RVertex vertex;
		// vertex 困摹, 过急, 咆胶贸 谅钎甫 贸府
		vertex.Pos = RVector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals())
		{
			vertex.Normal = RVector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.Tangent = RVector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.Binormal = RVector3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}

		// 		vertex.UV.clear();
		// 		int j = 0;
		// 		while (mesh->HasTextureCoords(j))
		// 		{
		// 			vertex.UV.emplace_back();
		// 			vertex.UV[j].x = mesh->mTextureCoords[j][i].x;
		// 			vertex.UV[j].y = mesh->mTextureCoords[j][i].y;
		// 			j++;
		// 		}
		if (mesh->HasTextureCoords(0))
		{
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;
		}
		Verticies.push_back(vertex);
	}

	for (int f = 0; f < mesh->mNumFaces; f++)
	{
		for (int i = 0; i < mesh->mFaces[f].mNumIndices; i++)
		{
			Indicies.push_back(mesh->mFaces[f].mIndices[i]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		Mat = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", path);
		Mat->Load(scene, material);
	}
}


RMesh::~RMesh()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
}
