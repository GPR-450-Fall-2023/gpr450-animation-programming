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
	
	a3_HierarchyStateBlend.inl
	Implementation of inline hierarchical blend operations.
*/


#ifdef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_INL
#define __ANIMAL3D_HIERARCHYSTATEBLEND_INL

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../a3_KeyframeAnimationController.h"

//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out)
{
	pose_out->transformMat = a3mat4_identity;
	// ...

	// done
	return pose_out;
}

// pointer-based construct operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpConstruct(a3_SpatialPose* pose_out, a3real3p const translate, a3real3p const rotation, a3real3p const scale)
{
	a3spatialPoseSetTranslation(pose_out, translate[0], translate[1], translate[2]);
	a3spatialPoseSetRotation(pose_out, rotation[0], rotation[1], rotation[2]);
	a3spatialPoseSetScale(pose_out, scale[0], scale[1], scale[2]);
	// done
	return pose_out;
}

// pointer-based copy operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpCopy(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in)
{
	a3real3 trans, rot, scale;
	trans[0] = pose_in->translate.x;
	trans[1] = pose_in->translate.y;
	trans[2] = pose_in->translate.z;
	rot[0] = pose_in->rotate.x;
	rot[1] = pose_in->rotate.y;
	rot[2] = pose_in->rotate.z;
	scale[0] = pose_in->scale.x;
	scale[1] = pose_in->scale.y;
	scale[2] = pose_in->scale.z;
	a3spatialPoseOpConstruct(pose_out, trans, rot, scale);
	// if there's a better way to turn a3vec3 to a3real3p then please tell me - Neo
	// done
	return pose_out;
}
// pointer-based negate operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpNegate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in)
{
	a3real3 trans, rot, scale;
	trans[0] = pose_in->translate.x * (a3real)-1.0;
	trans[1] = pose_in->translate.y * (a3real)-1.0;
	trans[2] = pose_in->translate.z * (a3real)-1.0;
	rot[0] = pose_in->rotate.x * (a3real)-1.0;
	rot[1] = pose_in->rotate.y * (a3real)-1.0;
	rot[2] = pose_in->rotate.z * (a3real)-1.0;
	scale[0] = (a3real)1.0 / pose_in->scale.x;
	scale[1] = (a3real)1.0 / pose_in->scale.y;
	scale[2] = (a3real)1.0 / pose_in->scale.z;
	a3spatialPoseOpConstruct(pose_out, trans, rot, scale);
	// done
	return pose_out;
}

// pointer-based concatenate operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpConcatenate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_left, a3_SpatialPose const* pose_right)
{
	a3real3 trans, rot, scale;
	trans[0] = pose_left->translate.x + pose_right->translate.x;
	trans[1] = pose_left->translate.y + pose_right->translate.y;
	trans[2] = pose_left->translate.z + pose_right->translate.z;
	rot[0] = pose_left->rotate.x + pose_right->rotate.x;
	rot[1] = pose_left->rotate.y + pose_right->rotate.y;
	rot[2] = pose_left->rotate.z + pose_right->rotate.z;
	scale[0] = pose_left->scale.x * pose_right->scale.x;
	scale[1] = pose_left->scale.y * pose_right->scale.y;
	scale[2] = pose_left->scale.z * pose_right->scale.z;
	a3spatialPoseOpConstruct(pose_out, trans, rot, scale);
	// done
	return pose_out;
}

// pointer-based nearest operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpNearest(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, const a3real u)
{
	if (u < (a3real)0.5) {
		a3spatialPoseOpCopy(pose_out, pose0);
	}
	else {
		a3spatialPoseOpCopy(pose_out, pose1);
	}
	// done
	return pose_out;
}

