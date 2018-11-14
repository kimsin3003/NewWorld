#pragma once
#include <vector>
#include <fbxsdk.h>
#include <string>
using namespace fbxsdk;

class ResourceLoader
{
public:
	static bool LoadFBX(class RGameObject* outMeshes, std::string fbxFileName);
private:
	static void LoadVertexInformation(FbxMesh* pMesh, std::vector<struct RVertex>& outVertexVector, std::vector<unsigned int>& outIndexVector);
	static void LoadUVInformation(FbxMesh* pMesh, std::vector<struct RVertex>& outVertexVector);
	static void LoadNormalInformation(FbxMesh* pMesh, std::vector<RVertex>& outVertexVector);
};