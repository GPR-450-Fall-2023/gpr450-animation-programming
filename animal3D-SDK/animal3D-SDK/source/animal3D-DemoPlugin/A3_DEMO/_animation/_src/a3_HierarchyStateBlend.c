/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein

	a3_HierarchyStateBlend.c
	Implementation of hierarchical blend operations.
*/

#include "../a3_HierarchyStateBlend.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//-----------------------------------------------------------------------------

//Given a set of points, calculate the triangulation of said points and return the triangles in that triangulation
a3i32 a3_calculateDelaunayTriangulation(Triangle* triArray_out, a3ui32* triCount_out, const a3vec2* pointSet, const a3ui32* pointCount)
{
	//Declare empty list/array of triangles
	//Find super triangle
	//Add it to list of triangles called "triangles"
	//for every point "p"
		//Initialize a list that will have triangles containing this point (in their circumcircle)
		//Initialize a hash map of <Edge, int> that will have number of occurrences of each edge
		// 
		//for every triangle (new ones added at each iteration of the above loop)
			//if circumsphere contains the point
				//add the triangle to list of triangles containing this point
				//update hashmap of occurences to add 1 to the occurence of each edge in the triangle
		// 
		//Create a list of edges "polygon"
		//for every containing triangle
			//get the edges within that triangle
			//for each edge in triangle
				//if edge only occurs once in "occurences"
					//add edge to list of edges "polygon"
		//
		//for every triangle in containing
			//remove that triangle from "triangles"
		//
		//for every edge in "polygon"
			//add a new triangle using the current point "p" as the third point

	//null check
	if (pointSet && pointCount)
	{
		printf("\n ---------------- Starting Delaunay Triangulation -------------\n\n");

		const a3ui32 EDGES_IN_TRIANGLE = 3; //3 edges in a triangle

		//Positions are normalized between 0 and 1
		//This super triangle is guaranteed to contain the entire square with the corners of 0,0 and 1,1
		Triangle superTriangle;
		ConstructTriangle(&superTriangle,
			(a3real)-.1, (a3real)-.1,
			(a3real)2.1, (a3real)0,
			(a3real)0, (a3real)2.1);

		//Add superTriangle as initial
		triArray_out[0] = superTriangle;
		*triCount_out = 1;

		a3ui32 maxTriangles = *pointCount * 2;

		//Loop through all points
		for (a3ui32 pointIndex = 0; pointIndex < *pointCount; pointIndex++)
		{
			const a3ui32 memContainSize = sizeof(Triangle) * maxTriangles;
			const a3ui32 memEdgeSize = sizeof(Edge) * maxTriangles * 3;
			const a3ui32 memPolygonSize = sizeof(Edge) * maxTriangles * 3;
			const a3ui32 memEdgeCountSize = sizeof(a3ui32) * maxTriangles * 3;
			const a3ui32 memreq = memContainSize + memEdgeSize + memEdgeCountSize + memPolygonSize;

			//Allocate necessary memory
			Triangle* containing = (Triangle*)malloc(memreq);
			Edge* edgesOccurring = (Edge*)((Triangle*)containing + maxTriangles);
			Edge* polygon = (Edge*)((Edge*)edgesOccurring + maxTriangles * 3);
			a3ui32* edgeOccurrencesCount = (a3ui32*)((Edge*)polygon + maxTriangles * 3);

			//Could set this all at once, but it acts as an early warning system if something wasn't malloced correctly
			//That shouldn't happen anymore, but it was an issue at one point
			memset(containing, 0, memContainSize);
			memset(edgesOccurring, 0, memEdgeSize);
			memset(polygon, 0, memPolygonSize);
			memset(edgeOccurrencesCount, 0, memEdgeCountSize);

			if (!containing)
			{
				printf("\n ------------------------------\nERROR - Failed to malloc 'containing'\n--------------------------\n\n");
			}

			if (!edgesOccurring)
			{
				printf("\n ------------------------------\nERROR - Failed to malloc 'containing'\n--------------------------\n\n");
			}

			if (!edgeOccurrencesCount)
			{
				printf("\n ------------------------------\nERROR - Failed to malloc 'containing'\n--------------------------\n\n");
			}

			if (!polygon)
			{
				printf("\n ------------------------------\nERROR - Failed to malloc 'polygon'\n--------------------------\n\n");
			}

			a3ui32 containingCount = 0;	//Number of triangles in "containing"
			a3ui32 edgeCount = 0; //Number of edges in "edgesOccurring" and "edgeOccurrencesCount"
			a3ui32 polygonEdgeCount = 0; //Number of edges in "polygon" used later in algorithm

			a3ui32 iterationTriCount = *triCount_out;

			//Find triangles whose circumsphere contains the current point
			//triCount_out changes every iteration so we iterate over all new triangles
			for (a3ui32 triIndex = 0; triIndex < iterationTriCount; triIndex++)
			{
				//Get circumcenter of triangle
				Circumcircle circle;
				a3_findCircumcenter(&circle, &triArray_out[triIndex]);

				printf("Index: %i    Center: (%f, %f)    Radius: %f     Point: (%f, %f)\n",
					triIndex, circle.center.x, circle.center.y,
					circle.radius, pointSet[pointIndex].x, pointSet[pointIndex].y);

				//Distance from point to circle center
				a3real dist;
				dist = a3real2Distance(pointSet[pointIndex].v, circle.center.v);

				//If within circumcenter of triangle, store to perform triangulation later
				if (circle.radius > dist)
				{
					printf("Circumcircle contains point\n");

					//Store the containing triangle for later
					containing[containingCount] = triArray_out[triIndex];
					containingCount++;

					//Get edges in triangle
					Edge triEdges[3];
					ConstructEdgesFromTriangle(triEdges, &containing[containingCount - 1]);

					//Store colliding edges by counting instances of said edges in the array

					//Get index of first edge
					a3i32 edgeIndex = -1;
					GetIndexOfEdge(&edgeIndex, edgesOccurring, &edgeCount, &triEdges[0]);

					//If index found, increment count
					if (edgeIndex >= 0)
					{
						edgeOccurrencesCount[edgeIndex] += (a3ui32)1;
					}
					else //If index not found, add to list of occurring edges
					{
						edgesOccurring[edgeCount] = triEdges[0];
						edgeOccurrencesCount[edgeCount] = 1;
						edgeCount++;
					}

					//Get index of second edge
					edgeIndex = -1;
					GetIndexOfEdge(&edgeIndex, edgesOccurring, &edgeCount, &triEdges[1]);

					//If index found, increment count
					if (edgeIndex >= 0)
					{
						//Should be initialized to 0 with memset on allocation so we can just add
						edgeOccurrencesCount[edgeIndex] += (a3ui32)1;
					}
					else //If index not found, add to list of occurring edges
					{
						edgesOccurring[edgeCount] = triEdges[1];
						edgeOccurrencesCount[edgeCount] = 1;
						edgeCount++;
					}

					//Get index of third edge
					edgeIndex = -1;
					GetIndexOfEdge(&edgeIndex, edgesOccurring, &edgeCount, &triEdges[2]);

					//If index found, increment count
					if (edgeIndex >= 0)
					{
						//Should be initialized to 0 with memset on allocation so we can just add
						edgeOccurrencesCount[edgeIndex] += (a3ui32)1;
					}
					else //If index not found, add to list of occurring edges
					{
						edgesOccurring[edgeCount] = triEdges[2];
						edgeOccurrencesCount[edgeCount] = 1;
						edgeCount++;
					}
				}
			}

			//Get the polygon of all edges that are not duplicates
				//For every triangle containing the current point in their circumcircle
			for (a3ui32 containingIndex = 0; containingIndex < containingCount; containingIndex++)
			{
				//Get edges in triangle from "containing"
				Edge triEdges[3];
				ConstructEdgesFromTriangle(triEdges, &containing[containingIndex]);

				//For each edge in this triangle
				for (a3ui32 edgeIndex = 0; edgeIndex < EDGES_IN_TRIANGLE; edgeIndex++)
				{
					a3i32 index = -1;
					GetIndexOfEdge(&index, edgesOccurring, &edgeCount, &triEdges[edgeIndex]);

					//Sanity check, if index is less than 0, edge does not occur which should be impossible
					if (index < 0)
					{
						printf("ERROR - Edge does not exist");
						free(containing);
						return -1;
					}

					//If edge does not occur more than once
					if (edgeOccurrencesCount[index] <= 1)
					{
						//Add edge to the polygon
						polygon[polygonEdgeCount] = triEdges[edgeIndex];
						polygonEdgeCount++;
					}
				}
			}

			//Remove triangles in "containing" from the array of all triangles "triArray_out"

			for (a3ui32 containIndex = 0; containIndex < containingCount; containIndex++)
			{
				a3i32 index = -1;

				//Get index of containing triangle in triArray_out
				GetIndexOfTriangle(&index, triArray_out, triCount_out, &containing[containIndex]);

				//Sanity check, if index is less than 0, triangle does not occur which should be impossible
				if (index < 0)
				{
					printf("ERROR - Triangle does not exist");
					free(containing);
					return -1;
				}

				//Remove triangle at index
				RemoveTriangleFromArray(triArray_out, triCount_out, &(a3ui32)index);
			}

			//Create a new triangle using both points in each valid edge and the current point

			for (a3ui32 polyIndex = 0; polyIndex < polygonEdgeCount; polyIndex++)
			{
				Triangle* newTri = &triArray_out[*triCount_out];
				newTri->pointA.x = polygon[polyIndex].pointA.x;
				newTri->pointA.y = polygon[polyIndex].pointA.y;
				newTri->pointB.x = polygon[polyIndex].pointB.x;
				newTri->pointB.y = polygon[polyIndex].pointB.y;
				newTri->pointC.x = pointSet[pointIndex].x;
				newTri->pointC.y = pointSet[pointIndex].y;

				*triCount_out += 1;
			}

			//printf("\n ---------------- Finished Delaunay Iteration POINT -------------\n\n");

			//Clean up temporarily allocated data
			if (containing)
			{
				free(containing);

				containing = 0;
				edgesOccurring = 0;
				edgeOccurrencesCount = 0;
				polygon = 0;
			}
		}

		//Delete all triangles that contain points from the super triangle
		for (a3i32 triIndex = *triCount_out - 1; triIndex >= 0; triIndex--)
		{
			if (CompareVec2(triArray_out[triIndex].pointA, superTriangle.pointA) ||
				CompareVec2(triArray_out[triIndex].pointB, superTriangle.pointA) ||
				CompareVec2(triArray_out[triIndex].pointC, superTriangle.pointA) ||
				CompareVec2(triArray_out[triIndex].pointA, superTriangle.pointB) ||
				CompareVec2(triArray_out[triIndex].pointB, superTriangle.pointB) ||
				CompareVec2(triArray_out[triIndex].pointC, superTriangle.pointB) ||
				CompareVec2(triArray_out[triIndex].pointA, superTriangle.pointC) ||
				CompareVec2(triArray_out[triIndex].pointB, superTriangle.pointC) ||
				CompareVec2(triArray_out[triIndex].pointC, superTriangle.pointC))
			{
				//Remove triangle at index
				RemoveTriangleFromArray(triArray_out, triCount_out, &(a3ui32)triIndex);
			}
		}

		printf("\n ---------------- Finished Delaunay Triangulation -------------\n\n");

		return 1;
	}
	return -1;
}