// pointer-based LERP operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{
	a3real3 trans, rot, scale;
	trans[0] = (pose1->translate.x * u) + (pose0->translate.x * ((a3real)1.0 - u));
	trans[1] = (pose1->translate.y * u) + (pose0->translate.y * ((a3real)1.0 - u));
	trans[2] = (pose1->translate.z * u) + (pose0->translate.z * ((a3real)1.0 - u));
	rot[0] = (pose1->rotate.x * u) + (pose0->rotate.x * ((a3real)1.0 - u));
	rot[1] = (pose1->rotate.y * u) + (pose0->rotate.y * ((a3real)1.0 - u));
	rot[2] = (pose1->rotate.z * u) + (pose0->rotate.z * ((a3real)1.0 - u));
	scale[0] = (pose1->scale.x * u) + (pose0->scale.x * ((a3real)1.0 - u));
	scale[1] = (pose1->scale.y * u) + (pose0->scale.y * ((a3real)1.0 - u));
	scale[2] = (pose1->scale.z * u) + (pose0->scale.z * ((a3real)1.0 - u));
	a3spatialPoseOpConstruct(pose_out, trans, rot, scale);
	// done
	return pose_out;
}

// pointer-based cubic operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpCubic(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1,
	a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, const a3real u)
{
	a3real3 trans, rot, scale;
	trans[0] = a3cubic(pose0->translate.x, pose1->translate.x, pose2->translate.x, pose3->translate.x, u);
	trans[1] = a3cubic(pose0->translate.y, pose1->translate.y, pose2->translate.y, pose3->translate.y, u);
	trans[2] = a3cubic(pose0->translate.z, pose1->translate.z, pose2->translate.z, pose3->translate.z, u);
	rot[0] = a3cubic(pose0->rotate.x, pose1->rotate.x, pose2->rotate.x, pose3->rotate.x, u);
	rot[1] = a3cubic(pose0->rotate.y, pose1->rotate.y, pose2->rotate.y, pose3->rotate.y, u);
	rot[2] = a3cubic(pose0->rotate.z, pose1->rotate.z, pose2->rotate.z, pose3->rotate.z, u);
	scale[0] = a3cubic(pose0->scale.x, pose1->scale.x, pose2->scale.x, pose3->scale.x, u);
	scale[1] = a3cubic(pose0->scale.y, pose1->scale.y, pose2->scale.y, pose3->scale.y, u);
	scale[2] = a3cubic(pose0->scale.z, pose1->scale.z, pose2->scale.z, pose3->scale.z, u);
	a3spatialPoseOpConstruct(pose_out, trans, rot, scale);
	// done
	return pose_out;
}

// pointer-based deconcatenate operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpDeconcatenate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_left, a3_SpatialPose const* pose_right)
{
	a3_SpatialPose rightArg = a3spatialPoseDOpNegate(*pose_right);
	a3spatialPoseOpConcatenate(pose_out, pose_left, &rightArg);

	// done
	return pose_out;
}

// pointer-based scale operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpScale(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in, const a3real u)
{
	a3real3 translate;
	a3real3 rotation;
	a3real3 scale;

	translate[0] = pose_in->translate.x * u;
	translate[1] = pose_in->translate.y * u;
	translate[2] = pose_in->translate.z * u;

	rotation[0] = pose_in->rotate.x * u;
	rotation[1] = pose_in->rotate.y * u;
	rotation[2] = pose_in->rotate.z * u;

	scale[0] = powf(pose_in->scale.x, u);
	scale[1] = powf(pose_in->scale.y, u);
	scale[2] = powf(pose_in->scale.z, u);

	a3spatialPoseOpConstruct(pose_out, translate, rotation, scale);

	// done
	return pose_out;
}

// pointer-based triangular operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpTriangular(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1,
	a3_SpatialPose const* pose2, const a3real u0, const a3real u1)
{
	a3_SpatialPose scaled0 = a3spatialPoseDOpScale(*pose0, u0);
	a3_SpatialPose scaled1 = a3spatialPoseDOpScale(*pose1, u1);

	a3real gamma = (a3real)1 - u0 - u1;

	a3_SpatialPose scaled2 = a3spatialPoseDOpScale(*pose2, gamma);

	a3_SpatialPose added01 = a3spatialPoseDOpConcatenate(scaled0, scaled1);

	a3spatialPoseOpConcatenate(pose_out, &added01, &scaled2);

	// done
	return pose_out;
}

