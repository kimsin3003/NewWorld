#pragma once
#include <vector>
#include <string>

class ResourceLoader
{
public:
	bool LoadFBX(std::string fbxFileName, std::vector<struct Vertex>& outVertexVector, std::vector<int>& outIndexVector);
};

