#pragma once
class Texture
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	~Texture();
};