// pointer-based bi-nearest operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpBiNearest(a3_SpatialPose* pose_out,
	a3_SpatialPose const* pose0_initial, a3_SpatialPose const* pose0_terminal,
	a3_SpatialPose const* pose1_initial, a3_SpatialPose const* pose1_terminal,
	const a3real u0, const a3real u1, const a3real u01)
{
	a3_SpatialPose nearest0 = a3spatialPoseDOpNearest(*pose0_initial, *pose0_terminal, u0);
	a3_SpatialPose nearest1 = a3spatialPoseDOpNearest(*pose1_initial, *pose1_terminal, u1);

	a3spatialPoseOpNearest(pose_out, &nearest0, &nearest1, u01);

	// done
	return pose_out;
}

// pointer-based bi-linear operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpBiLinear(a3_SpatialPose* pose_out,
	a3_SpatialPose const* pose0_initial, a3_SpatialPose const* pose0_terminal,
	a3_SpatialPose const* pose1_initial, a3_SpatialPose const* pose1_terminal,
	const a3real u0, const a3real u1, const a3real u01)
{
	a3_SpatialPose firstLerp = a3spatialPoseDOpLERP(*pose0_initial, *pose0_terminal, u0);
	a3_SpatialPose secondLerp = a3spatialPoseDOpLERP(*pose1_initial, *pose1_terminal, u1);

	a3spatialPoseOpLERP(pose_out, &firstLerp, &secondLerp, u01);

	// done
	return pose_out;
}

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// pointer-based bi-cubic operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpBiCubic(a3_SpatialPose* pose_out,
	a3_SpatialPose* poseSet, //Array of 16 poses
	a3real* uArray) //Array of 5 a3real3's
{
	a3_SpatialPose pose1, pose2, pose3, pose4;
	a3spatialPoseOpCubic(&pose1, poseSet, poseSet + 1, poseSet + 2, poseSet + 3, *uArray);
	a3spatialPoseOpCubic(&pose2, poseSet + 4, poseSet + 5, poseSet + 6, poseSet + 7, *(uArray + 1));
	a3spatialPoseOpCubic(&pose3, poseSet + 8, poseSet + 9, poseSet + 10, poseSet + 11, *(uArray + 2));
	a3spatialPoseOpCubic(&pose4, poseSet + 12, poseSet + 13, poseSet + 14, poseSet + 15, *(uArray + 3));

	a3spatialPoseOpCubic(pose_out, &pose1, &pose2, &pose3, &pose4, *(uArray + 4));

	// Don't think I can do for now, seems like something's wrong with cubic

	// done
	return pose_out;
}



//-----------------------------------------------------------------------------

// data-based reset/identity
inline a3_SpatialPose a3spatialPoseDOpIdentity()
{
	a3_SpatialPose const result = { a3mat4_identity /*, ...*/ };
	return result;
}

// data-based construct operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpConstruct(a3real3p const translate, a3real3p const rotation, a3real3p const scale)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpConstruct(&result, translate, rotation, scale);
	// done
	return result;
}

// data-based copy operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpCopy(a3_SpatialPose const pose_in)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpCopy(&result, &pose_in);
	// done
	return result;
}

// data-based negate operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpNegate(a3_SpatialPose const pose_in)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpNegate(&result, &pose_in);
	// done
	return result;
}

// data-based concatenate operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpConcatenate(a3_SpatialPose const pose_left, a3_SpatialPose const pose_right)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpConcatenate(&result, &pose_left, &pose_right);
	// done
	return result;
}

// data-based nearest operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpNearest(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpNearest(&result, &pose0, &pose1, u);
	// done
	return result;
}

// data-based LERP
inline a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpLERP(&result, &pose0, &pose1, u);
	// done
	return result;
}

// data-based cubic operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpCubic(a3_SpatialPose const pose0, a3_SpatialPose const pose1,
	a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, const a3real u)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpCubic(&result, &pose0, &pose1, pose2, pose3, u);

	// done
	return result;
}

