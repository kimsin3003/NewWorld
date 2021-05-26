#pragma once
#include <windows.h>
#include <string>
#include <directxmath.h>
#include <vector>
using namespace DirectX;

struct RTexture
{
	enum RTextureType
	{
		DIFFUSE,
		NORMAL,
		SPECULAR
	};
	std::string filePath;
	RTextureType type;
	struct ID3D11ShaderResourceView* rv;
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
		XMMATRIX world;
	};
	RMaterial(const WCHAR* vsFileName, const WCHAR* psFileName, std::string directory) : m_vsFileName(vsFileName), m_psFileName(psFileName), m_directory(directory) {}
	bool IsInitialized() { return m_vertexShader && m_pixelShader && m_inputLayout; }
	void Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	
	void Load(const struct aiScene* scene, const struct aiMaterial* aiMat);

	void LoadTextureType(const struct aiScene* scene, const struct aiMaterial* aiMat, RTexture::RTextureType type);

	virtual ~RMaterial();

private:
	void SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	void SetVSConstBuffer(XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix);
	void SetPSConstBuffer();
	ID3D11ShaderResourceView* CreateShaderResourceView(const struct aiScene* scene, std::string fileName);
private:

	struct	ID3D11VertexShader*			m_vertexShader		= nullptr;
	struct	ID3D11PixelShader*			m_pixelShader		= nullptr;
	struct	ID3D11InputLayout*			m_inputLayout		= nullptr;
	struct	ID3D11SamplerState*			m_sampleState		= nullptr;
	const	WCHAR*						m_vsFileName		= nullptr;
	const	WCHAR*						m_psFileName		= nullptr;
	struct	ID3D11Buffer*				m_vsConstBuffer		= nullptr;
	struct	ID3D11Buffer*				m_psConstBuffer		= nullptr;
	std::vector<RTexture*>				m_textures;
	static std::vector<RTexture*>		m_loadedTextures;
	std::string m_directory;
};