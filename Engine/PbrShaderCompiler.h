#pragma once
#include <string>
class PbrShaderCompiler
{
public:
	static class ID3D11ComputeShader* GetComputeShader(struct ID3D11Device* device, std::wstring fileName);
};

