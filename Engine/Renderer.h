#pragma once
#include <vector>
#include "Mesh.h"


class Renderer
{
public:
	bool Initialize(HWND hwnd, int winWidth, int winHeight);
	bool Render(HWND hwnd, class ObjectManager* const objectManager, float deltaTime);
private:

	bool InitDevice(HWND hwnd, int winWidth, int winHeight);
	bool SetRenderTargets();
	void SetViewports(float winWidth, float winHeight);
	struct IDXGISwapChain*									m_swapChain = nullptr; //DC 바꾸기
	struct ID3D11Device*										m_device = nullptr;
	struct ID3D11DeviceContext*								m_immediateContext = nullptr; //Dx용 DC
	struct ID3D11RenderTargetView*							m_renderTargetView = nullptr;
};

