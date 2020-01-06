#include "PbrShaderCompiler.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Logger.h"


struct ID3D11ComputeShader* PbrShaderCompiler::GetComputeShader(ID3D11Device* device, std::wstring fileName)
{
	ID3D10Blob *csBlob, *errorblob;
	HRESULT hr = D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", NULL, NULL, &csBlob, &errorblob);
	if (FAILED(hr))
	{
		if (errorblob)
		{
			Logger::Log((char*)errorblob->GetBufferPointer());
			errorblob->Release();
		}

		if (csBlob)
			csBlob->Release();

		return nullptr;
	}

	ID3D11ComputeShader* computeShader = nullptr;
	// encapsulate both shaders into shader objects
	hr = device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), 0, &computeShader);
	if (FAILED(hr))
	{
		Logger::Log("ComputeShader 생성 오류");
		return nullptr;
	}
	return computeShader;
}

template <class T>
HRESULT PbrShaderCompiler::CreateStructuredBuffer(
	ID3D11Device*               pd3dDevice,
	const int                   iNumElements,
	const bool                  isCpuWritable,
	const bool                  isGpuWritable,
	ID3D11Buffer**              ppBuffer,
	ID3D11ShaderResourceView**  ppSRV,
	ID3D11UnorderedAccessView** ppUAV,
	const T*                    pInitialData)
{
	HRESULT hr = S_OK;

	assert(pd3dDevice != NULL);
	assert(ppBuffer != NULL);
	assert(ppSRV != NULL);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = iNumElements * sizeof(T);

	if ((!isCpuWritable) && (!isGpuWritable))
	{
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	}
	else if (isCpuWritable && (!isGpuWritable))
	{
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}
	else if ((!isCpuWritable) && isGpuWritable)
	{
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.BindFlags = (D3D11_BIND_SHADER_RESOURCE |
			D3D11_BIND_UNORDERED_ACCESS);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	}
	else
	{
		assert((!(isCpuWritable && isGpuWritable)));
	}

	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(T);

	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory((&bufferInitData), sizeof(bufferInitData));
	bufferInitData.pSysMem = pInitialData;
	V_RETURN(pd3dDevice->CreateBuffer((&bufferDesc),
		(pInitialData) ? (&bufferInitData) : NULL,
		ppBuffer));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = iNumElements;
	V_RETURN(pd3dDevice->CreateShaderResourceView(*ppBuffer, &srvDesc, ppSRV));

	if (isGpuWritable)
	{
		assert(ppUAV != NULL);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory((&uavDesc), sizeof(uavDesc));
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = iNumElements;
		V_RETURN(pd3dDevice->CreateUnorderedAccessView(*ppBuffer, &uavDesc, ppUAV));
	}

	return hr;
}


