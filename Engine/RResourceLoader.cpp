#include "RResourceLoader.h"
#include "Logger.h"
#include "RMesh.h"
#include "RGameObject.h"
#include "RMaterial.h"
#include "Logger.h"
#include <vector>
#include <iostream>






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
