#pragma once
#include <vector>
#include <string>

class JsonUtility
{
public:
	static void WriteModelToFIle(class RGameObject* gameObject, std::string filename);
	static bool ReadModelFromFile(class RGameObject* outGameObject, std::string filename);
};

