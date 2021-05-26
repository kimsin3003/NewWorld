#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "RMaterial.h"
#include "RCamera.h"
#include "Logger.h"
#include "RContext.h"


std::vector<RTexture*>		RMaterial::m_loadedTextures;

void RMaterial::Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix);

	auto deviceContext = Renderer->GetDeviceContext();

	deviceContext->IASetInputLayout(m_inputLayout);

	if (m_vertexShader)
	{
		deviceContext->VSSetShader(m_vertexShader, 0, 0);
	}

	if (m_pixelShader)
	{
		deviceContext->PSSetShader(m_pixelShader, 0, 0);
	}

	if (m_sampleState)
	{
		deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	}


}

void RMaterial::Load(const struct aiScene* scene, const struct aiMaterial* aiMat)
{
	ID3D10Blob* vsBlob, * psBlob, * errorblob;
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
	hr = Renderer->GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &m_vertexShader);
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

	hr = Renderer->GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &m_pixelShader);
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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Binormal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto device = Renderer->GetDevice();
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

	Logger::Log("SamplerState 생성 성공");

	LoadTextureType(scene, aiMat, RTexture::DIFFUSE);
}

aiTextureType ConvertType(RTexture::RTextureType type)
{
	switch (type)
	{
	case RTexture::DIFFUSE:
		return aiTextureType::aiTextureType_DIFFUSE;
	case RTexture::NORMAL:
		return aiTextureType::aiTextureType_NORMALS;
	case RTexture::SPECULAR:
		return aiTextureType::aiTextureType_SPECULAR;
	}
	return aiTextureType::aiTextureType_NONE;
}

void RMaterial::LoadTextureType(const struct aiScene* scene, const struct aiMaterial* aiMat, RTexture::RTextureType type)
{
	aiTextureType aiType = ConvertType(type);
	for (UINT i = 0; i < aiMat->GetTextureCount(aiType); i++) {
		aiString str;
		aiMat->GetTexture(aiType, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < m_textures.size(); j++) {
			if (std::strcmp(m_loadedTextures[j]->filePath.c_str(), str.C_Str()) == 0) {
				m_textures.push_back(m_loadedTextures[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip) {   // If texture hasn't been loaded already, load it
			RTexture* texture = new RTexture();	
			texture->filePath = str.C_Str();
			texture->rv = CreateShaderResourceView(scene, str.C_Str());
			m_textures.push_back(texture);
			m_loadedTextures.push_back(texture);
		}
	}
}

void RMaterial::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	SetVSConstBuffer(worldMatrix, viewMatrix, projectionMatrix);

	SetPSConstBuffer();

	auto deviceContext = Renderer->GetDeviceContext();

	for (auto texture : m_textures)
	{
		switch (texture->type)
		{
		case RTexture::DIFFUSE:
			deviceContext->PSSetShaderResources(0, 1, &texture->rv);
			break;
		case RTexture::NORMAL:
			deviceContext->PSSetShaderResources(1, 1, &texture->rv);
			break;
		case RTexture::SPECULAR:
			deviceContext->PSSetShaderResources(2, 1, &texture->rv);
			break;
		}
	}

}

void RMaterial::SetVSConstBuffer(XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE vsMappedResource;

	auto deviceContext = Renderer->GetDeviceContext();

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

void RMaterial::SetPSConstBuffer()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE psMappedResource;
	auto deviceContext = Renderer->GetDeviceContext();
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
	psConstDataPtr->ambientColor = XMFLOAT4(0.1, 0.1, 0.1, 0);

	// 상수 버퍼의 잠금을 풉니다.
	deviceContext->Unmap(m_psConstBuffer, 0);


	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	unsigned int bufferSlot = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	deviceContext->PSSetConstantBuffers(bufferSlot, 1, &m_psConstBuffer);
}

ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture) {
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0) {
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		hr = Renderer->GetDevice()->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
			MessageBoxA(SysManager->GetHWND(), "CreateTexture2D failed!", "Error!", MB_ICONERROR | MB_OK);

		hr = Renderer->GetDevice()->CreateShaderResourceView(texture2D, nullptr, &texture);
		if (FAILED(hr))
			MessageBoxA(SysManager->GetHWND(), "CreateShaderResourceView failed!", "Error!", MB_ICONERROR | MB_OK);

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(Renderer->GetDevice(), Renderer->GetDeviceContext(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
	if (FAILED(hr))
		MessageBoxA(SysManager->GetHWND(), "Texture couldn't be created from memory!", "Error!", MB_ICONERROR | MB_OK);

	return texture;
}

ID3D11ShaderResourceView* RMaterial::CreateShaderResourceView(const struct aiScene* scene, std::string fileName)
{
	const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(fileName.c_str());
	if (embeddedTexture != nullptr) {
		return LoadEmbeddedTexture(embeddedTexture);
	}
	else {
		std::string filename = m_directory + '/' + fileName;
		std::wstring filenamews = std::wstring(filename.begin(), filename.end());
		ID3D11ShaderResourceView* rv = nullptr;
		HRESULT hr = CreateWICTextureFromFile(Renderer->GetDevice(), filenamews.c_str(), nullptr, &rv);
		if (FAILED(hr))
		{
			MessageBoxA(SysManager->GetHWND(), "Texture couldn't be loaded", "Error!", MB_ICONERROR | MB_OK);
		}
		return rv;
	}
}

RMaterial::~RMaterial()
{
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_inputLayout->Release();
}
