
#include <d3d11.h>
#include <D3Dcompiler.h>
#include "Material.h"
#include "Camera.h"

bool Material::Initialize(struct ID3D11Device* device, HWND hwnd)
{
	ID3D10Blob *vsBlob, *psBlob, *errorblob;
	HRESULT hr = D3DCompileFromFile(m_vsFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", NULL, NULL, &vsBlob, &errorblob);
	if (FAILED(hr))
	{
		if (errorblob)
		{
			OutputDebugStringA((char*)errorblob->GetBufferPointer());
			errorblob->Release();
		}

		if (vsBlob)
			vsBlob->Release();

		return false;
	}
	hr = D3DCompileFromFile(m_psFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", NULL, NULL, &psBlob, &errorblob);
	if (FAILED(hr))
	{
		if (errorblob)
		{
			OutputDebugStringA((char*)errorblob->GetBufferPointer());
			errorblob->Release();
		}

		if (psBlob)
			psBlob->Release();

		return false;
	}

	// encapsulate both shaders into shader objects
	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &VertexShader);
	if (FAILED(hr))
	{
		return false;
	}
	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &PixelShader);
	if (FAILED(hr))
	{
		return false;
	}
	// set the shader objects

	// 	create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = device->CreateInputLayout(ied, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &InputLayout);
	if (FAILED(hr))
	{
		return false;
	}
	vsBlob->Release();
	psBlob->Release();
	if (errorblob)
		errorblob->Release();

	return true;
}

Material::ConstBuffer* Material::CreateConstBuffer(const XMMATRIX& projectionMatrix, const XMMATRIX& viewMatrix, class GameObject* gameObject)
{
	float fieldOfView = (float) 3.14 / 4.0f;
	float screenAspect = (float)m_winWidth / (float)m_winHeight;
	XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);
	XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
	XMMATRIX viewMatrix = camera->GetViewMatrix(); 
	ID3D11Buffer matrixBuffer;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	unsigned int bufferNumber;


	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다.
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);


	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	result = m_immediateContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	matrixBuffer = (MatrixBuffer*)mappedResource.pData;

	// 상수 버퍼에 행렬을 복사합니다.
	matrixBuffer->world = worldMatrix;
	matrixBuffer->view = viewMatrix;
	matrixBuffer->projection = projectionMatrix;

	// 상수 버퍼의 잠금을 풉니다.
	m_immediateContext->Unmap(matrixBuffer, 0);


	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	bufferNumber = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	m_immediateContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	return true;
}

Material::~Material()
{
	VertexShader->Release();
	PixelShader->Release();
	InputLayout->Release();
}
