#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <time.h>
#include <dxgi.h>
#include <D3D11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <map>
#include "RRenderer.h"
#include "RMaterial.h"
#include "RGameObject.h"
#include "RObjectManager.h"
#include "RCamera.h"
#include "RCameraManager.h"
#include "Logger.h"

void RRenderer::Initialize(HWND hwnd, float bufferWidth, float bufferHeight)
{
	m_bufferWidth = bufferWidth;
	m_bufferHeight = bufferHeight;

	if (!InitDevice(hwnd))
		return;
	SetDepthStencilState();
	SetRenderTargets();
	SetViewports();
}


void RRenderer::Tick(class RCameraManager* cameraManager, class RObjectManager* objectManager, double deltaTime)
{
	static double elapsedTime = 0;
	elapsedTime += deltaTime;
	if (elapsedTime < 1.0f / 60)
		return;
	elapsedTime = 0;

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_immediateContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	auto gameObjectPool = objectManager->GetGameObjectPool();

	RCamera* currentCamera = cameraManager->GetCurrentCamera();
	for (RGameObject* gameObject : gameObjectPool)
	{
		for (RMesh* mesh : gameObject->Meshes)
		{
			if (mesh)
			{
				RMaterial* mat = mesh->Mat;
				if (mat)
				{
					mat->Render(gameObject->GetWorldMatrix(), currentCamera->GetViewMatrix(), currentCamera->GetProjectionMatrix());
				}
				mesh->Render(m_device, m_immediateContext);
			}
		}
	}

	m_swapChain->Present(0, 0);

	return;
}


bool RRenderer::InitDevice(HWND hwnd)
{
	HRESULT hr = S_OK;

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

	sd.BufferDesc.Width = m_bufferWidth;
	sd.BufferDesc.Height = m_bufferHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//����� ����
	sd.BufferDesc.RefreshRate.Numerator = 60;	//����
	sd.BufferDesc.RefreshRate.Denominator = 1;	//�и�

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ����� ������
	sd.OutputWindow = hwnd;	//����� ������ �ڵ�
	sd.SampleDesc.Count = 1;	//multisampling(���� ǥ��ȭ) 1�̸� �Ѱ��� ����� ����ǥ��ȭ --> ����ǥ��ȭ ���ϰڴٴ� ��.
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

void RRenderer::SetRenderTargets()
{
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr = m_swapChain->GetBuffer(0,							 // �ĸ� ���� �ε���, �������� �� �߿�, ������ 1�� �̹Ƿ� 0.
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
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	m_device->CreateDepthStencilView(m_depthStencilTexture, &descDSV, &m_depthStencilView);

	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void RRenderer::SetViewports()
{
	//���⿡ ������ depthbuffer�� �並 �־����.

	D3D11_VIEWPORT	vp; // �� �����쿡 �������� ȭ���� �׸� �� �ִ� �ڽ���. ex)���̽� ���� 2player���.
	vp.Width = m_bufferWidth;
	vp.Height = m_bufferHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // ����Ʈ�� �������� ������, �迭�� �ּҸ� �ִ´�.
}

void RRenderer::SetDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC    depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;    // Depth Test Ȱ��ȭ
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // Depth ���� ��� Ȱ��ȭ.
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;  	// Z ���� ������ ���. �� �׸���.

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