
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

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&matrixBufferDesc, NULL, &m_constBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void Material::Render(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!IsInitialized())
		Initialize(device, hwnd);

	if (VertexShader)
	{
		deviceContext->VSSetShader(VertexShader, 0, 0);
	}

	if (PixelShader)
	{
		deviceContext->PSSetShader(PixelShader, 0, 0);
	}
	deviceContext->IASetInputLayout(InputLayout);

	SetConstBuffer(deviceContext, worldMatrix, projectionMatrix, viewMatrix);
}

void Material::SetConstBuffer(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstBuffer* dataPtr;
	


	// ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �մϴ�.
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);


	// ��� ������ ������ �� �� �ֵ��� ��޴ϴ�.
	result = deviceContext->Map(m_constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	// ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
	dataPtr = (ConstBuffer*)mappedResource.pData;

	// ��� ���ۿ� ����� �����մϴ�.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ��� ������ ����� Ǳ�ϴ�.
	deviceContext->Unmap(m_constBuffer, 0);


	// ���� ���̴������� ��� ������ ��ġ�� �����մϴ�.

	unsigned int bufferNumber = 0;

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_constBuffer);

}

Material::~Material()
{
	VertexShader->Release();
	PixelShader->Release();
	InputLayout->Release();
}