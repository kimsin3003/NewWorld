#pragma once
#include <vector>
#include <fbxsdk.h>
#include "ResouceInfo.h"
using namespace fbxsdk;

class ResourceConverter
{
public:
	bool LoadFBX(std::string fbxFileName, std::vector<MeshInfo*>& outMeshes);
private:
	void LoadVertexInformation(FbxMesh* pMesh, std::vector<VertexInfo>& outVertexVector, std::vector<unsigned int>& outIndexVector);
	void LoadUVInformation(FbxMesh* pMesh, std::vector<VertexInfo>& outVertexVector);
};

