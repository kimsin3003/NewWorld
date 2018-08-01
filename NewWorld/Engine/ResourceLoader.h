#pragma once
#include <vector>
#include <string>
#include <fbxsdk.h>
#include "Mesh.h"
using namespace fbxsdk;

class ResourceLoader
{
public:
	bool LoadFBX(std::string fbxFileName, std::vector<Mesh*>& outMeshes);
private:
	void LoadVertexInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector);
	void LoadUVElement(FbxMesh* mesh, int controlPointIndex, int vertexCounter, Mesh* outMesh);
	void LoadUVInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector);
};
