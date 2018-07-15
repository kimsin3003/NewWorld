#include "Texture.h"


bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
		bool result;
		int height, width;
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT hResult;
		unsigned int rowPitch;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		result = LoadTarga(filename, height, width);
		if (!result)
		{
			return false;
		}
		// Setup the description of the texture.
		textureDesc.Height = height;
		textureDesc.Width = width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create the empty texture.
		hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
		if (FAILED(hResult))
		{
			return false;
		}

		// Set the row pitch of the targa image data.
		rowPitch = (width * 4) * sizeof(unsigned char);
		// Copy the targa image data into the texture.
		deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);
		
			// Setup the shader resource view description.
			srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		// Create the shader resource view for the texture.
		hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
		if (FAILED(hResult))
		{
			return false;
		}

		// Generate mipmaps for this texture.
		deviceContext->GenerateMips(m_textureView);

		// Release the targa image data now that the image data has been loaded into the texture.
		delete[] m_targaData;
		m_targaData = 0;

		return true;
}

Texture::~Texture()
{
}
