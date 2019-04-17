#include "PbrShaderCompiler.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Logger.h"


class ID3D11ComputeShader* PbrShaderCompiler::GetComputeShader(ID3D11Device* device, std::wstring fileName)
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
