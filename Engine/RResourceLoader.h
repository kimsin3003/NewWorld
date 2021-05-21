#pragma once
#include <vector>
#include <string>

class RResourceLoader
{
public:
	static bool LoadFile(std::string fileName, class RGameObject* gameObj);
};