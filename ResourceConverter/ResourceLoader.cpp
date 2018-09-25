#include "ResourceLoader.h"
#include <iostream>
#include "Logger.h"
#include "Share/ResourceInfo.h"

FbxManager* g_pFbxSdkManager = nullptr;


bool ResourceLoader::LoadFBX(std::string fbxFileName, ModelInfo* outModelInfo)
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
		FbxString error = pImporter->GetStatus().GetErrorString();
		std::cout << "importer initialize failed :" << error.Buffer() << std::endl;
		return false;
	}
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		FbxString error = pImporter->GetStatus().GetErrorString();
		std::cout << "importer import failed :" << error.Buffer() << std::endl;
		return false;
	}

	pImporter->Destroy();

	FbxGeometryConverter lGeomConverter(g_pFbxSdkManager);
	lGeomConverter.Triangulate(pFbxScene, true);
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			FbxMesh* pMesh = pFbxChildNode->GetMesh();
			if (!pMesh)
				continue;

			MeshInfo* mesh = new MeshInfo();
			LoadVertexInformation(pMesh, mesh->Verticies, mesh->Indicies);

			LoadUVInformation(pMesh, mesh->Verticies);
			LoadNormalInformation(pMesh, mesh->Verticies);
			outModelInfo->meshInfos.emplace_back(mesh);
		}
	}

	if (outModelInfo->meshInfos.size() == 0)
	{
		std::cout << "loading failed" << std::endl;
		return false;
	}
	return true;
}

void ResourceLoader::LoadVertexInformation(FbxMesh* pMesh, std::vector<VertexInfo>& outVertexVector, std::vector<unsigned int>& outIndexVector)
{
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	if (!controlPoints)
		return;
	outVertexVector.clear();
	outIndexVector.clear();

	for (int j = 0; j < pMesh->GetControlPointsCount(); j++)
	{
		VertexInfo vertex;
		vertex.Pos.x = (float)controlPoints[j].mData[0];
		vertex.Pos.y = (float)controlPoints[j].mData[2];
		vertex.Pos.z = -(float)controlPoints[j].mData[1];
		outVertexVector.emplace_back(vertex);
	}

	int* iControlPointIndicies = pMesh->GetPolygonVertices();
	for (int j = 0; j < pMesh->GetPolygonVertexCount(); j++)
	{
		outIndexVector.push_back(iControlPointIndicies[j]);
	}
}

void ResourceLoader::LoadUVInformation(FbxMesh* pMesh, std::vector<VertexInfo>& outVertexVector)
{
	//get all UV set names
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					FbxVector2 lUVValue;

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

					outVertexVector[lUVIndex].UV.x = lUVValue[0];
					outVertexVector[lUVIndex].UV.y = 1 - lUVValue[1];
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
						int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

						outVertexVector[lPolyVertIndex].UV.x = lUVValue[0];
						outVertexVector[lPolyVertIndex].UV.y = 1 - lUVValue[1];

						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}

void ResourceLoader::LoadNormalInformation(FbxMesh* pMesh, std::vector<VertexInfo>& outVertexVector)
{
	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
	if (lNormalElement)
	{
		//mapping mode is by control points. The mesh should be smooth and soft.
		//we can get normals by retrieving each control point
		if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			//Let's get normals of each vertex, since the mapping mode of normal element is by control point
			for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
			{
				int lNormalIndex = 0;
				//reference mode is direct, the normal index is same as vertex index.
				//get normals by the index of control vertex
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					lNormalIndex = lVertexIndex;

				//reference mode is index-to-direct, get normals by the index-to-direct
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

				//Got normals of each vertex.
				FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				outVertexVector[lVertexIndex].Normal.x = lNormal[0];
				outVertexVector[lVertexIndex].Normal.y = lNormal[2];
				outVertexVector[lVertexIndex].Normal.z = - lNormal[1];
				//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
				//. . .
			}//end for lVertexIndex
		}//end eByControlPoint
		 //mapping mode is by polygon-vertex.
		 //we can get normals by retrieving polygon-vertex.
		else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lIndexByPolygonVertex = 0;
			//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
			for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++)
			{
				//get polygon size, you know how many vertices in current polygon.
				int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
				//retrieve each vertex of current polygon.
				for (int i = 0; i < lPolygonSize; i++)
				{
					int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
						lNormalIndex = lIndexByPolygonVertex;

					//reference mode is index-to-direct, get normals by the index-to-direct
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

					//Got normals of each polygon-vertex.
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolygonIndex, i);
					outVertexVector[lPolyVertIndex].Normal.x = lNormal[0];
					outVertexVector[lPolyVertIndex].Normal.y = lNormal[2];
					outVertexVector[lPolyVertIndex].Normal.z = -lNormal[1];

					lIndexByPolygonVertex++;
				}//end for i //lPolygonSize
			}//end for lPolygonIndex //PolygonCount

		}//end eByPolygonVertex
	}//end if lNormalElement
}