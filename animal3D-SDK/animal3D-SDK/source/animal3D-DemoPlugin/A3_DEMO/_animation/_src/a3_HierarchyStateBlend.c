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
#include <assert.h>


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


void a3_InitBlendTreeNodeInfoToEmpty(a3_BlendTreeNodeInfo* info)
{
	for (a3ui32 i = 0; i < a3_blend_spatial_data_max; i++)
	{
		info->spatialData[i] = NULL;
		info->spatialDataNodes[i] = NULL;
	}

	for (a3ui32 i = 0; i < a3_blend_param_data_max; i++)
	{
		info->paramData[i] = NULL;
		info->paramDataNodes[i] = NULL;
	}

	for (a3ui32 i = 0; i < a3_blend_misc_data_max; i++)
	{
		info->miscData[i] = NULL;
	}
}


a3_BlendNode* a3_CreateBlendNode(a3_BlendOp blendOperation)
{
	a3_BlendNode* newNode = (a3_BlendNode*)malloc(sizeof(a3_BlendNode));

	if (newNode == NULL) return newNode;

	a3_InitBlendTreeNodeInfoToEmpty(&(newNode->info));

	newNode->blendOperation = blendOperation;

	return newNode;
}


a3_ParamNode* a3_CreateParamNode(a3_ParamOp paramOperation)
{
	a3_ParamNode* newNode = (a3_ParamNode*)malloc(sizeof(a3_ParamNode));

	if (newNode == NULL) return newNode;

	a3_InitBlendTreeNodeInfoToEmpty(&(newNode->info));

	newNode->paramOperation = paramOperation;

	return newNode;
}


a3boolean a3_InitDataFromNodes(a3_BlendTreeNodeInfo* info, a3_BlendTree* tree, a3ui32 hierarchyIndex, a3real dt, a3ui32 numBlendData, a3ui32 numParamData)
{
	for (a3ui32 i = 0; i < numBlendData; i++)
	{
		a3_BlendNode* dataNode = info->spatialDataNodes[i];

		if (dataNode != NULL)
		{
			a3boolean result = dataNode->blendOperation(dataNode, tree, hierarchyIndex, dt);

			if (result == true) // Node successfully run
			{
				info->spatialData[i] = &(dataNode->result);
			}
		}
	}

	for (a3ui32 i = 0; i < numParamData; i++)
	{
		a3_ParamNode* dataNode = info->paramDataNodes[i];

		if (dataNode != NULL)
		{
			a3boolean result = dataNode->paramOperation(dataNode, tree, hierarchyIndex, dt);

			if (result == true) // Node successfully run
			{
				info->paramData[i] = &(dataNode->result);
			}
		}
	}

	return true;
}

a3_BlendNode* a3blendTreeGetNode(a3_BlendNode** nodes, a3ui32 nodeCount, const a3byte name[a3_blend_node_id_length])
{
	a3ui32 i;
	for (i = 0; i < nodeCount; ++i)
		if (!strcmp(nodes[i]->info.node_id, name))
			return nodes[i];
	return NULL;
}


// Gets result of blend operation from node
a3_BlendPose a3_GetBlendNodeResult(a3_BlendNode* node, a3_BlendTree* tree, a3ui32 hierarchyIndex, a3real dt)
{
	node->blendOperation(node, tree, hierarchyIndex, dt);
	return node->result;
}


// Checks if passed in params list is sequential ascending, starting at startIndex and going up to startIndex + count
a3boolean a3_BlendParamsAscendingSequential(a3_BlendParam* params[a3_blend_param_data_max], a3ui32 startIndex, a3ui32 count)
{
	if (count <= 1) return true;
	if (count <= 2) return params[0] <= params[1];

	a3_BlendParam prev;
	a3_BlendParam curr;
	a3_BlendParam next;

	for (a3ui32 i = startIndex + 1; i < startIndex + count - 1; i++)
	{
		prev = *(params[i - 1]);
		curr = *(params[i]);
		next = *(params[i + 1]);

		if (!(prev <= curr && curr <= next))
		{
			return false;
		}
	}

	return true;
}


// BLEND OPERATIONS ------------------------------------------------------------------------------- 

a3boolean a3_BlendOp_Identity(a3_BlendNode* const node_identity, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	node_identity->result = *node_identity->info.spatialData[0];
	return true;
}


