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
	bool IsInitialized() { return m_vertexShader && m_pixelShader && m_inputLayout; }
	void Initialize(struct ID3D11Device* device);
	void Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	virtual ~Material();

private:

	struct	ID3D11VertexShader*			m_vertexShader	= nullptr;
	struct	ID3D11PixelShader*			m_pixelShader	= nullptr;
	struct	ID3D11InputLayout*			m_inputLayout	= nullptr;
	struct	ID3D11SamplerState*			m_sampleState	= nullptr;
	class	Texture*					m_texture		= nullptr;
	const	WCHAR*						m_vsFileName	= nullptr;
	const	WCHAR*						m_psFileName	= nullptr;
	struct	ID3D11Buffer*				m_constBuffer	= nullptr;
};