// data-based deconcatenate operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpDeconcatenate(a3_SpatialPose const pose_left, a3_SpatialPose const pose_right) {
	a3_SpatialPose result = { 0 };

	a3spatialPoseOpDeconcatenate(&result, &pose_left, &pose_right);

	// done
	return result;
}

// data-based scale operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpScale(a3_SpatialPose const pose_in, a3real const u)
{
	a3_SpatialPose result = { 0 };

	a3spatialPoseOpScale(&result, &pose_in, u);

	// done
	return result;
}

// data-based triangular operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpTriangular(a3_SpatialPose const pose0, a3_SpatialPose const pose1,
	a3_SpatialPose const pose2, a3real const u0, a3real const u1)
{
	a3_SpatialPose result = { 0 };

	a3spatialPoseOpTriangular(&result, &pose0, &pose1, &pose2, u0, u1);

	// done
	return result;
}

// data-based bi-nearest operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpBiNearest(
	a3_SpatialPose const pose0_initial, a3_SpatialPose const pose0_terminal,
	a3_SpatialPose const pose1_initial, a3_SpatialPose const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{
	a3_SpatialPose result = { 0 };

	a3spatialPoseOpBiNearest(&result, &pose0_initial, &pose0_terminal, &pose1_initial, &pose1_terminal, u0, u1, u01);

	// done
	return result;
}

// data-based bi-linear operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpBiLinear(
	a3_SpatialPose const pose0_initial, a3_SpatialPose const pose0_terminal,
	a3_SpatialPose const pose1_initial, a3_SpatialPose const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{
	a3_SpatialPose result = { 0 };

	a3spatialPoseOpBiLinear(&result, &pose0_initial, &pose0_terminal, &pose1_initial, &pose1_terminal, u0, u1, u01);

	// done
	return result;
}

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// data-based bi-cubic operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpBiCubic(a3_SpatialPose* pose_out,
	a3_SpatialPose* poseSet, //Array of 16 poses
	a3real* uArray) //Array of 5 a3real3's
{
	a3_SpatialPose result = { 0 };

	// done
	return result;
}


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out, a3ui32 numNodes)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpIdentity(&pose_out->pose[i]);
	}
	// done
	return pose_out;
}

// pointer-based construct operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpConstruct(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3real3p const translate, a3real3p const rotation, a3real3p const scale)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpConstruct(&pose_out->pose[i], translate, rotation, scale);
	}
	// done
	return pose_out;
}

// pointer-based copy operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpCopy(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_in)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpCopy(&pose_out->pose[i], &pose_in->pose[i]);
	}
	// done
	return pose_out;
}

// pointer-based negate operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpNegate(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_in)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpNegate(&pose_out->pose[i], &pose_in->pose[i]);
	}
	// done
	return pose_out;
}

// pointer-based concatenate operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpConcatenate(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_left, a3_HierarchyPose* const pose_right)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpConcatenate(&pose_out->pose[i], &pose_left->pose[i], &pose_right->pose[i]);
	}
	// done
	return pose_out;
}

// pointer-based nearest operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpNearest(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1, a3real const u)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpNearest(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], u);
	}
	// done
	return pose_out;
}

// pointer-based LERP operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1, a3real const u)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpLERP(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], u);
	}
	// done
	return pose_out;
}

// pointer-based cubic operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpCubic(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1,
	a3_HierarchyPose* const pose2, a3_HierarchyPose* const pose3, const a3real u)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpCubic(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], &pose2->pose[i], &pose3->pose[i], u);
	}
	// done
	return pose_out;
}

// pointer-based deconcatenate operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpDeconcatenate(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_left, a3_HierarchyPose* const pose_right)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpDeconcatenate(&pose_out->pose[i], &pose_left->pose[i], &pose_right->pose[i]);
	}

	// done
	return pose_out;
}

// pointer-based scale operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpScale(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_in, a3real const u)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpScale(&pose_out->pose[i], &pose_in->pose[i], u);
	}

	// done
	return pose_out;
}

