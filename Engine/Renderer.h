#pragma once
#include <vector>
#include "RMesh.h"
#include "RRay.h"
#include "RMath.h"

class Renderer
{
public:
	~Renderer();
	void Initialize(HWND hwnd, int bufferWidth, int bufferHeight);
	void LoadLastResult();
	void RenderPbrScene(HWND hWnd, double deltaTime);
	void RenderPbrSceneWithCS(HWND hWnd, double deltaTime);
	void ShowResult(std::string fileName);
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

	struct IDXGISwapChain*							m_swapChain = nullptr; //DC 바꾸기
	struct ID3D11Device*							m_device = nullptr;
	struct ID3D11DeviceContext*						m_immediateContext = nullptr; //Dx용 DC
	struct ID3D11RenderTargetView*					m_renderTargetView = nullptr;
	struct ID3D11DepthStencilState*					m_depthStencilState = nullptr;
	struct ID3D11Texture2D*							m_depthStencilTexture = nullptr;
	struct ID3D11DepthStencilView*					m_depthStencilView = nullptr;
	int m_bufferWidth;
	int m_bufferHeight;
	RVector3* pixels = nullptr;
	int* hitCountOnPixel = nullptr;
};

