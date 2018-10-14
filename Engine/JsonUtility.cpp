#include "JsonUtility.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"
#include "RMesh.h"
#include "RGameObject.h"
#include <cstdio>
#include <fstream>

using namespace rapidjson;


static Value MeshInfoToJsonValue(class RMesh* mesh, Document::AllocatorType& allocator)
{
	Value meshData(kObjectType);
	meshData.AddMember("name", Value().SetString("mesh"), allocator);
	
	Value verticies(kArrayType);
	for (RVertex vertexInfo : mesh->Verticies)
	{
		Value pos(kObjectType);
		pos.AddMember("x", vertexInfo.Pos.x, allocator);
		pos.AddMember("y", vertexInfo.Pos.y, allocator);
		pos.AddMember("z", vertexInfo.Pos.z, allocator);

		Value uv(kObjectType);
		uv.AddMember("u", vertexInfo.UV.x, allocator);
		uv.AddMember("v", vertexInfo.UV.y, allocator);

		Value normal(kObjectType);
		normal.AddMember("x", vertexInfo.Normal.x, allocator);
		normal.AddMember("y", vertexInfo.Normal.y, allocator);
		normal.AddMember("z", vertexInfo.Normal.z, allocator);

		Value vertex(kObjectType);
		vertex.AddMember("pos", pos, allocator);
		vertex.AddMember("uv", uv, allocator);
		vertex.AddMember("normal", normal, allocator);

		verticies.PushBack(vertex, allocator);
	}
	meshData.AddMember("verticies", verticies, allocator);

	Value indicies(kArrayType);
	for (unsigned int index : mesh->Indicies)
	{
		indicies.PushBack(index, allocator);
	}
	meshData.AddMember("indicies", indicies, allocator);

	return meshData;
}

void JsonUtility::WriteModelToFIle(RGameObject* gameObject, std::string filename)
{
	// 1. Parse a JSON string into DOM.
	Document document;
	document.SetObject();

	Value meshes(kArrayType);
	for (RMesh* meshInfo : gameObject->Meshes)
	{
		Document::AllocatorType& allocator = document.GetAllocator();
		meshes.PushBack(MeshInfoToJsonValue(meshInfo, allocator), allocator);
	}
	document.AddMember("meshes", meshes, document.GetAllocator());

	std::ofstream jsonFile;
	jsonFile.open(filename);
	StringBuffer buffer;
	buffer.Clear();
	PrettyWriter<StringBuffer> writer(buffer);
	document.Accept(writer);
	jsonFile << buffer.GetString();
	jsonFile.close();
	buffer.Flush();
}

static RMesh* JsonValueToMesh(Value jsonValue)
{
	RMesh* mesh = new RMesh();
	if (jsonValue.HasMember("verticies"))
	{
		Value& verticies = jsonValue["verticies"];
		for (Value::ValueIterator itr = verticies.Begin();
			itr != verticies.End(); ++itr)
		{
			const Value& vertex = itr->GetObject();
			const Value& pos = vertex["pos"];
			const Value& uv = vertex["uv"];
			const Value& normal = vertex["normal"];

			RVertex vertexInfo;
			vertexInfo.Pos.x = pos["x"].GetFloat();
			vertexInfo.Pos.y = pos["y"].GetFloat();
			vertexInfo.Pos.z = pos["z"].GetFloat();
			vertexInfo.UV.x = uv["u"].GetFloat();
			vertexInfo.UV.y = uv["v"].GetFloat();
			vertexInfo.Normal.x = normal["x"].GetFloat();
			vertexInfo.Normal.y = normal["y"].GetFloat();
			vertexInfo.Normal.z = normal["z"].GetFloat();
			mesh->Verticies.push_back(vertexInfo);
		}
	}

	if (jsonValue.HasMember("indicies"))
	{
		Value& indicies = jsonValue["indicies"];
		for (Value::ValueIterator itr = indicies.Begin();
			itr != indicies.End(); ++itr)
		{
			unsigned int index = itr->GetUint();
			mesh->Indicies.push_back(index);
		}
	}

	return mesh;
}

bool JsonUtility::ReadModelFromFile(class RGameObject* outGameObject, std::string filename)
{
	std::string jsonString = "";
	std::ifstream myfile(filename);
	if (myfile.is_open())
	{
		std::string line;
		while (getline(myfile, line))
		{
			jsonString += line;
		}
		myfile.close();
	}

	if (jsonString == "")
		return false;

	Document document;
	document.Parse(jsonString.c_str());

	if (document.HasMember("meshes"))
	{
		Value& meshes = document["meshes"];
		assert(meshes.IsArray());
		for (SizeType i = 0; i < meshes.Size(); i++)
		{
			RMesh* mesh = JsonValueToMesh(meshes[i].GetObject());
			if(mesh->Verticies.size() > 0 && mesh->Indicies.size() > 0)
				outGameObject->Meshes.push_back(mesh);
		}
	}

	if(outGameObject->Meshes.size() > 0)
		return true;

	return false;
}
