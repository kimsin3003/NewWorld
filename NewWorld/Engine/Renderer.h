#pragma once
#include <vector>
#include "Mesh.h"


class Renderer
{
public:
	void Initialize(HWND hwnd, float bufferWidth, float bufferHeight);
	void Tick(class CameraManager* cameraManager, class ObjectManager* objectManager, float deltaTime);
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
	float m_bufferWidth;
	float m_bufferHeight;
};
