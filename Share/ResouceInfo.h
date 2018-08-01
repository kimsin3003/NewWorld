#include <vector>
#include <string>
#include <sstream>
#include <iostream>

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

	void FromString(std::string str)
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
			ret += vertexInfo.ToString() + ',';
		}
		ret = ret.substr(0, ret.size() - 1);

		ret += "\n";

		for (auto& index : Indicies)
		{
			ret += index + ',';
		}
		ret = ret.substr(0, ret.size() - 1);
	}

	void FromString(std::string str)
	{
		std::string verticiesString;
		std::string indiciesString;
		std::istringstream stream(str);
		if (std::getline(stream, verticiesString, '\n'))
		{
			std::istringstream vertexStream(verticiesString);
			std::string token;
			while (std::getline(vertexStream, token, ','))
			{
				VertexInfo vertex;
				vertex.FromString(token);
				Verticies.push_back(vertex);
			}
		}
		if (std::getline(stream, indiciesString, '\n'))
		{
			std::istringstream vertexStream(verticiesString);
			std::string token;
			while (std::getline(vertexStream, token, ',')) Indicies.push_back(token);
		}
	}
};