#pragma once
#include <windows.h>
#include <directxmath.h>
using namespace DirectX;
class Material
{
public:

	struct ConstBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	Material(const WCHAR* vsFileName, const WCHAR* psFileName) : m_vsFileName(vsFileName), m_psFileName(psFileName) {}
	bool IsInitialized() { return VertexShader && PixelShader && InputLayout; }
	bool Initialize(struct ID3D11Device* device, HWND hwnd);
	void Render(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual ConstBuffer* CreateConstBuffer(ID3D11DeviceContext* deviceContext, XMMATRIX projectionMatrix, XMMATRIX viewMatrix, class GameObject* gameObject);
	virtual ~Material();

	struct ID3D11VertexShader*		VertexShader	= nullptr;
	struct ID3D11PixelShader*		PixelShader		= nullptr;
	struct ID3D11InputLayout*		InputLayout		= nullptr;

private:
	const WCHAR * m_vsFileName;
	const WCHAR* m_psFileName;
	ID3D11Buffer* m_constBuffer;
};

