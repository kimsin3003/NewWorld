#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <map>
#include <memory>

using namespace DirectX;

class Renderer
{
public:
	bool Initialize(HWND hwnd, int winWidth, int winHeight);
	bool Tick(float deltaTime);
private:

	HRESULT InitDevice(HWND hwnd, int winWidth, int winHeight);
	bool SetVertexBuffer();
	bool SetIndexBuffer();
	bool SetVertexShader();
	IDXGISwapChain*									m_swapChain = nullptr; //DC 바꾸기
	ID3D11Device*									m_device = nullptr;
	ID3D11DeviceContext*							m_immediateContext = nullptr; //Dx용 DC
	ID3D11RenderTargetView*							m_renderTargetView = nullptr;

	ID3D11Buffer*									m_vertexBuffer = nullptr;
	ID3D11Buffer*									m_indexBuffer = nullptr;
	D3D_FEATURE_LEVEL								m_featureLevel = D3D_FEATURE_LEVEL_11_0;

};

