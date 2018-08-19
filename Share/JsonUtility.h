#pragma once
#include <vector>
#include <string>

class JsonUtility
{
public:
	static void WriteModelToFIle(class ModelInfo* modelInfo, std::string filename);
	static void ReadModelFromFile(class ModelInfo* outModelInfo, std::string filename);
};

