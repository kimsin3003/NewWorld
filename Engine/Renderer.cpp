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

	//Flag설정
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
	sd.BufferCount = 1;		// 백 버퍼 갯수

	sd.BufferDesc.Width = winWidth;
	sd.BufferDesc.Height = winHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//백버퍼 포맷
	sd.BufferDesc.RefreshRate.Numerator = 60;	//분자
	sd.BufferDesc.RefreshRate.Denominator = 1;	//분모

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 백버퍼 랜더링
	sd.OutputWindow = hwnd;	//출력할 윈도우 핸들
	sd.SampleDesc.Count = 1;	//multisampling(다중 표본화) 1이면 한가지 색깔로 다중표본화 --> 다중표본화 안하겠다는 뜻.
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
	hr = m_swapChain->GetBuffer(0,							 // 후면 버퍼 인덱스, 여러개일 때 중요, 지금은 1개 이므로 0.
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

	//여기에 원래는 depthbuffer의 뷰를 넣어야함.

	D3D11_VIEWPORT	vp; // 한 윈도우에 여러개의 화면을 그릴 수 있는 박스들. ex)레이싱 게임 2player모드.
	vp.Width = m_width;
	vp.Height = m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // 뷰포트가 여러개면 개수와, 배열의 주소를 넣는다.


	return hr;

}

void Renderer::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC    depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;    // Depth Test 활성화
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // Depth 쓰기 기능 활성화.
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;  	// Z 값이 작으면 통과. 즉 그린다.

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
	descDepth.MipLevels = 1; //밉맵은 쓰지 않는다.
	descDepth.ArraySize = 1; //따라서 하나의 스탠실 버퍼 사용
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
	matrixBufferDesc.StructureByteStride = 0; // 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다. 
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


	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다.
	m_worldMatrix = DirectX::XMMatrixTranspose(m_worldMatrix);
	m_viewMatrix = DirectX::XMMatrixTranspose(m_viewMatrix);
	m_projectionMatrix = DirectX::XMMatrixTranspose(m_projectionMatrix);


	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	result = m_immediateContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	dataPtr = (MatrixBuffer*)mappedResource.pData;

	// 상수 버퍼에 행렬을 복사합니다.
	dataPtr->world = m_worldMatrix;
	dataPtr->view = m_viewMatrix;
	dataPtr->projection = m_projectionMatrix;

	// 상수 버퍼의 잠금을 풉니다.
	m_immediateContext->Unmap(m_matrixBuffer, 0);

	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	bufferNumber = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
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