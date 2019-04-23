#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXTex.h>
#include "RMaterial.h"
#include "RCamera.h"
#include "Logger.h"

static HRESULT CreateShaderResourceViewFromFile(ID3D11Device* device, std::wstring filename, ID3D11ShaderResourceView** pSRV);

void RMaterial::Initialize(struct ID3D11Device* device)
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = device->CreateInputLayout(ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
	vsBlob->Release();
	psBlob->Release();

	D3D11_BUFFER_DESC vsConstBufferDesc;
	vsConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vsConstBufferDesc.ByteWidth = sizeof(VSConstBuffer);
	vsConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vsConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vsConstBufferDesc.MiscFlags = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&vsConstBufferDesc, NULL, &m_vsConstBuffer);
	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
	Logger::Log("VSConstBuffer 생성 성공");

	D3D11_BUFFER_DESC psConstBufferDesc;
	psConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	psConstBufferDesc.ByteWidth = sizeof(PSConstBuffer);
	psConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	psConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	psConstBufferDesc.MiscFlags = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	hr = device->CreateBuffer(&psConstBufferDesc, NULL, &m_psConstBuffer);
	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
	Logger::Log("PSConstBuffer 생성 성공");

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
	for (auto texInfo : m_textures)
	{
		if (texInfo.type == RTexture::DIFFUSE)
			CreateShaderResourceViewFromFile(device, texInfo.filename, &m_textureView);
	}

	Logger::Log("SamplerState 생성 성공");
}

void RMaterial::Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!IsInitialized())
		Initialize(device);

	//PBR을 위해 매번 그릴때마다 택스쳐를 갱신한다.
	for (auto texInfo : m_textures)
	{
		if (texInfo.type == RTexture::DIFFUSE)
		{
			m_textureView->Release();
			CreateShaderResourceViewFromFile(device, texInfo.filename, &m_textureView);
		}
	}

	SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);

	deviceContext->IASetInputLayout(m_inputLayout);

	if (m_vertexShader)
	{
		deviceContext->VSSetShader(m_vertexShader, 0, 0);
	}

	if (m_pixelShader)
	{
		deviceContext->PSSetShader(m_pixelShader, 0, 0);
	}

	if (m_pixelShader)
	{
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	}


}

void RMaterial::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	SetVSConstBuffer(worldMatrix, viewMatrix, projectionMatrix, deviceContext);

	SetPSConstBuffer(deviceContext);

	deviceContext->PSSetShaderResources(0, 1, &m_textureView);

}

void RMaterial::SetVSConstBuffer(XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE vsMappedResource;

	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다.
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	result = deviceContext->Map(m_vsConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vsMappedResource);
	if (FAILED(result))
	{
		Logger::Log(result);
		return;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.

	VSConstBuffer* vsConstDataPtr = (VSConstBuffer*)vsMappedResource.pData;

	// 상수 버퍼에 행렬을 복사합니다.
	vsConstDataPtr->world = worldMatrix;
	vsConstDataPtr->view = viewMatrix;
	vsConstDataPtr->projection = projectionMatrix;
	vsConstDataPtr->lightPosition = XMFLOAT4(0, 0, 0, 0);

	// 상수 버퍼의 잠금을 풉니다.
	deviceContext->Unmap(m_vsConstBuffer, 0);


	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	unsigned int bufferSlot = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	deviceContext->VSSetConstantBuffers(bufferSlot, 1, &m_vsConstBuffer);
}

void RMaterial::SetPSConstBuffer(ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE psMappedResource;
	result = deviceContext->Map(m_psConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &psMappedResource);
	if (FAILED(result))
	{
		Logger::Log(result);
		return;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	PSConstBuffer* psConstDataPtr = (PSConstBuffer*)psMappedResource.pData;

	// 상수 버퍼에 행렬을 복사합니다.
	psConstDataPtr->lightIntensity = XMFLOAT4(1, 0, 0, 0);
	psConstDataPtr->ambientColor = XMFLOAT4(0.1, 0.1, 0.3, 0);

	// 상수 버퍼의 잠금을 풉니다.
	deviceContext->Unmap(m_psConstBuffer, 0);


	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	unsigned int bufferSlot = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	deviceContext->PSSetConstantBuffers(bufferSlot, 1, &m_psConstBuffer);
}

static HRESULT CreateShaderResourceViewFromFile(ID3D11Device* device, std::wstring filename, ID3D11ShaderResourceView** pSRV)
{

	TexMetadata imageMetadata;

	ScratchImage* pScratchImage = new ScratchImage();

	HRESULT hr;

	if (filename.find(L".dds") != std::string::npos)
	{
		hr = LoadFromDDSFile(filename.c_str(), DDS_FLAGS_NONE, &imageMetadata, *pScratchImage);
	}
	else if (filename.find(L".tga") != std::string::npos)
	{
		hr = LoadFromTGAFile(filename.c_str(), &imageMetadata, *pScratchImage);
	}
	else
	{
		hr = LoadFromWICFile(filename.c_str(), WIC_FLAGS_NONE, &imageMetadata, *pScratchImage);
	}

	if (SUCCEEDED(hr))
	{

		hr = CreateShaderResourceView(

			device,

			pScratchImage->GetImages(),

			pScratchImage->GetImageCount(),

			imageMetadata,

			pSRV);

	}

	delete pScratchImage;

	return hr;
}

RMaterial::~RMaterial()
{
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_inputLayout->Release();
	m_sampleState->Release();
	m_textureView->Release();
	m_vsConstBuffer->Release();
	m_psConstBuffer->Release();
	delete m_vsFileName;
	delete m_psFileName;
}
