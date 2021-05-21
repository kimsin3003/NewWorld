#include "RResourceLoader.h"
#include "Logger.h"
#include "RMesh.h"
#include "RGameObject.h"
#include "RMaterial.h"
#include "Logger.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>

std::vector<RTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<RTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		RTexture texture;
		switch (type)
		{
		case aiTextureType_DIFFUSE:
			texture.type = RTexture::TextureType::DIFFUSE;
			break;
		case aiTextureType_SPECULAR:
			texture.type = RTexture::TextureType::SPECULAR;
			break;
		case aiTextureType_NORMALS:
			texture.type = RTexture::TextureType::NORMAL;
			break;
		}
		texture.filePath = path.data;
		textures.push_back(texture);
	}
	return textures;
}

RMesh* LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<RVertex> vertices;
	RMesh* result = new RMesh();
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		RVertex vertex;
		// vertex 위치, 법선, 텍스처 좌표를 처리
		vertex.Normal = RVector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.Pos = RVector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.UV.clear();
		int j = 0;
		while (mesh->HasTextureCoords(j))
		{
			vertex.UV.emplace_back();
			vertex.UV[j].x = mesh->mTextureCoords[j][i].x;
			vertex.UV[j].y = mesh->mTextureCoords[j][i].y;
			j++;
		}
		result->Verticies.push_back(vertex);
	}

	for (int f = 0; f < mesh->mNumFaces; f++)
	{
		for (int i = 0; i < mesh->mFaces[f].mNumIndices; i++)
		{
			result->Indicies.push_back(mesh->mFaces[f].mIndices[i]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		std::vector<RTexture> textures;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<RTexture> diffuseTextures = LoadMaterialTextures(material,
			aiTextureType_DIFFUSE);
		std::vector<RTexture> specularTextures = LoadMaterialTextures(material,
			aiTextureType_SPECULAR);
		std::vector<RTexture> normalTextures = LoadMaterialTextures(material,
			aiTextureType_NORMALS);
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
		textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
		textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
		result->Mat = new RMaterial(L"Default_VS.hlsl", L"Default_PS.hlsl", textures);
	}

	return result;
}



void ProcessNode(aiNode* node, const aiScene* scene, RGameObject* gameObj)
{
	// 노드의 모든 mesh들을 처리(만약 있다면)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		gameObj->Meshes.push_back(LoadMesh(mesh, scene));
	}
	// 그런 다음 각 자식들에게도 동일하게 적용
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, gameObj);
	}
}


bool RResourceLoader::LoadFile(std::string fileName, RGameObject* gameObj)
{
	std::string fileDir = "../Resource/" + fileName;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(fileDir,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return false;
	}

	auto node = scene->mRootNode;
	ProcessNode(node, scene, gameObj);
	return true;  
}

// 
// void RResourceLoader::LoadVertexInformation(FbxMesh* pMesh, std::vector<RVertex>& outVertexVector, std::vector<unsigned int>& outIndexVector)
// {
// 	FbxVector4* controlPoints = pMesh->GetControlPoints();
// 	if (!controlPoints)
// 		return;
// 	outVertexVector.clear();
// 	outIndexVector.clear();
// 
// 	for (int j = 0; j < pMesh->GetControlPointsCount(); j++)
// 	{
// 		RVertex vertex;
// 		vertex.Pos.x = (float)controlPoints[j].mData[0];
// 		vertex.Pos.y = (float)controlPoints[j].mData[2];
// 		vertex.Pos.z = -(float)controlPoints[j].mData[1];
// 		outVertexVector.emplace_back(vertex);
// 	}
// 
// 	int* iControlPointIndicies = pMesh->GetPolygonVertices();
// 	for (int j = 0; j < pMesh->GetPolygonVertexCount(); j++)
// 	{
// 		outIndexVector.push_back(iControlPointIndicies[j]);
// 	}
// }

// void RResourceLoader::LoadUVInformation(FbxMesh* pMesh, std::vector<RVertex>& outVertexVector)
// {
// 	//get all UV set names
// 	FbxStringList lUVSetNameList;
// 	pMesh->GetUVSetNames(lUVSetNameList);
// 
// 	//iterating over all uv sets
// 	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
// 	{
// 		for (int i = 0; i < pMesh->GetPolygonCount(); i++)
// 		{
// 			int lPolygonsize = pMesh->GetPolygonSize(i);
// 
// 			for (int pol = 0; pol < lPolygonsize; pol++)
// 			{
// 				FbxString name = lUVSetNameList.GetStringAt(lUVSetIndex);
// 				FbxVector2 texCoord;
// 				bool unmapped = false;
// 				if (!pMesh->GetPolygonVertexUV(i, pol, name, texCoord, unmapped))
// 					return;
// 
// 				int lPolyVertIndex = pMesh->GetPolygonVertex(i, pol);
// 				outVertexVector[lPolyVertIndex].UV.x = texCoord.mData[0];
// 				outVertexVector[lPolyVertIndex].UV.y = 1 - texCoord.mData[1];
// 			}
// 		}
// 	}
// }
// 
// void RResourceLoader::LoadNormalInformation(FbxMesh* pMesh, std::vector<RVertex>& outVertexVector)
// {
// 	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
// 	if (lNormalElement)
// 	{
// 		//mapping mode is by control points. The mesh should be smooth and soft.
// 		//we can get normals by retrieving each control point
// 		if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
// 		{
// 			//Let's get normals of each vertex, since the mapping mode of normal element is by control point
// 			for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
// 			{
// 				int lNormalIndex = 0;
// 				//reference mode is direct, the normal index is same as vertex index.
// 				//get normals by the index of control vertex
// 				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
// 					lNormalIndex = lVertexIndex;
// 
// 				//reference mode is index-to-direct, get normals by the index-to-direct
// 				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
// 					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);
// 
// 				//Got normals of each vertex.
// 				FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
// 				outVertexVector[lVertexIndex].Normal.x = lNormal[0];
// 				outVertexVector[lVertexIndex].Normal.y = lNormal[2];
// 				outVertexVector[lVertexIndex].Normal.z = -lNormal[1];
// 				//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
// 				//. . .
// 			}//end for lVertexIndex
// 		}//end eByControlPoint
// 		 //mapping mode is by polygon-vertex.
// 		 //we can get normals by retrieving polygon-vertex.
// 		else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
// 		{
// 			int lIndexByPolygonVertex = 0;
// 			//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
// 			for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++)
// 			{
// 				//get polygon size, you know how many vertices in current polygon.
// 				int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
// 				//retrieve each vertex of current polygon.
// 				for (int i = 0; i < lPolygonSize; i++)
// 				{
// 					int lNormalIndex = 0;
// 					//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
// 					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
// 						lNormalIndex = lIndexByPolygonVertex;
// 
// 					//reference mode is index-to-direct, get normals by the index-to-direct
// 					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
// 						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
// 
// 					//Got normals of each polygon-vertex.
// 					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
// 					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolygonIndex, i);
// 					outVertexVector[lPolyVertIndex].Normal.x = lNormal[0];
// 					outVertexVector[lPolyVertIndex].Normal.y = lNormal[2];
// 					outVertexVector[lPolyVertIndex].Normal.z = -lNormal[1];
// 
// 					lIndexByPolygonVertex++;
// 				}//end for i //lPolygonSize
// 			}//end for lPolygonIndex //PolygonCount
// 
// 		}//end eByPolygonVertex
// 	}//end if lNormalElement
// }
