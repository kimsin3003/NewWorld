#pragma once
#include <windows.h>
#include <string>
#include <directxmath.h>
using namespace DirectX;

struct TextureInfo
{
	enum TextureType
	{
		DIFFUSE,
		NORMAL,
	};
	std::wstring filename;
	TextureType type;
};

class Material
{
public:

	struct ConstBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	Material(const WCHAR* vsFileName, const WCHAR* psFileName, std::vector<TextureInfo> textures) : m_vsFileName(vsFileName), m_psFileName(psFileName), m_textures(textures) {}
	bool IsInitialized() { return m_vertexShader && m_pixelShader && m_inputLayout; }
	void Initialize(struct ID3D11Device* device);
	void Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void SetShaderParameters(struct ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	virtual ~Material();

private:

	struct	ID3D11VertexShader*			m_vertexShader		= nullptr;
	struct	ID3D11PixelShader*			m_pixelShader		= nullptr;
	struct	ID3D11InputLayout*			m_inputLayout		= nullptr;
	struct	ID3D11SamplerState*			m_sampleState		= nullptr;
	struct	ID3D11ShaderResourceView*	m_textureView		= nullptr;
	const	WCHAR*						m_vsFileName		= nullptr;
	const	WCHAR*						m_psFileName		= nullptr;
	struct	ID3D11Buffer*				m_constBuffer		= nullptr;
	std::vector<TextureInfo>			m_textures;
};

