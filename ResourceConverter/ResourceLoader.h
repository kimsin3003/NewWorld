#pragma once
#include <vector>
#include <fbxsdk.h>
#include <string>
using namespace fbxsdk;

class ResourceLoader
{
public:
	bool LoadFBX(std::string fbxFileName, class ModelInfo* outMeshes);
private:
	void LoadVertexInformation(FbxMesh* pMesh, std::vector<class VertexInfo>& outVertexVector, std::vector<unsigned int>& outIndexVector);
	void LoadUVInformation(FbxMesh* pMesh, std::vector<class VertexInfo>& outVertexVector);
};