// BLEND OPERATIONS
//-------------------------------------------


void a3_InitBlendTreeNodeInfo(a3_BlendTreeNodeInfo* info)
{
	for (a3ui32 i = 0; i < a3_blend_data_max; i++)
	{
		info->spatialData[i] = NULL;
		info->spatialDataNodes[i] = NULL;
	}

	for (a3ui32 i = 0; i < a3_blend_param_max; i++)
	{
		info->paramData[i] = NULL;
		info->paramDataNodes[i] = NULL;
	}
}


a3_BlendNode* a3_CreateBlendNode(a3_BlendOp blendOperation)
{
	a3_BlendNode* newNode = (a3_BlendNode*)malloc(sizeof(a3_BlendNode));

	if (newNode == NULL) return newNode;

	a3_InitBlendTreeNodeInfo(&(newNode->info));

	newNode->blendOperation = blendOperation;

	return newNode;
}


a3_ParamNode* a3_CreateParamNode(a3_ParamOp paramOperation)
{
	a3_ParamNode* newNode = (a3_ParamNode*)malloc(sizeof(a3_ParamNode));

	if (newNode == NULL) return newNode;

	a3_InitBlendTreeNodeInfo(&(newNode->info));

	newNode->paramOperation = paramOperation;

	return newNode;
}


