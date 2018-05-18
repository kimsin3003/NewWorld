#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include <time.h>
#include <dxgi.h>
#include <D3D11.h>
#include <DirectXMath.h>
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
	HRESULT hr = m_swapChain->GetBuffer(0,							 // 후면 버퍼 인덱스, 여러개일 때 중요, 지금은 1개 이므로 0.
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
	//여기에 원래는 depthbuffer의 뷰를 넣어야함.

	D3D11_VIEWPORT	vp; // 한 윈도우에 여러개의 화면을 그릴 수 있는 박스들. ex)레이싱 게임 2player모드.
	vp.Width = winWidth;
	vp.Height = winHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // 뷰포트가 여러개면 개수와, 배열의 주소를 넣는다.
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
// void Renderer::CreateDepthStencilState()
// {
// 	D3D11_DEPTH_STENCIL_DESC    depthStencilDesc;
// 	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
// 
// 	depthStencilDesc.DepthEnable = true;    // Depth Test 활성화
// 	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // Depth 쓰기 기능 활성화.
// 	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;  	// Z 값이 작으면 통과. 즉 그린다.
// 
// 	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
// 	m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 1);
// }
// 
// void Renderer::CreateDepthStencilTexture()
// {
// 
// 	//Create depth stencil texture
// 	D3D11_TEXTURE2D_DESC descDepth;
// 	ZeroMemory(&descDepth, sizeof(descDepth));
// 	descDepth.Width = m_width;
// 	descDepth.Height = m_height;
// 	descDepth.MipLevels = 1; //밉맵은 쓰지 않는다.
// 	descDepth.ArraySize = 1; //따라서 하나의 스탠실 버퍼 사용
// 	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
// 	descDepth.SampleDesc.Count = 1;
// 	descDepth.SampleDesc.Quality = 0;
// 	descDepth.Usage = D3D11_USAGE_DEFAULT;
// 	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
// 	descDepth.CPUAccessFlags = 0;
// 	descDepth.MiscFlags = 0;
// 	m_device->CreateTexture2D(&descDepth, NULL, &m_depthStencilTexture);
// 
// 	//Create depth stencil view
// 
// 	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
// 	ZeroMemory(&descDSV, sizeof(descDSV));
// 	descDSV.Format = descDepth.Format;
// 
// 	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
// 	descDSV.Texture2D.MipSlice = 0;
// 	m_device->CreateDepthStencilView(m_depthStencilTexture, &descDSV, &m_depthStencilView);
// 
// 
// 	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
// }
// 
// void Renderer::CreateMatrices()
// {
// 	float fieldOfView = (float) 3.14 / 4.0f; 
// 	float screenAspect = (float)m_width / (float)m_height;
// 	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);
// 	m_worldMatrix = DirectX::XMMatrixIdentity();
// 	m_viewMatrix = DirectX::XMMatrixOrthographicLH(m_width, m_height, 0.1f, 1000.0f);
// }
//
// bool Renderer::SetConstantBuffers()
// {
// 	HRESULT result;
// 	D3D11_MAPPED_SUBRESOURCE mappedResource;
// 	MatrixBuffer* dataPtr;
// 	unsigned int bufferNumber;
// 
// 
// 	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다.
// 	m_worldMatrix = DirectX::XMMatrixTranspose(m_worldMatrix);
// 	m_viewMatrix = DirectX::XMMatrixTranspose(m_viewMatrix);
// 	m_projectionMatrix = DirectX::XMMatrixTranspose(m_projectionMatrix);
// 
// 
// 	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
// 	result = m_immediateContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
// 	if (FAILED(result))
// 	{
// 		return false;
// 	}
// 
// 	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
// 	dataPtr = (MatrixBuffer*)mappedResource.pData;
// 
// 	// 상수 버퍼에 행렬을 복사합니다.
// 	dataPtr->world = m_worldMatrix;
// 	dataPtr->view = m_viewMatrix;
// 	dataPtr->projection = m_projectionMatrix;
// 
// 	// 상수 버퍼의 잠금을 풉니다.
// 	m_immediateContext->Unmap(m_matrixBuffer, 0);
// 
// 	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
// 	bufferNumber = 0;
// 
// 	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
// 	m_immediateContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
// 
// 	return true;
// 
// }


