#include "RGameObject.h"
#include "RMesh.h"
#include "RMaterial.h"
#include "RResourceLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

RGameObject::~RGameObject()
{
}

const DirectX::XMMATRIX RGameObject::GetWorldMatrix()
{
	XMVECTOR positionVector = XMLoadFloat3(&m_position);
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = m_rotation.x * 0.0174532925f;
	float yaw = m_rotation.y * 0.0174532925f;
	float roll = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	XMMATRIX positionMatrix = DirectX::XMMatrixTranslationFromVector(positionVector);

	XMMATRIX worldMatrix = DirectX::XMMatrixMultiply(rotationMatrix, positionMatrix);

	return worldMatrix;
}

void RGameObject::SetResource(std::string filenPath)
{
	std::string path = filenPath.substr(0, filenPath.find_last_of("/\\"));

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filenPath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	auto node = scene->mRootNode;
	ProcessNode(node, scene, path);
}

void RGameObject::ProcessNode(const struct aiNode* node, const struct aiScene* scene, std::string path)
{
	// 노드의 모든 mesh들을 처리(만약 있다면)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		RMesh* mesh = new RMesh();
		mesh->Load(aiMesh, scene, "../Resource/");
		Meshes.push_back(mesh);
	}
	// 그런 다음 각 자식들에게도 동일하게 적용
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, path);
	}
}
