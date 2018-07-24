#pragma once
#include <vector>
#include <fbxsdk.h>
#include "Mesh.h"
using namespace fbxsdk;

class ResourceLoader
{
public:
	bool LoadFBX(std::string fbxFileName, std::vector<Mesh*>& outMeshes);
private:
	void LoadVertexInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector);
	void LoadUVInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector);
};