// pointer-based triangular operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpTriangular(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1,
	a3_HierarchyPose* const pose2, a3real const u0, a3real const u1)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpTriangular(&pose_out->pose[i], &pose0->pose[i], &pose1->pose[i], &pose2->pose[i], u0, u1);
	}

	// done
	return pose_out;
}

// pointer-based bi-nearest operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiNearest(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpBiNearest(&pose_out->pose[i], &pose0_initial->pose[i], &pose0_terminal->pose[i], &pose1_initial->pose[i], &pose1_terminal->pose[i], u0, u1, u01);
	}

	// done
	return pose_out;
}

// pointer-based bi-linear operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiLinear(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpBiLinear(&pose_out->pose[i], &pose0_initial->pose[i], &pose0_terminal->pose[i], &pose1_initial->pose[i], &pose1_terminal->pose[i], u0, u1, u01);
	}

	// done
	return pose_out;
}

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// pointer-based bi-cubic operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiCubic(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* poseSet, //Array of 16 poses
	a3real* uArray) //Array of 5 a3real3's
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		//Collect all spatial poses from hierarchy poses
		a3_SpatialPose sposes[16];
		for (a3ui32 j = 0; j < 16; j++)
		{
			sposes[j] = (poseSet + j)->pose[i];
		}

		a3spatialPoseOpBiCubic(&pose_out->pose[i],
			sposes, uArray);
	}

	// done
	return pose_out;
}

inline void ConstructTriangle(Triangle* tri_out, 
	const a3real pointAX, const a3real pointAY,
	const a3real pointBX, const a3real pointBY,
	const a3real pointCX, const a3real pointCY)
{
	//Edge 1
	tri_out->pointA.x = pointAX;
	tri_out->pointA.y = pointAY;

	//Edge 2
	tri_out->pointB.x = pointBX;
	tri_out->pointB.y = pointBY;

	//Edge 3
	tri_out->pointC.x = pointCX;
	tri_out->pointC.y = pointCY;
}

inline a3i32 TrianglesEquivalent(a3boolean* equal_out, const Triangle* lhs, const Triangle* rhs)
{
	if (lhs && rhs && equal_out)
	{
		//Check each point against all 3 of the other triangle, order doesn't matter
		*equal_out = (
			(CompareVec2(lhs->pointA, rhs->pointA) || CompareVec2(lhs->pointA, rhs->pointB) || CompareVec2(lhs->pointA, rhs->pointC)) &&
			(CompareVec2(lhs->pointB, rhs->pointA) || CompareVec2(lhs->pointB, rhs->pointB) || CompareVec2(lhs->pointB, rhs->pointC)) &&
			(CompareVec2(lhs->pointC, rhs->pointA) || CompareVec2(lhs->pointC, rhs->pointB) || CompareVec2(lhs->pointC, rhs->pointC))
			);

		return 1;
	}

	return -1;
}

inline void PrintTriangle(Triangle* tri)
{
	//Print out 3 points of triangle
	printf("Point A: (%f, %f)   Point B: (%f, %f)   Point C: (%f, %f)\n",
		tri->pointA.x, tri->pointA.y, 
		tri->pointB.x, tri->pointB.y, 
		tri->pointC.x, tri->pointC.y);
}

inline a3i32 ConstructEdgesFromTriangle(Edge* edges_out, const Triangle* tri_in)
{
	if (edges_out && tri_in)
	{
		//Edge 1
		edges_out[0].pointA = tri_in[0].pointA;
		edges_out[0].pointB = tri_in[0].pointB;

		//Edge 2
		edges_out[1].pointA = tri_in[0].pointB;
		edges_out[1].pointB = tri_in[0].pointC;

		//Edge 3
		edges_out[2].pointA = tri_in[0].pointC;
		edges_out[2].pointB = tri_in[0].pointA;

		return 1;
	}

	return -1;
}