// Create blend node with passed in variables
//a3_BlendNode* a3_CreateInitializedBlendNode(a3_BlendNode* dataNodes[a3_blend_data_max], a3_BlendData* data[a3_blend_data_max], a3_BlendParam const* param[a3_blend_param_max], a3_BlendOp blendOperation)
//{
//	a3_BlendNode* newNode = a3_CreateBlendNode(blendOperation);
//
//	if (newNode == NULL) return newNode;
//
//	for (a3ui32 i = 0; i < a3_blend_data_max; i++)
//	{
//		newNode->info.spatialDataNodes[i] = dataNodes[i];
//		newNode->info.spatialData[i] = data[i];
//	}
//
//	for (a3ui32 i = 0; i < a3_blend_param_max; i++)
//	{
//		newNode->paramData[i] = param[i];
//	}
//
//	return newNode;
//}


a3boolean a3_InitDataFromNodes(a3_BlendNode* node_out, a3_BlendTree* tree, a3ui32 numBlendData, a3ui32 numParamData)
{
	for (a3ui32 i = 0; i < numBlendData; i++)
	{
		a3_BlendNode* dataNode = node_out->info.spatialDataNodes[i];

		if (dataNode != NULL)
		{
			a3boolean result = dataNode->blendOperation(dataNode, tree);

			if (result == true) // Node successfully run
			{
				node_out->info.spatialData[i] = &(dataNode->result);
			}
		}
	}

	for (a3ui32 i = 0; i < numParamData; i++)
	{
		a3_ParamNode* dataNode = node_out->info.paramDataNodes[i];

		if (dataNode != NULL)
		{
			a3boolean result = dataNode->paramOperation(dataNode, tree);

			if (result == true) // Node successfully run
			{
				node_out->info.paramData[i] = &(dataNode->result);
			}
		}
	}

	return true;
}


