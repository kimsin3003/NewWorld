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
	struct MatrixBuffer
	{ 
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	HRESULT InitDevice(HWND hwnd, int winWidth, int winHeight);
	void CreateDepthStencilState();
	void CreateDepthStencilTexture();
	void CreateMatrices();
	bool SetVertexBuffer();
	bool SetIndexBuffer();
	bool SetShaders();

	bool SetConstantBuffers();
	int					m_width;
	int					m_height;
	XMMATRIX			m_projectionMatrix;
	XMMATRIX			m_worldMatrix;
	XMMATRIX			m_viewMatrix; 
	ID3D11Buffer*		m_matrixBuffer = nullptr;

	IDXGISwapChain*									m_swapChain = nullptr; //DC 바꾸기
	ID3D11Device*									m_device = nullptr;
	ID3D11DeviceContext*							m_immediateContext = nullptr; //Dx용 DC
	ID3D11RenderTargetView*							m_renderTargetView = nullptr;

	ID3D11DepthStencilState*						m_depthStencilState = nullptr; 
	ID3D11Texture2D*								m_depthStencilTexture = nullptr;
	ID3D11DepthStencilView*							m_depthStencilView = nullptr;

	ID3D11Buffer*									m_vertexBuffer = nullptr;
	ID3D11Buffer*									m_indexBuffer = nullptr;
	D3D_FEATURE_LEVEL								m_featureLevel = D3D_FEATURE_LEVEL_11_0;

};

