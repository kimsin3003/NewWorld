#pragma once
#include <vector>
#include "RMesh.h"
#include "RRay.h"

class Renderer
{
public:
	void Initialize(HWND hwnd, int bufferWidth, int bufferHeight);
	void RenderPbrScene(HWND hWnd, double deltaTime);
	void Tick(double deltaTime);
private:
	struct MatrixBuffer
	{ 
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	bool InitDevice(HWND hwnd);
	void SetRenderTargets();
	void SetViewports();
	void SetDepthStencilState();

	struct IDXGISwapChain*							m_swapChain = nullptr; //DC �ٲٱ�
	struct ID3D11Device*							m_device = nullptr;
	struct ID3D11DeviceContext*						m_immediateContext = nullptr; //Dx�� DC
	struct ID3D11RenderTargetView*					m_renderTargetView = nullptr;
	struct ID3D11DepthStencilState*					m_depthStencilState = nullptr;
	struct ID3D11Texture2D*							m_depthStencilTexture = nullptr;
	struct ID3D11DepthStencilView*					m_depthStencilView = nullptr;
	int m_bufferWidth;
	int m_bufferHeight;
};

