#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <time.h>
#include <dxgi.h>
#include <D3D11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Declares.h"
#include "Renderer.h"


bool Renderer::Initialize(HWND hwnd, int winWidth, int winHeight)
{
	InitDevice(hwnd, winWidth, winHeight);
	CreateDepthStencilTexture();
	CreateDepthStencilState();

	CreateMatrices();
	if (!SetVertexBuffer())
		return false;
	if (!SetIndexBuffer())
		return false;
	if (!SetShaders())
		return false;
	if (!SetConstantBuffers())
		return false;

	return true;
}
bool Renderer::Tick(float deltaTime)
{
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_immediateContext->ClearRenderTargetView(m_renderTargetView, ClearColor);
	m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	m_immediateContext->DrawIndexed(4, 0, 0);

	m_swapChain->Present(0, 0);
	return true;
}

HRESULT Renderer::InitDevice(HWND hwnd, int winWidth, int winHeight)
{
	HRESULT hr = S_OK;

	m_width = winWidth;
	m_height = winHeight;

	//Flag����
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;		// �� ���� ����

	sd.BufferDesc.Width = winWidth;
	sd.BufferDesc.Height = winHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//����� ����
	sd.BufferDesc.RefreshRate.Numerator = 60;	//����
	sd.BufferDesc.RefreshRate.Denominator = 1;	//�и�

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ����� ������
	sd.OutputWindow = hwnd;	//����� ������ �ڵ�
	sd.SampleDesc.Count = 1;	//multisampling(���� ǥ��ȭ) 1�̸� �Ѱ��� ����� ����ǥ��ȭ --> ����ǥ��ȭ ���ϰڴٴ� ��.
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&m_swapChain,
		&m_device,
		&m_featureLevel,
		&m_immediateContext);

	if (FAILED(hr))
		return hr;

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_swapChain->GetBuffer(0,							 // �ĸ� ���� �ε���, �������� �� �߿�, ������ 1�� �̹Ƿ� 0.
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = m_device->CreateRenderTargetView(pBackBuffer,
		NULL,
		&m_renderTargetView);

	 pBackBuffer->Release();
	 pBackBuffer = nullptr;

	if (FAILED(hr))
		return hr;

	//���⿡ ������ depthbuffer�� �並 �־����.

	D3D11_VIEWPORT	vp; // �� �����쿡 �������� ȭ���� �׸� �� �ִ� �ڽ���. ex)���̽� ���� 2player���.
	vp.Width = m_width;
	vp.Height = m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // ����Ʈ�� �������� ������, �迭�� �ּҸ� �ִ´�.


	return hr;

}

void Renderer::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC    depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;    // Depth Test Ȱ��ȭ
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // Depth ���� ��� Ȱ��ȭ.
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;  	// Z ���� ������ ���. �� �׸���.

	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void Renderer::CreateDepthStencilTexture()
{

	//Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1; //�Ӹ��� ���� �ʴ´�.
	descDepth.ArraySize = 1; //���� �ϳ��� ���Ľ� ���� ���
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_device->CreateTexture2D(&descDepth, NULL, &m_depthStencilTexture);

	//Create depth stencil view

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;

	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(m_depthStencilTexture, &descDSV, &m_depthStencilView);


	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void Renderer::CreateMatrices()
{
	float fieldOfView = (float) 3.14 / 4.0f; 
	float screenAspect = (float)m_width / (float)m_height;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);
	m_worldMatrix = DirectX::XMMatrixIdentity();
	m_viewMatrix = DirectX::XMMatrixOrthographicLH(m_width, m_height, 0.1f, 1000.0f);
}

bool Renderer::SetShaders()
{
	// load and compile the two shaders
	ID3D10Blob *vsBlob, *psBlob, *errorblob;
	HRESULT hr = D3DCompileFromFile(L"Engine/Default_VS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_4_0", NULL, NULL, &vsBlob, &errorblob);
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
	hr = D3DCompileFromFile(L"Engine/Default_PS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_4_0", NULL, NULL, &psBlob, &errorblob);
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
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &vertexShader);
	if (FAILED(hr))
	{
		return false;
	}
	hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &pixelShader);
	if (FAILED(hr))
	{
		return false;
	}
	// set the shader objects
	m_immediateContext->VSSetShader(vertexShader, 0, 0);
	m_immediateContext->PSSetShader(pixelShader, 0, 0);

// 	create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D11InputLayout* inputLayout = nullptr;
	hr = m_device->CreateInputLayout(ied, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	if (FAILED(hr))
	{
		return false;
	}
	m_immediateContext->IASetInputLayout(inputLayout);


	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0; // ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ְ� �մϴ�. 
	hr = m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer); 
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Renderer::SetConstantBuffers()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;
	unsigned int bufferNumber;


	// ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �մϴ�.
	m_worldMatrix = DirectX::XMMatrixTranspose(m_worldMatrix);
	m_viewMatrix = DirectX::XMMatrixTranspose(m_viewMatrix);
	m_projectionMatrix = DirectX::XMMatrixTranspose(m_projectionMatrix);


	// ��� ������ ������ �� �� �ֵ��� ��޴ϴ�.
	result = m_immediateContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
	dataPtr = (MatrixBuffer*)mappedResource.pData;

	// ��� ���ۿ� ����� �����մϴ�.
	dataPtr->world = m_worldMatrix;
	dataPtr->view = m_viewMatrix;
	dataPtr->projection = m_projectionMatrix;

	// ��� ������ ����� Ǳ�ϴ�.
	m_immediateContext->Unmap(m_matrixBuffer, 0);

	// ���� ���̴������� ��� ������ ��ġ�� �����մϴ�.
	bufferNumber = 0;

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	m_immediateContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;

}

bool Renderer::SetVertexBuffer()
{
	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Col;
	};

	// Supply the actual vertex data.
	Vertex sampleVerticies[] =
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
	bufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(sampleVerticies);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = sampleVerticies;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT hr = S_OK;
	hr = m_device->CreateBuffer(&bufferDesc, &InitData, &m_vertexBuffer);

	if (FAILED(hr))
		return false;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_immediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	
	return true;
}

bool Renderer::SetIndexBuffer()
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
	hr = m_device->CreateBuffer(&bufferDesc, &InitData, &m_indexBuffer);
	if (FAILED(hr))
		return false;

	// Set the buffer.
	m_immediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	return true;
}