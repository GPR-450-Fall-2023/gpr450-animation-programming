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

typedef struct Circumcircle Circumcircle;
typedef struct Edge Edge;
typedef struct Triangle Triangle;
	

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
	const a3_SpatialPose* poseSet0, //Array of 4 poses
	const a3_SpatialPose* poseSet1, //Array of 4 poses
	const a3_SpatialPose* poseSet2, //Array of 4 poses
	const a3_SpatialPose* poseSet3, //Array of 4 poses
	const a3real3* uArray); //Array of 5 a3real3's


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
a3_SpatialPose a3spatialPoseDOpBiCubic(
	a3_SpatialPose const* poseSet0, //Array of 4 poses
	a3_SpatialPose const* poseSet1, //Array of 4 poses
	a3_SpatialPose const* poseSet2, //Array of 4 poses
	a3_SpatialPose const* poseSet3, //Array of 4 poses
	a3real3 const* uArray); //Array of 5 a3real3's


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
	a3_HierarchyPose* const* poseSet0, //Array of 4 poses
	a3_HierarchyPose* const* poseSet1, //Array of 4 poses
	a3_HierarchyPose* const* poseSet2, //Array of 4 poses
	a3_HierarchyPose* const* poseSet3, //Array of 4 poses
	a3real3 const* uArray); //Array of 5 a3real3's

//-----------------------------------------------------------------------------

// Helper Functions

a3real a3cubic(p0, p1, m0, m1, t);


//-----------------------------------------------------------------------------

// Delaunay Triangulation

struct Circumcircle
{
	a3real2 center;
	a3real radius;
};

struct Edge
{
	a3real2 pointA;
	a3real2 pointB;
};

struct Triangle
{
	//Points that define triangle
	a3real2 pointA;
	a3real2 pointB;
	a3real2 pointC;
};

//Find circumcenter of a triangle
a3i32 a3_findCircumcenter(Circumcircle* circum_out, Triangle* tri);

//Given a set of points, calculate the triangulation of said points and return the triangles in that triangulation
a3i32 a3_calculateDelaunayTriangulation(Triangle* triArray_out, const a3vec2* triSet, const a3real* triCount);


a3_SpatialPose* a3spatialPoseOPDelaunay(a3_SpatialPose* pose_out, a3real2p point);



#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H