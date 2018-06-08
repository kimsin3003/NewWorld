
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "Material.h"
#include "Camera.h"
#include "Logger.h"

void Material::Initialize(struct ID3D11Device* device)
{
	ID3D10Blob *vsBlob, *psBlob, *errorblob;
	HRESULT hr = D3DCompileFromFile(m_vsFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", NULL, NULL, &vsBlob, &errorblob);
	if (FAILED(hr))
	{
		if (errorblob)
		{
			Logger::Log((char*)errorblob->GetBufferPointer());
			errorblob->Release();
		}

		if (vsBlob)
			vsBlob->Release();

		return;
	}

	// encapsulate both shaders into shader objects
	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &m_vertexShader);
	if (FAILED(hr))
	{
		Logger::Log("VertexShader 생성 오류");
		return;
	}

	hr = D3DCompileFromFile(m_psFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", NULL, NULL, &psBlob, &errorblob);
	if (FAILED(hr))
	{
		if (errorblob)
		{
			Logger::Log((char*)errorblob->GetBufferPointer());
			errorblob->Release();
		}

		if (psBlob)
			psBlob->Release();

		return;
	}

	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &m_pixelShader);
	if (FAILED(hr))
	{
		Logger::Log("PixelShader 생성 오류");
		return;
	}

	Logger::Log("Shader 생성 성공");

	// 	create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = device->CreateInputLayout(ied, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
	vsBlob->Release();
	psBlob->Release();

	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&constBufferDesc, NULL, &m_constBuffer);
	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
	Logger::Log("ConstBuffer 생성 성공");
}

void Material::Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!IsInitialized())
		Initialize(device);

	deviceContext->IASetInputLayout(m_inputLayout);

	if (m_vertexShader)
	{
		deviceContext->VSSetShader(m_vertexShader, 0, 0);
	}

	if (m_pixelShader)
	{
		deviceContext->PSSetShader(m_pixelShader, 0, 0);
	}

	SetConstBuffer(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
}

void Material::SetConstBuffer(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstBuffer* dataPtr;
	


	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다.
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);


	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	result = deviceContext->Map(m_constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		Logger::Log(result);
		return;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	dataPtr = (ConstBuffer*)mappedResource.pData;

	// 상수 버퍼에 행렬을 복사합니다.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 상수 버퍼의 잠금을 풉니다.
	deviceContext->Unmap(m_constBuffer, 0);


	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.

	unsigned int bufferSlot = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	deviceContext->VSSetConstantBuffers(bufferSlot, 1, &m_constBuffer);

}

Material::~Material()
{
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_inputLayout->Release();
}