a3boolean a3_BlendOp_Lerp(a3_BlendNode* const node_lerp, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	if (!node_lerp) return false;

	a3_BlendPose* const data_out = &(node_lerp->result);

	a3_InitDataFromNodes(&(node_lerp->info), tree, hierarchyIndex, dt, 2, 1);

	const a3_BlendPose* data0 = node_lerp->info.spatialData[0];
	const a3_BlendPose* data1 = node_lerp->info.spatialData[1];
	const a3_BlendParam param = *(node_lerp->info.paramData[0]);

	a3_SpatialPose* result = a3spatialPoseOpLERP(data_out, data0, data1, param);
	return result == data_out;
}


a3boolean a3_BlendOp_Concat(a3_BlendNode* const node_concat, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	if (!node_concat) return false;

	a3_InitDataFromNodes(&(node_concat->info), tree, hierarchyIndex, dt, 2, 0);

	a3_BlendPose* const data_out = &(node_concat->result);
	const a3_BlendPose* data0 = node_concat->info.spatialData[0];
	const a3_BlendPose* data1 = node_concat->info.spatialData[1];

	a3_SpatialPose* result = a3spatialPoseOpConcatenate(data_out, data0, data1);

	return result == data_out;
}


a3boolean a3_BlendOp_Scale(a3_BlendNode* const node_scale, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	if (!node_scale) return false;

	a3_InitDataFromNodes(&(node_scale->info), tree, hierarchyIndex, dt, 2, 1);

	a3_BlendPose* const data_out = &(node_scale->result);
	const a3_BlendPose* data0 = node_scale->info.spatialData[0];
	const a3_BlendParam param = *(node_scale->info.paramData[0]);

	a3_SpatialPose* result = a3spatialPoseOpScale(data_out, data0, param);

	return result == data_out;
}


// Note: Can definitely make this generic to handle n blend options, but need a way to store in node how many it should blend
// Would love to add variables to blend tree, but not enough time
// 1st Param : Blend Num, 2nd - 4th Param : Blend Threshold
a3boolean a3_BlendOp_Blend_3(a3_BlendNode* const node_blend, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	a3_InitDataFromNodes(&(node_blend->info), tree, hierarchyIndex, dt, 3, 4);

	if (!a3_BlendParamsAscendingSequential(node_blend->info.paramData, 1, 3))
	{
		return false;
	}

	a3_BlendParam param = *(node_blend->info.paramData[0]);

	a3real portionBetween = 0;
	a3ui32 indexChosen = 0; // Not used for functionality, just debug
	
	a3_BlendPose* pose0;
	a3_BlendPose* pose1;

	// Note: Spatial Data tied to threshold = Threshold index - 1
	if (param <= *(node_blend->info.paramData[1])) // Check if param below lowest threshold
	{
		portionBetween = 0;
		indexChosen = 1;
		pose0 = node_blend->info.spatialData[0];
		pose1 = node_blend->info.spatialData[1];
	}
	else if (param >= *(node_blend->info.paramData[3])) // Check if param above highest threshold
	{
		portionBetween = 1;
		indexChosen = 3;
		pose0 = node_blend->info.spatialData[1];
		pose1 = node_blend->info.spatialData[2];
	}
	else // Calculate where it is between thresholds
	{
		for (a3ui32 i = 2; i <= 3; i++)
		{
			// Check if below threshold, since thresholds are in order this means it is between this and previous one
			if (param <= *(node_blend->info.paramData[i]))
			{
				// Threshold0 will be a lower number
				a3real threshold0 = *(node_blend->info.paramData[i - 1]);
				a3real threshold1 = *(node_blend->info.paramData[i]);

				pose0 = node_blend->info.spatialData[i - 2];
				pose1 = node_blend->info.spatialData[i - 1];

				portionBetween = 1 - ((threshold1 - param) / (threshold1 - threshold0));

				indexChosen = i - 1;

				break;
			}
		}
	}

	a3_BlendNode calcNode;
	a3_InitBlendTreeNodeInfoToEmpty(&(calcNode.info));

	calcNode.info.spatialData[0] = pose0;
	calcNode.info.spatialData[1] = pose1;
	calcNode.info.paramData[0] = &portionBetween;

	a3_BlendOp_Lerp(&calcNode, tree, hierarchyIndex, dt);

	node_blend->result = calcNode.result;

	return true;
}


