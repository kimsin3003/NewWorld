#pragma once
#include <vector>
#include <string>
#include "ResouceInfo.h"

class JsonUtility
{
public:
	static void WriteFBXToFIle(std::vector<MeshInfo*> infos, std::string filename);
};