a3_BlendData a3_GetBlendNodeResult(a3_BlendNode* node, a3_BlendTree* tree)
{
	node->blendOperation(node, tree);
	return node->result;
}


// BLEND OPERATIONS ------------------------------------------------------------------------------- 

a3boolean a3_BlendOp_Identity(a3_BlendNode* const node_identity, a3_BlendTree* const tree)
{
	node_identity->result = *node_identity->info.spatialData[0];
	return true;
}


// Requires:
// miscData: [0] = clip controller : a3_ClipController*, [1] = clip pool : a3_ClipPool*,
	// [2] = skeletonGroup : a3_HierarchyPoseGroup*, [3] = skeletonHierarchy : a3_Hierarchy*
//a3boolean a3_BlendOpGetClipControllerPose(a3_BlendNode* const node_pose)
//{
//	if (!node_pose) return false;
//	
//	a3_ClipController* clipController = (a3_ClipController*) node_pose->miscData[0];
//	a3_ClipPool* clipPool = (a3_ClipPool*) node_pose->miscData[1];
//	a3_HierarchyPoseGroup* skeletonGroup = (a3_HierarchyPoseGroup*) node_pose->miscData[2];
//	a3_Hierarchy* skeletonHierarchy = (a3_Hierarchy*) node_pose->miscData[3];
//
//	a3_Keyframe keyframe = clipController->keyframe[clipController->keyframeIndex];
//	//a3_Keyframe pose1 = clipController->keyframe[a3clipControllerGetNextKeyframeIndex(clipController)];
//
//	if(pose == NULL) return false;
//
//	a3hierarchyPoseLerp(node_pose->result,
//		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex0,
//		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex1,
//		(a3f32)clipCtrl->keyframeParam, demoMode->hierarchy_skel->numNodes);
//
//	//a3spatialPoseOpLERP(pose0.)
//
//	//if(clipController->clip[clipController->clipIndex].keyframeCount - 1 <= a3clipControllerUpdate)
//
//	return true;
//}


