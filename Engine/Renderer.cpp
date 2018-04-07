#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <time.h>
#include <dxgi.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include "Declares.h"
#include "Renderer.h"


bool Renderer::Initialize(HWND hwnd, int winWidth, int winHeight)
{
	InitDevice(hwnd, winWidth, winHeight);

	//CreateShader();
// 	CreateDepthStencilTexture();
// 	CreateDepthStencilState();
// 	CreateBlendState();

	return true;
}

HRESULT Renderer::InitDevice(HWND hwnd, int winWidth, int winHeight)
{
	HRESULT hr = S_OK;

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

	if (FAILED(hr))
		return hr;

	//여기에 원래는 depthbuffer의 뷰를 넣어야함.

	D3D11_VIEWPORT	vp; // 한 윈도우에 여러개의 화면을 그릴 수 있는 박스들. ex)레이싱 게임 2player모드.
	vp.Width = winWidth;
	vp.Height = winHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // 뷰포트가 여러개면 개수와, 배열의 주소를 넣는다.


	if (!SetVertexBuffer())
		return false;
	if (!SetIndexBuffer())
		return false;
	if (!SetVertexShader())
		return false;
	return hr;

}

bool Renderer::Tick(float deltaTime)
{
	//m_immediateContext->OMSetDepthStencilState(m_depthStencilStateForNormalModel, 0);
	m_immediateContext->OMSetBlendState(0, 0, 0xffffffff);

	m_immediateContext->DrawIndexed(3, 0, 0);

	m_swapChain->Present(0, 0);
	return true;
}

HRESULT Renderer::SetVertexShader()
{
	// load and compile the two shaders
	ID3D10Blob *vsBlob, *psBlob, *errorblob;
	HRESULT hr = D3DCompileFromFile(L"Engine/Default.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "vs_5_0", NULL, NULL, &vsBlob, &errorblob);
	if (FAILED(hr))
		return hr;
	hr = D3DCompileFromFile(L"Engine/Default.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "vs_5_0", NULL, NULL, &psBlob, &errorblob);
	if (FAILED(hr))
		return hr;

	// encapsulate both shaders into shader objects
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &vertexShader);
	m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &pixelShader);

	// set the shader objects
	m_immediateContext->VSSetShader(vertexShader, 0, 0);
	m_immediateContext->PSSetShader(pixelShader, 0, 0);

	return hr;

// 	create the input layout object
// 		D3D11_INPUT_ELEMENT_DESC ied[] =
// 		{
// 			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 		};
// 	
// 		ID3D11InputLayout* inputLayout = nullptr;
// 		m_device->CreateInputLayout(ied, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
// 		m_immediateContext->IASetInputLayout(inputLayout);
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
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.0f, 0.0f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f, 0.0f, 0.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.0f, 0.5f, 0.0f),
	};

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * 3;
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
	unsigned int indices[] = { 0, 1, 2 };

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
	m_immediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	return true;
}