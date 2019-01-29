#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "Gdiplus.lib")
#include <time.h>
#include <dxgi.h>
#include <D3D11.h>
#include <thread>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <map>
#include "Renderer.h"
#include "RMaterial.h"
#include "RGameObject.h"
#include "RObjectManager.h"
#include "RCamera.h"
#include "RCameraManager.h"
#include "Logger.h"
#include "PathTracer.h"
#include "RContext.h"
#include "bitmap_image.h"

Renderer::~Renderer()
{
	m_swapChain->Release();
	m_device->Release();
	m_immediateContext->Release();
	m_renderTargetView->Release();
	m_depthStencilState->Release();
	m_depthStencilTexture->Release();
	m_depthStencilView->Release();
	delete[] pixels;
}

void Renderer::Initialize(HWND hwnd, int bufferWidth, int bufferHeight)
{
	m_bufferWidth = bufferWidth;
	m_bufferHeight = bufferHeight;

	if (!InitDevice(hwnd))
		return;
	SetDepthStencilState();
	SetRenderTargets();
	SetViewports();
	pixels = new RVector3[m_bufferWidth * m_bufferHeight];
	srand(time(NULL));
}

void Renderer::RenderPbrScene(HWND hWnd, double deltaTime)
{
	static int sampleCount = 0;
	unsigned numOfThread = std::thread::hardware_concurrency() * 2 + 1;

	std::vector<std::thread> threads;
	threads.reserve(numOfThread);
	for (int i = 0; i < numOfThread; i++)
	{
		int withOfOneThread = m_bufferWidth / numOfThread;
		int screenWidth = m_bufferWidth;
		int screenHeight = m_bufferHeight;
		threads.emplace_back([=] {
			for (int m = i * withOfOneThread; m < (i+1) * withOfOneThread; m++)
			{
				for (int n = 0; n < screenHeight; n++)
				{
					RRay ray(m, n, screenWidth, screenHeight);
					RVector3 pixelColor = PathTracer::GetPixelColor(ray, ObjectManager->GetGameObjectPool(), 0);
					pixels[m * screenHeight + n] += pixelColor;
				}
			}
		});
	}
	for (int i = 0; i < numOfThread; i++)
	{
		threads[i].join();
	}
	sampleCount++;
	ShowResult(sampleCount, "result.bmp");
// 	if (sampleCount >= 10)
// 		::MessageBox(hWnd, L"End", NULL, NULL);
}

void Renderer::ShowResult(int sampleCount, std::string fileName)
{

	bitmap_image image(m_bufferWidth, m_bufferHeight);

	for (int m = 0; m < m_bufferWidth; m++)
	{
		for (int n = 0; n < m_bufferHeight; n++)
		{
			RVector3 pixelColor = pixels[m * m_bufferHeight + n] / sampleCount;
			INT r = pixelColor.x >= 1 ? 255 : pixelColor.x * 255;
			INT g = pixelColor.y >= 1 ? 255 : pixelColor.y * 255;
			INT b = pixelColor.z >= 1 ? 255 : pixelColor.z * 255;


			DWORD rgbColor = RGB(r, g, b);
			//SetPixel(hdc, m, n, rgbColor);
			unsigned char red = GetRValue(rgbColor);
			unsigned char green = GetGValue(rgbColor);
			unsigned char blue = GetBValue(rgbColor);
			image.set_pixel(m, n, red, green, blue);
		}
	}

	image.save_image(fileName);
}

void Renderer::Tick(double deltaTime)
{
	static double elapsedTime = 0;
	elapsedTime += deltaTime;
	if (elapsedTime < 1.0f / 60)
		return;
	elapsedTime = 0;

	float clearColor[4] = { 0, 1.0f, 0, 1.0f };
	m_immediateContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	auto gameObjectPool = ObjectManager->GetGameObjectPool();

	RCamera* currentCamera = CameraManager->GetCurrentCamera();
	for (RGameObject* gameObject : gameObjectPool)
	{
		for (RMesh* mesh : gameObject->Meshes)
		{
			if (mesh)
			{
				RMaterial* mat = mesh->Mat;
				if (mat)
				{
					mat->Render(m_device, m_immediateContext, gameObject->GetWorldMatrix(), currentCamera->GetViewMatrix(), currentCamera->GetProjectionMatrix());
				}
				mesh->Render(m_device, m_immediateContext);
			}
		}
	}

 	m_swapChain->Present(0, 0);

	return;
}


bool Renderer::InitDevice(HWND hwnd)
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

	sd.BufferDesc.Width = m_bufferWidth;
	sd.BufferDesc.Height = m_bufferHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//백버퍼 포맷
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferDesc.RefreshRate.Numerator = 60;	//분자
	sd.BufferDesc.RefreshRate.Denominator = 1;	//분모

	//sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 백버퍼 랜더링
	sd.OutputWindow = hwnd;	//출력할 윈도우 핸들
	sd.SampleDesc.Count = 1;	//multisampling(다중 표본화) 1이면 한가지 색깔로 다중표본화 --> 다중표본화 안하겠다는 뜻.
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
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
		&featureLevel,
		&m_immediateContext);

	if (FAILED(hr))
	{
		Logger::Log(hr);
		return false;
	}
	return true;
}

void Renderer::SetRenderTargets()
{
	ID3D11Texture2D* backBuffer = NULL;
	HRESULT hr = m_swapChain->GetBuffer(0,							 // 후면 버퍼 인덱스, 여러개일 때 중요, 지금은 1개 이므로 0.
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&backBuffer);

	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}

	hr = m_device->CreateRenderTargetView(backBuffer,
		NULL,
		&m_renderTargetView);

	if (FAILED(hr))
	{
		Logger::Log(hr);
		return;
	}
	backBuffer->Release();

	//Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_bufferWidth;
	descDepth.Height = m_bufferHeight;
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
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(m_depthStencilTexture, &descDSV, &m_depthStencilView);

	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void Renderer::SetViewports()
{
	//여기에 원래는 depthbuffer의 뷰를 넣어야함.

	D3D11_VIEWPORT	vp; // 한 윈도우에 여러개의 화면을 그릴 수 있는 박스들. ex)레이싱 게임 2player모드.
	vp.Width = m_bufferWidth;
	vp.Height = m_bufferHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // 뷰포트가 여러개면 개수와, 배열의 주소를 넣는다.
}

void Renderer::SetDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC    depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;    // Depth Test 활성화
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // Depth 쓰기 기능 활성화.
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;  	// Z 값이 작으면 통과. 즉 그린다.

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 1);
}