// MiscData: 0 = Clip Controller, 1 = Hierarchy Pose Group
a3boolean a3_BlendOp_EvaluateClipController(a3_BlendNode* const node_eval, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	a3_InitDataFromNodes(&(node_eval->info), tree, hierarchyIndex, dt, 0, 0);

	a3_ClipController* ctrl = (a3_ClipController*) node_eval->info.miscData[0];
	a3_HierarchyPoseGroup* poseGroup = (a3_HierarchyPoseGroup*) node_eval->info.miscData[1];

	if (ctrl == NULL || poseGroup == NULL) return false;

	a3_HierarchyPose* pose0 = poseGroup->hpose + ctrl->clipPool->keyframe[ctrl->keyframeIndex].sampleIndex0;
	a3_HierarchyPose* pose1 = poseGroup->hpose + ctrl->clipPool->keyframe[ctrl->keyframeIndex].sampleIndex1;

	a3_BlendParam keyframeParam = (a3real) ctrl->keyframeParam;

	node_eval->info.spatialData[0] = pose0->pose + hierarchyIndex;
	node_eval->info.spatialData[1] = pose1->pose + hierarchyIndex;
	node_eval->info.paramData[0] = &keyframeParam;

	a3_BlendOp_Lerp(node_eval, tree, hierarchyIndex, dt);

	return true;
}


// If condition is true, will run node at 0, else will run node at 1
// MiscData: 0 = Boolean condition
a3boolean a3_BlendOp_BoolBranch(a3_BlendNode* const node_branch, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	a3boolean* condition = node_branch->info.miscData[0];

	if (condition == NULL) return false;

	a3ui32 index = *condition ? 0 : 1;

	a3_BlendNode* dataNode = node_branch->info.spatialDataNodes[index];

	if (dataNode != NULL)
	{
		a3boolean result = dataNode->blendOperation(dataNode, tree, hierarchyIndex, dt);

		if (result == true) // Node successfully run
		{
			node_branch->result = dataNode->result;
			return true;
		}
	}

	node_branch->result = *(node_branch->info.spatialData[index]);
	return true;
}


// Param Data: 0 = jumpDuration, 1 = jumpHeight, 2 = jumpFadeInTime, 3 = jumpFadeOutTime
// Misc Data: 0 = timeSinceJump, 1 = jumpLerpParam, 2 = isJumping, 3 = ctrlNode
a3boolean a3_BlendOp_HandleJump(a3_BlendNode* const node_jump, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
{
	a3_InitDataFromNodes(&(node_jump->info), tree, hierarchyIndex, dt, 1, 0);
	node_jump->result = *(node_jump->info.spatialData[0]);
	
	if (hierarchyIndex != 0) return true;

	a3_BlendParam* jumpDuration = node_jump->info.paramData[0];
	a3_BlendParam* jumpHeight = node_jump->info.paramData[1];
	a3_BlendParam* jumpFadeInTime = node_jump->info.paramData[2];
	a3_BlendParam* jumpFadeOutTime = node_jump->info.paramData[3];

	a3real* timeSinceJump = (a3real*)node_jump->info.miscData[0];
	a3real* jumpLerpParam = (a3real*)node_jump->info.miscData[1];
	a3boolean* isJumping = (a3boolean*) node_jump->info.miscData[2];
	a3_SpatialPose* ctrlNode = (a3_SpatialPose*)node_jump->info.miscData[3];

	(*timeSinceJump) += dt;

	// If we've been jumping for at least duration, reset
	if (*timeSinceJump >= *jumpDuration)
	{
		*isJumping = false;
		ctrlNode->translate.z = 0;
		*timeSinceJump = 0;
		return true;
	}

	// Calculate jumpLerpParam
	if (*timeSinceJump < *jumpFadeInTime)
	{
		*jumpLerpParam = *timeSinceJump / *jumpFadeInTime;
	}
	else if (*jumpDuration - *timeSinceJump < *jumpFadeOutTime)
	{
		*jumpLerpParam = (*jumpDuration - *timeSinceJump) / *jumpFadeOutTime;
	}
	else
	{
		*jumpLerpParam = 1;
	}

	// Parabola equation: (-4H/D^2)t^2 + (4H/D)t
	// H = Max jump height, D = Max jump duration
	a3real a = (-4 * *jumpHeight) / (*jumpDuration * *jumpDuration);
	a3real b = (4 * *jumpHeight) / *jumpDuration;

	a3real firstComp = (a * *timeSinceJump * *timeSinceJump);
	a3real secondComp = (b * *timeSinceJump);

	ctrlNode->translate.z = firstComp + secondComp;

	//printf("%f\n", ctrlNode->translate.z);

	return true;
}


// PARAM OPERATIONS -------------------------------------------------------------------------------------


a3boolean a3_ParamOp_Identity(a3_ParamNode* const node_identity, a3_BlendTree* const tree, a3ui32 hierarchyIndex, a3real dt)
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