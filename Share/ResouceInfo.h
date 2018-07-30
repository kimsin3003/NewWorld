#include <vector>
#include <string.h>

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Vector2
{
	float x;
	float y;
};

class VertexInfo
{
public:
	Vector3 Pos;
	Vector2 UV;
	std::string ToString()
	{
	}
};

class MeshInfo
{
public:
	std::vector<VertexInfo>	Verticies;
	std::vector<unsigned int> Indicies;
	std::string ToString()
	{ 
		std::string ret;

		for (auto& vertexInfo : Verticies)
		{
			ret += vertexInfo.ToString();
		}

		for (auto& index : Indicies)
		{
			ret += index + ", ";
		}
	}
};