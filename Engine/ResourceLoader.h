#pragma once
#include <vector>
#include <string>

class ResourceLoader
{
public:
	bool LoadFBX(std::string fbxFileName, std::vector<struct Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector);

private:
	void LoadVertexInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector);
	void LoadUVInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector);
};

