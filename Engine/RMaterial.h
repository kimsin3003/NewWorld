#pragma once
#include <windows.h>
#include <string>
#include <directxmath.h>
using namespace DirectX;

struct RTexture
{
	enum TextureType
	{
		DIFFUSE,
		NORMAL,
	};
	std::wstring filename;
	TextureType type;
};

class RMaterial
{
public:

	__declspec(align(16))
	struct VSConstBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMFLOAT4 lightPosition;
	};

	__declspec(align(16))
	struct PSConstBuffer //sizeof해서 16의 배수가 되어야한다.
	{
		XMFLOAT4 lightIntensity; //앞의 4byte만 쓰자.
		XMFLOAT4 ambientColor;
	};
	RMaterial(const WCHAR* vsFileName, const WCHAR* psFileName, std::vector<RTexture> textures) : m_vsFileName(vsFileName), m_psFileName(psFileName), m_textures(textures) {}
	bool IsInitialized() { return m_vertexShader && m_pixelShader && m_inputLayout; }
	void Initialize(struct ID3D11Device* device);
	void Render(struct ID3D11Device* device, struct ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void SetShaderParameters(struct ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	void SetVSConstBuffer(XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, ID3D11DeviceContext* deviceContext);
	void SetPSConstBuffer(ID3D11DeviceContext* deviceContext);

	virtual ~RMaterial();

private:

	struct	ID3D11VertexShader*			m_vertexShader		= nullptr;
	struct	ID3D11PixelShader*			m_pixelShader		= nullptr;
	struct	ID3D11InputLayout*			m_inputLayout		= nullptr;
	struct	ID3D11SamplerState*			m_sampleState		= nullptr;
	struct	ID3D11ShaderResourceView*	m_textureView		= nullptr;
	const	WCHAR*						m_vsFileName		= nullptr;
	const	WCHAR*						m_psFileName		= nullptr;
	struct	ID3D11Buffer*				m_vsConstBuffer		= nullptr;
	struct	ID3D11Buffer*				m_psConstBuffer		= nullptr;
	std::vector<RTexture>				m_textures;
};

