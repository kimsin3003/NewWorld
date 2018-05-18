#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <time.h>
#include <dxgi.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include <map>
#include "Renderer.h"
#include "Material.h"
#include "GameObject.h"
#include "ObjectManager.h"

bool Renderer::Initialize(HWND hwnd, int winWidth, int winHeight)
{
	if (!InitDevice(hwnd, winWidth, winHeight))
		return false;
	if (!SetRenderTargets())
		return false;
	SetViewports(winWidth, winHeight);

	return true;
}


bool Renderer::InitDevice(HWND hwnd, int winWidth, int winHeight)
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
		return false;

	return true;
}

bool Renderer::SetRenderTargets()
{
	ID3D11Texture2D* pBackBuffer = NULL;
	HRESULT hr = m_swapChain->GetBuffer(0,							 // �ĸ� ���� �ε���, �������� �� �߿�, ������ 1�� �̹Ƿ� 0.
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return false;

	hr = m_device->CreateRenderTargetView(pBackBuffer,
		NULL,
		&m_renderTargetView);

	if (FAILED(hr))
		return false;

	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
	pBackBuffer->Release();

	return true;
}

void Renderer::SetViewports(float winWidth, float winHeight)
{
	//���⿡ ������ depthbuffer�� �並 �־����.

	D3D11_VIEWPORT	vp;
	vp.Width = winWidth;
	vp.Height = winHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // ����Ʈ�� �������� ������, �迭�� �ּҸ� �ִ´�.
}

bool Renderer::Render(HWND hwnd, class ObjectManager* const objectManager, float deltaTime)
{
	float ClearColor[4] = { 0.0f, 0.3f, 0.3f, 1.0f };
	m_immediateContext->ClearRenderTargetView(m_renderTargetView, ClearColor);

	auto indiciesOnUse = objectManager->GetIndiciesOnUse();
	auto gameObjectPool = objectManager->GetGameObjectPool();
	for (int index : indiciesOnUse)
	{
		GameObject* gameObject = gameObjectPool[index];
		Mesh* mesh = gameObject->Mesh;
		if (mesh)
		{
			Material* mat = mesh->Mat;
			if (!mat->IsInitialized())
				mat->Initialize(m_device, hwnd);
			if (!mesh->IsInitialized())
				mesh->Initialize(m_device);

			if (mat->VertexShader)
			{
				m_immediateContext->VSSetShader(mat->VertexShader, 0, 0);
			}

			if (mat->PixelShader)
			{
				m_immediateContext->PSSetShader(mat->PixelShader, 0, 0);
			}
			m_immediateContext->IASetInputLayout(mat->InputLayout);

			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			ID3D11Buffer* const buffers[] = { mesh->GetVertexBuffer() };
			m_immediateContext->IASetVertexBuffers(0, 1, buffers, &stride, &offset);
			m_immediateContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			m_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_immediateContext->DrawIndexed(3, 0, 0);
		}

	}
	m_swapChain->Present(0, 0);
	return true;
}


