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
	
	a3_HierarchyStateBlend.h
	Hierarchy blend operations.
*/

#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#define __ANIMAL3D_HIERARCHYSTATEBLEND_H


#include "a3_HierarchyState.h"

#include "a3_Kinematics.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus

//Typedefs
typedef struct Circumcircle Circumcircle;
typedef struct Edge Edge;
typedef struct Triangle Triangle;
typedef struct a3_ClipController a3_ClipController;

//Forward Declarations
struct a3_ClipController;


//-----------------------------------------------------------------------------

/////////////////// Basic Operations //////////////////////////

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out);

// pointer-based construct operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpConstruct(a3_SpatialPose* pose_out, a3real3p const translation, a3real3p const rotation, a3real3p const scale);

// pointer-based copy operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpCopy(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in);

// pointer-based negate operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpNegate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in);

// pointer-based concatenate operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpConcatenate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_left, a3_SpatialPose const* pose_right);

// pointer-based nearest operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpNearest(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, const a3real u);

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u);

// pointer-based cubic operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpCubic(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1,
	a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, const a3real u);

////////////////// Derivative Operations /////////////////////////

// pointer-based deconcatenate operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpDeconcatenate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_left, a3_SpatialPose const* pose_right);

// pointer-based scale operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpScale(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in, const a3real u);

// pointer-based triangular operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpTriangular(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1,
	a3_SpatialPose const* pose2, const a3real u0, const a3real u1);

// pointer-based bi-nearest operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpBiNearest(a3_SpatialPose* pose_out,
	a3_SpatialPose const* pose0_initial, a3_SpatialPose const* pose0_terminal,
	a3_SpatialPose const* pose1_initial, a3_SpatialPose const* pose1_terminal,
	const a3real u0, const a3real u1, const a3real u01);

// pointer-based bi-linear operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpBiLinear(a3_SpatialPose* pose_out,
	a3_SpatialPose const* pose0_initial, a3_SpatialPose const* pose0_terminal,
	a3_SpatialPose const* pose1_initial, a3_SpatialPose const* pose1_terminal,
	const a3real u0, const a3real u1, const a3real u01);

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// pointer-based bi-cubic operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpBiCubic(a3_SpatialPose* pose_out,
	a3_SpatialPose* poseSet, //Array of 16 poses
	a3real* uArray); //Array of 5 a3real3's


//-----------------------------------------------------------------------------


/////////////////// Basic Operations //////////////////////////

// data-based reset/identity
a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based construct operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpConstruct(a3real3p const translation, a3real3p const rotation, a3real3p const scale);

// data-based copy operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpCopy(a3_SpatialPose const pose_in);

// data-based negate operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpNegate(a3_SpatialPose const pose_in);

// data-based concatenate operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpConcatenate(a3_SpatialPose const pose_left, a3_SpatialPose const pose_right);

// data-based nearest operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpNearest(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);

// data-based LERP
a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);

// data-based cubic operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpCubic(a3_SpatialPose const pose0, a3_SpatialPose const pose1,
	a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, const a3real u);

////////////////// Derivative Operations /////////////////////////

// data-based deconcatenate operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpDeconcatenate(a3_SpatialPose const pose_left, a3_SpatialPose const pose_right);

// data-based scale operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpScale(a3_SpatialPose const pose_in, a3real const u);

// data-based triangular operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpTriangular(a3_SpatialPose const pose0, a3_SpatialPose const pose1,
	a3_SpatialPose const pose2, a3real const u0, a3real const u1);

// data-based bi-nearest operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpBiNearest(
	a3_SpatialPose const pose0_initial, a3_SpatialPose const pose0_terminal,
	a3_SpatialPose const pose1_initial, a3_SpatialPose const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01);

// data-based bi-linear operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpBiLinear(
	a3_SpatialPose const pose0_initial, a3_SpatialPose const pose0_terminal,
	a3_SpatialPose const pose1_initial, a3_SpatialPose const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01);

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// data-based bi-cubic operation for single spatial pose
a3_SpatialPose a3spatialPoseDOpBiCubic(a3_SpatialPose* pose_out,
	a3_SpatialPose* poseSet, //Array of 16 poses
	a3real* uArray); //Array of 5 a3real3's


//-----------------------------------------------------------------------------


/////////////////// Basic Operations //////////////////////////

// pointer-based reset/identity operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out, a3ui32 numNodes);

// pointer-based construct operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpConstruct(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3real3p const translation, a3real3p const rotation, a3real3p const scale);

// pointer-based copy operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpCopy(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_in);

// pointer-based negate operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpNegate(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_in);

// pointer-based concatenate operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpConcatenate(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_left, a3_HierarchyPose* const pose_right);

// pointer-based nearest operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpNearest(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1, a3real const u);

// pointer-based LERP operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1, a3real const u);

// pointer-based cubic operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpCubic(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1,
	a3_HierarchyPose* const pose2, a3_HierarchyPose* const pose3, const a3real u);

////////////////// Derivative Operations /////////////////////////

// pointer-based deconcatenate operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseDOpDeconcatenate(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_left, a3_HierarchyPose* const pose_right);

