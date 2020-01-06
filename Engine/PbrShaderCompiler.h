#pragma once
#include <string>
#include <windows.h>
class PbrShaderCompiler
{
public:
	static struct ID3D11ComputeShader* GetComputeShader(struct ID3D11Device* device, std::wstring fileName);
	template <class T>
	static HRESULT CreateStructuredBuffer(
		struct ID3D11Device*               pd3dDevice,
		const int                   iNumElements,
		const bool                  isCpuWritable,
		const bool                  isGpuWritable,
		struct ID3D11Buffer**              ppBuffer,
		struct ID3D11ShaderResourceView**  ppSRV,
		struct ID3D11UnorderedAccessView** ppUAV,
		const T*                    pInitialData = NULL);
};