a3boolean a3_BlendOp_Lerp(a3_BlendNode* const node_lerp, a3_BlendTree* const tree)
{
	if (!node_lerp) return false;

	a3_BlendData* const data_out = &(node_lerp->result);

	a3_InitDataFromNodes(node_lerp, tree, 2, 1);

	const a3_BlendData* data0 = node_lerp->info.spatialData[0];
	const a3_BlendData* data1 = node_lerp->info.spatialData[1];
	const a3_BlendParam param = *(node_lerp->info.paramData[0]);

	a3_SpatialPose* result = a3spatialPoseOpLERP(data_out, data0, data1, param);
	return result == data_out;
}


a3boolean a3_BlendOp_Concat(a3_BlendNode* const node_concat, a3_BlendTree* const tree)
{
	if (!node_concat) return false;

	a3_InitDataFromNodes(node_concat, tree, 2, 0);

	a3_BlendData* const data_out = &(node_concat->result);
	const a3_BlendData* data0 = node_concat->info.spatialData[0];
	const a3_BlendData* data1 = node_concat->info.spatialData[1];

	a3_SpatialPose* result = a3spatialPoseOpConcatenate(data_out, data0, data1);

	return result == data_out;
}


a3boolean a3_BlendOp_Scale(a3_BlendNode* const node_scale, a3_BlendTree* const tree)
{
	if (!node_scale) return false;

	a3_InitDataFromNodes(node_scale, tree, 2, 1);

	a3_BlendData* const data_out = &(node_scale->result);
	const a3_BlendData* data0 = node_scale->info.spatialData[0];
	const a3_BlendParam param = *(node_scale->info.paramData[0]);

	a3_SpatialPose* result = a3spatialPoseOpScale(data_out, data0, param);

	return result == data_out;
}


// PARAM OPERATIONS -------------------------------------------------------------------------------------


a3boolean a3_ParamOp_Identity(a3_ParamNode* const node_identity, a3_BlendTree* const tree)
{
	node_identity->result = *node_identity->info.paramData[0];
	return true;
}


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Locomotion Algorithms

// Euler Integration --------------------------------------------------

// Scalar
a3real fIntegrateEuler1(a3real x, a3real dx_dt, a3real dt) {
	a3real result = x + (dx_dt * dt);
	return result;
}

// Vec2
a3vec2 fIntegrateEuler2(a3vec2 x, a3vec2 dx_dt, a3real dt) {
	a3vec2 result = a3vec2_one;
	result.x = x.x + (dx_dt.x * dt);
	result.y = x.y + (dx_dt.y * dt);
	return result;
}

// Vec3
a3vec3 fIntegrateEuler3(a3vec3 x, a3vec3 dx_dt, a3real dt) {
	a3vec3 result = a3vec3_one;
	result.x = x.x + (dx_dt.x * dt);
	result.y = x.y + (dx_dt.y * dt);
	result.z = x.z + (dx_dt.z * dt);
	return result;
}

// Kinematic Integration --------------------------------------------------
// For Aster To Do...

// Interpolation-based Integration --------------------------------------------------

// Scalar
a3real fIntegrateInterpolation1(a3real x, a3real xc, a3real u) {
	a3real result = x + ((xc - x) * u);
	return result;
}

// Vec2
a3vec2 fIntegrateInterpolation2(a3vec2 x, a3vec2 xc, a3real u) {
	a3vec2 result = a3vec2_one;
	result.x = x.x + ((xc.x - x.x) * u);
	result.y = x.y + ((xc.y - x.y) * u);
	return result;
}

// Vec3
a3vec3 fIntegrateInterpolation3(a3vec3 x, a3vec3 xc, a3real u) {
	a3vec3 result = a3vec3_one;
	result.x = x.x + ((xc.x - x.x) * u);
	result.y = x.y + ((xc.y - x.y) * u);
	result.z = x.z + ((xc.z - x.z) * u);
	return result;
}