inline a3i32 EdgesEquivalent(a3boolean* equal_out, const Edge* lhs, const Edge* rhs)
{
	if (lhs && rhs && equal_out)
	{
		//Check each point against both of the others, values may be flipped
		*equal_out =
			(CompareVec2(lhs->pointA, rhs->pointA) || CompareVec2(lhs->pointA, rhs->pointB)) &&
			(CompareVec2(lhs->pointB, rhs->pointA) || CompareVec2(lhs->pointB, rhs->pointB));

		return 1;
	}

	return -1;
}

// The equation for finding the circumcenter of a triangle was derived from this website
//https://gamedev.stackexchange.com/questions/60630/how-do-i-find-the-circumcenter-of-a-triangle-in-3d
inline a3i32 a3_findCircumcenter(Circumcircle* circum_out, Triangle* tri)
{
	if (circum_out)
	{
		a3vec3 ac = { tri->pointC.x - tri->pointA.x, tri->pointC.y - tri->pointA.y, 0 };
		a3vec3 ab = { tri->pointB.x - tri->pointA.x, tri->pointB.y - tri->pointA.y, 0 };
		a3vec3 abCrossAC;
		a3real3Cross(abCrossAC.v, ab.v, ac.v);

		a3vec3 abCrossACCrossAB;
		a3real3Cross(abCrossACCrossAB.v, abCrossAC.v, ab.v);

		a3vec3 mul1;
		a3real3SetReal3(mul1.v, abCrossACCrossAB.v);
		a3real3MulS(mul1.v, a3real3LengthSquared(ac.v));
		
		a3vec3 acCrossABCrossAC;
		a3real3Cross(acCrossABCrossAC.v, ac.v, abCrossAC.v);

		a3vec3 mul2;
		a3real3SetReal3(mul2.v, acCrossABCrossAC.v);
		a3real3MulS(mul2.v, a3real3LengthSquared(ab.v));

		a3vec3 sum;
		a3real3Sum(sum.v, mul1.v, mul2.v);

		a3real denominator = (a3real)2.0 * a3real3LengthSquared(abCrossAC.v);

		a3vec3 aToCenter;
		a3real3SetReal3(aToCenter.v, sum.v);
		a3real3DivS(aToCenter.v, denominator);

		circum_out->radius = a3real3Length(aToCenter.v);
		
		if (circum_out->radius < 0) { circum_out->radius = circum_out->radius * -1; }

		a3real3 newCenter;
		a3real3Sum(newCenter, tri->pointA.v, aToCenter.v);

		circum_out->center.x = newCenter[0];
		circum_out->center.y = newCenter[1];
		
		return 1;
	}

	return -1;
}

inline a3i32 RemoveTriangleFromArray(Triangle* triArray_out, a3ui32* triCount, const a3ui32* index)
{
	if (triArray_out && triCount)
	{
		//Move all elements past the given index back one space to erase it
		for (a3ui32 i = (a3ui32)(*index); i < *triCount; i++)
		{
			triArray_out[i] = triArray_out[i + 1];
		}

		//Erase the value at the end of the array if there is one
		if (*triCount > 0)
		{
			memset(&triArray_out[*triCount - 1], 0, sizeof(Triangle));
		}

		//Update number of triangles
		*triCount -= 1;

		return 1;
	}

	return -1;
}

inline a3i32 RemoveEdgeFromArray(Edge* edgeArray_out, a3ui32* edgeCount, const a3ui32* index)
{
	if (edgeArray_out && edgeCount)
	{
		//Move all elements past the given index back one space to erase it
		for (a3ui32 i = (a3ui32)(*index); i < *edgeCount; i++)
		{
			edgeArray_out[i] = edgeArray_out[i + 1];
		}

		//Erase the value at the end of the array if there is one
		if (*edgeCount > 0)
		{
			memset(&edgeArray_out[*edgeCount - 1], 0, sizeof(Triangle));
		}

		//Update number of edges
		*edgeCount -= 1;

		return 1;
	}

	return -1;
}

//Compares floats to see if they are reasonably close enough to be considered equal
inline a3boolean CompareFloats(a3real lhs, a3real rhs)
{
	return fabs(lhs - rhs) < .00001f;
}

