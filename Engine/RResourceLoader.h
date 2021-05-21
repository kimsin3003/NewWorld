#pragma once
#include <vector>
#include <string>

class RResourceLoader
{
public:
	static class RMesh* LoadFile(std::string fileName);
};