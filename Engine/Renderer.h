#pragma once
#include <vector>
#include "Mesh.h"


class Renderer
{
public:
	bool Initialize(HWND hwnd, float winWidth, float winHeight);
	bool Render(HWND hwnd, class CameraManager* cameraManager, class ObjectManager* objectManager, float deltaTime);
private:
	struct MatrixBuffer
	{ 
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	bool InitDevice(HWND hwnd);
	bool SetRenderTargets();
	void SetViewports();
	void SetMVPMatrix(class GameObject* gameObject);
	struct IDXGISwapChain*									m_swapChain = nullptr; //DC 바꾸기
	struct ID3D11Device*										m_device = nullptr;
	struct ID3D11DeviceContext*								m_immediateContext = nullptr; //Dx용 DC
	struct ID3D11RenderTargetView*							m_renderTargetView = nullptr;

	float m_winWidth;
	float m_winHeight;
};