//Compares vec2s to see if they are reasonably close enough to be considered equal
inline a3boolean CompareVec2(a3vec2 lhs, a3vec2 rhs)
{
	//Uses compare floats to check equality within certain bound
	return CompareFloats(lhs.x, rhs.x) && CompareFloats(lhs.y, rhs.y);
}

inline a3i32 GetIndexOfTriangle(a3i32* index_out, const Triangle* triArray, const a3ui32* triCount, const Triangle* triSearch)
{
	if (triArray && triSearch)
	{
		for (a3ui32 i = 0; i < *triCount; i++)
		{
			//Determine if triangles are the same
			a3boolean equal;
			TrianglesEquivalent(&equal, &triArray[i], triSearch);

			//If triangle is found, store index and break early
			if (equal)
			{
				//Return the given index
				*index_out = (a3i32)i;

				return 1;
			}
		}

		//Return -1 if not found
		*index_out = -1;

		return 1;
	}

	return -1;
}

inline a3i32 GetIndexOfVec2(a3i32* index_out, const a3vec2* vecArray, const a3ui32* vecCount, const a3vec2* vecSearch)
{
	if (vecArray && vecCount)
	{
		for (a3ui32 i = 0; i < *vecCount; i++)
		{
			//Determine if triangles are the same
			a3boolean equal;
			equal = CompareVec2(vecArray[i], *vecSearch);

			//If triangle is found, store index and break early
			if (equal)
			{
				//Return the given index
				*index_out = (a3i32)i;

				return 1;
			}
		}

		//Return -1 if not found
		*index_out = -1;

		return 1;
	}

	return -1;
}

inline a3i32 GetIndexOfEdge(a3i32* index_out, const Edge* edgeArray, const a3ui32* edgeCount, const Edge* edgeSearch)
{
	if (edgeArray && edgeSearch)
	{
		for (a3ui32 i = 0; i < *edgeCount; i++)
		{
			//Determine if edges are the same
			a3boolean equal;
			EdgesEquivalent(&equal, &edgeArray[i], edgeSearch);

			//If triangle is found, store index and break early
			if (equal)
			{
				//Return the given index
				*index_out = (a3i32)i;

				return 1;
			}
		}

		//Return -1 if not found
		*index_out = -1;

		return 1;
	}

	return -1;
}

//Pointer based spatial pose level operation using delaunay blending
inline a3_SpatialPose* a3spatialPoseOPDelaunay(a3_SpatialPose* pose_out,
	const a3_HierarchyPose* poses,	//Poses from which to get poses by indexes in clip controllers
	const a3vec2* pointSet, const a3_ClipController** clipCtrls, const a3ui32* pointCount, //Data per point
	const Triangle* currentTri,	//Currently selected triangle
	const a3real* blends,		//Blend parameter inputs
	const a3ui32 offset
)
{
	//Get pose0		demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex0
	const a3_SpatialPose* poseA0 = (poses + clipCtrls[0]->clipPool->keyframe[clipCtrls[0]->keyframeIndex].sampleIndex0)->pose + offset;
	const a3_SpatialPose* poseB0 = (poses + clipCtrls[1]->clipPool->keyframe[clipCtrls[1]->keyframeIndex].sampleIndex0)->pose + offset;
	const a3_SpatialPose* poseC0 = (poses + clipCtrls[2]->clipPool->keyframe[clipCtrls[2]->keyframeIndex].sampleIndex0)->pose + offset;

	//Get pose1
	const a3_SpatialPose* poseA1 = (poses + clipCtrls[0]->clipPool->keyframe[clipCtrls[0]->keyframeIndex].sampleIndex1)->pose + offset;
	const a3_SpatialPose* poseB1 = (poses + clipCtrls[1]->clipPool->keyframe[clipCtrls[1]->keyframeIndex].sampleIndex1)->pose + offset;
	const a3_SpatialPose* poseC1 = (poses + clipCtrls[2]->clipPool->keyframe[clipCtrls[2]->keyframeIndex].sampleIndex1)->pose + offset;
								   
	//Bilerp poseA, B, and C using their respective keyframe parameters to get the spatial pose at the currnet moment
	a3_SpatialPose pose0, pose1, pose2;
	a3spatialPoseLerp(&pose0, poseA0, poseA1, (a3real)clipCtrls[0]->keyframeParam);
	a3spatialPoseLerp(&pose1, poseB0, poseB1, (a3real)clipCtrls[1]->keyframeParam);
	a3spatialPoseLerp(&pose2, poseC0, poseC1, (a3real)clipCtrls[2]->keyframeParam);

	//Triangular operation on the resulting poses 
	a3spatialPoseOpTriangular(pose_out, &pose0, &pose1, &pose2, blends[0], blends[1]);
	//*pose_out = *poseC0;
	//a3spatialPoseOpCopy(pose_out, poseA0);

	return pose_out;
}