// pointer-based scale operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseDOpScale(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_in, a3real const u);

// pointer-based triangular operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseDOpTriangular(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1,
	a3_HierarchyPose* const pose2, a3real const u0, a3real const u1);

// pointer-based bi-nearest operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseDOpBiNearest(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01);

// pointer-based bi-linear operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseDOpBiLinear(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01);

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// pointer-based bi-cubic operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseDOpBiCubic(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* poseSet, //Array of 16 poses
	a3real* uArray); //Array of 5 a3real3's

//-----------------------------------------------------------------------------

// Helper Functions

a3real a3cubic(a3real p0, a3real p1, a3real m0, a3real m1, a3real t);


//-----------------------------------------------------------------------------

// Delaunay Triangulation
// Referenced C#/Unity code on a previous project by Dillon Drummond: 
// https://github.com/dillondrum70/daedalus-dungeon-generator/blob/8c4f452b369f37f34d6a853c8958b298a805f42e/Assets/Scripts/DelaunayTriangulation.cs
// https://github.com/dillondrum70/daedalus-dungeon-generator/blob/8c4f452b369f37f34d6a853c8958b298a805f42e/Assets/Scripts/DungeonGenerator.cs#L246

struct Circumcircle
{
	a3vec2 center;
	a3real radius;
};

struct Edge
{
	a3vec2 pointA;
	a3vec2 pointB;
};

struct Triangle
{
	//Points that define triangle
	a3vec2 pointA;
	a3vec2 pointB;
	a3vec2 pointC;
};

//Fill values in triangle
void ConstructTriangle(Triangle* tri_out,
	const a3real pointAX, const a3real pointAY,
	const a3real pointBX, const a3real pointBY,
	const a3real pointCX, const a3real pointCY);

//Check if two triangles are equivalent
//Checks values not addresses
a3i32 TrianglesEquivalent(a3boolean* equal_out, const Triangle* lhs, const Triangle* rhs);

//Print out the points in the triangle
void PrintTriangle(Triangle* tri);



//Construct edges from triangle
a3i32 ConstructEdgesFromTriangle(Edge* edges_out, const Triangle* tri_in);

//Chcek if two edges are equivalent
//Checks values not addresses
a3i32 EdgesEquivalent(a3boolean* equal_out, const Edge* lhs, const Edge* rhs);



//Find circumcenter of a triangle
a3i32 a3_findCircumcenter(Circumcircle* circum_out, Triangle* tri);



//Removes an entry from an array and shifts everything in front of it backwards, updates the count value to match
a3i32 RemoveTriangleFromArray(Triangle* triArray_out, a3ui32* triCount, const a3ui32* index);

//Same as RemoveTriangleFromArray but for edges
a3i32 RemoveEdgeFromArray(Edge* edgeArray_out, a3ui32* edgeCount, const a3ui32* index);



//Compares floats to see if they are reasonably close enough to be considered equal
a3boolean CompareFloats(a3real lhs, a3real rhs);

//Compares vec2s to see if they are reasonably close enough to be considered equal
a3boolean CompareVec2(a3vec2 lhs, a3vec2 rhs);



//Returns the index of a triangle if it exists in the array
//Returns -1 if not found
a3i32 GetIndexOfTriangle(a3i32* index_out, const Triangle* triArray, const a3ui32* triCount, const Triangle* triSearch);

//Returns the index of an a3real if it exists in the array
//Returns -1 if not found
a3i32 GetIndexOfVec2(a3i32* index_out, const a3vec2* vecArray, const a3ui32* vecCount, const a3vec2* vecSearch);

//Returns the index of a edge if it exists in the array
// Takes in an index as an out parameter
// an array of edges
// the length of the array of edges
// and the edge to search for
//Returns -1 if not found
a3i32 GetIndexOfEdge(a3i32* index_out, const Edge* edgeArray, const a3ui32* edgeCount, const Edge* edgeSearch);

//Given a set of points, calculate the triangulation of said points and return the triangles in that triangulation
a3i32 a3_calculateDelaunayTriangulation(Triangle* triArray_out, a3ui32* triCount_out, const a3vec2* pointSet, const a3ui32* pointCount);

//Pointer based spatial pose level operation using delaunay blending
a3_SpatialPose* a3spatialPoseOPDelaunay(a3_SpatialPose* pose_out,
	const a3_SpatialPose* poseA0, const a3_SpatialPose* poseA1,
	const a3_SpatialPose* poseB0, const a3_SpatialPose* poseB1,
	const a3_SpatialPose* poseC0, const a3_SpatialPose* poseC1,
	const a3real keyframeParam1, const a3real keyframeParam2, const a3real keyframeParam3,
	const a3real* blends		//Blend parameter inputs
);

// pointer-based delaunay operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpDelaunay(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	const a3_HierarchyPose* poses,	//Poses from which to get poses by indexes in clip controllers
	const a3vec2* pointSet, const a3_ClipController* clipCtrls, const a3ui32* pointCount, //Data per point
	const Triangle* currentTri,	//Currently selected triangle
	const a3real* blends		//Blend parameter inputs
);


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H