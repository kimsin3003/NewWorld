
#include "ResourceLoader.h"
#include <fbxsdk.h>
#include <cassert>
#include "Mesh.h"
#include "Logger.h"

FbxManager* g_pFbxSdkManager = nullptr;

bool ResourceLoader::LoadFBX(std::string fbxFileName, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector)
{
	if (g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(fbxFileName.c_str(), -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess)
	{
		Logger::Log("importer initialize failed");
		return false;
	}
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		Logger::Log("importer import failed");
		return false;
	}

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
			if (!pMesh)
				continue;

			FbxVector4* controlPoints = pMesh->GetControlPoints();
			if (!controlPoints)
				continue;

			for (int j = 0; j < pMesh->GetControlPointsCount(); j++)
			{
				Vertex vertex;
				vertex.Pos.x = (float)controlPoints[j].mData[0];
				vertex.Pos.y = (float)controlPoints[j].mData[1];
				vertex.Pos.z = (float)controlPoints[j].mData[2];
				outVertexVector.emplace_back(vertex);

				FbxGeometryElementVertexColor* vertexNormal = pMesh->GetElementVertexColor(0);
				if (!vertexNormal)
					continue;

				switch (vertexNormal->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (vertexNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						outVertexVector[j].Col.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mRed);
						outVertexVector[j].Col.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mGreen);
						outVertexVector[j].Col.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mBlue);
						outVertexVector[j].Col.w = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mAlpha);
					}
					break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int index = vertexNormal->GetIndexArray().GetAt(j);
						outVertexVector[index].Col.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mRed);
						outVertexVector[index].Col.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mGreen);
						outVertexVector[index].Col.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mBlue);
						outVertexVector[index].Col.w = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mAlpha);
					}
					break;

					default:
						throw std::exception("Invalid Reference");
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					switch (vertexNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						outVertexVector[j].Col.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mRed);
						outVertexVector[j].Col.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mGreen);
						outVertexVector[j].Col.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mBlue);
						outVertexVector[j].Col.w = static_cast<float>(vertexNormal->GetDirectArray().GetAt(j).mAlpha);
					}
					break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int index = vertexNormal->GetIndexArray().GetAt(j);
						outVertexVector[index].Col.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mRed);
						outVertexVector[index].Col.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mGreen);
						outVertexVector[index].Col.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mBlue);
						outVertexVector[index].Col.w = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mAlpha);
					}
					break;

					default:
						throw std::exception("Invalid Reference");
					}
					break;
				}
			}

			int* iControlPointIndicies = pMesh->GetPolygonVertices();
			for (int j = 0; j < pMesh->GetPolygonVertexCount(); j++)
			{

				outIndexVector.push_back(iControlPointIndicies[j]);
			}
		}
	}

	if (outVertexVector.size() == 0 || outIndexVector.size() == 0)
	{
		Logger::Log("loading failed");
		return false;
	}
	return true;
}