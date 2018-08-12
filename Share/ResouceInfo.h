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
};

class MeshInfo
{
public:
	std::vector<VertexInfo>	Verticies;
	std::vector<unsigned int> Indicies;
};