// pointer-based delaunay operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpDelaunay(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	const a3_HierarchyPose* poses,	//Poses from which to get poses by indexes in clip controllers
	const a3vec2* pointSet, const a3_ClipController* clipCtrls, const a3ui32* pointCount, //Data per point
	const Triangle* currentTri,	//Currently selected triangle
	const a3real* blends		//Blend parameter inputs
)
{
	//Find each point from the selected triangle in the point set
	//Get the clip controller associated with each point
	const a3_ClipController* controllers[3];

	//Above process for point A
	a3i32 indexA = -1;
	GetIndexOfVec2(&indexA, pointSet, pointCount, &currentTri->pointA);
	controllers[0] = &clipCtrls[indexA];

	printf("Controller: %s   KeyframeIndex: %i\n", controllers[0]->name, controllers[0]->keyframe->sampleIndex0);

	//Above process for point B
	a3i32 indexB = -1;
	GetIndexOfVec2(&indexB, pointSet, pointCount, &currentTri->pointB);
	controllers[1] = &clipCtrls[indexB];

	//Above process for point C
	a3i32 indexC = -1;
	GetIndexOfVec2(&indexC, pointSet, pointCount, &currentTri->pointC);
	controllers[2] = &clipCtrls[indexC];

	for (a3ui32 i = 0; i < numNodes; i++) {

		a3spatialPoseOPDelaunay(&pose_out->pose[i], poses, pointSet, controllers, pointCount, currentTri, blends, i);
	}

	return pose_out;
}


//-----------------------------------------------------------------------------

// cubic function
inline a3real a3cubic(a3real p0, a3real p1, a3real m0, a3real m1, a3real t) {
	/*return (a3real)(((a3real)2.0 * (a3real)pow(t, (a3real)3.0) - (a3real)3.0 * (a3real)pow(t, (a3real)2.0) + (a3real)1.0) * p0)
		+ (((a3real)pow(t, (a3real)3.0) - (a3real)2.0 * (a3real)pow(t, (a3real)2.0) + t) * m0)
		+ (((a3real)-2.0 * (a3real)pow(t, (a3real)3.0) + (a3real)3.0 * (a3real)pow(t, (a3real)2.0)) * p1)
		+ (((a3real)pow(t, (a3real)3.0) - (a3real)pow(t, (a3real)2.0)) * m1);*/
		/*return (a3real)
			(
				(
					2.0 * pow(t, 3.0) - 3.0 * pow(t, 2.0) + 1.0
					) * p0
				+ (
					pow(t, 3.0) - 2.0 * pow(t, 2.0) + t
					) * m0
				+ (
					-2.0 * pow(t, 3.0) + 3.0 * pow(t, 2.0)
					) * p1
				+ (
					pow(t, 3.0) - pow(t, 2.0)
				) * m1
			);*/

	return (a3real)(
		((-.5 * p0 + 1.5 * p1 - 1.5 * m0 + .5 * m1) * (t * t * t)) +
		((p0 - 2.5 * p1 + 2 * m0 - .5 * m1) * (t * t)) +
		((-.5 * p0 + .5 * m0) * t) +
		p1
		);
}


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H