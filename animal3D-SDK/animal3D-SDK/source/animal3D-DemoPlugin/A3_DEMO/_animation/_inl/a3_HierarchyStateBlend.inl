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


#include <stddef.h>
#include <math.h>

#ifdef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_INL
#define __ANIMAL3D_HIERARCHYSTATEBLEND_INL

#include <math.h>


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out)
{
	pose_out->transform = a3mat4_identity;
	// ...

	// done
	return pose_out;
}

// pointer-based construct operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpConstruct(a3_SpatialPose* pose_out, a3real3p const translation, a3real3p const rotation, a3real3p const scale)
{
	a3spatialPoseSetTranslation(pose_out, translation[0], translation[1], translation[2]);
	a3spatialPoseSetRotation(pose_out, rotation[0], rotation[1], rotation[2]);
	a3spatialPoseSetScale(pose_out, scale[0], scale[1], scale[2]);
	// done
	return pose_out;
}

// pointer-based copy operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpCopy(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in)
{
	a3real3 trans, rot, scale;
	trans[0] = pose_in->translation.x;
	trans[1] = pose_in->translation.y;
	trans[2] = pose_in->translation.z;
	rot[0] = pose_in->angles.x;
	rot[1] = pose_in->angles.y;
	rot[2] = pose_in->angles.z;
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
	trans[0] = pose_in->translation.x * (a3real)-1.0;
	trans[1] = pose_in->translation.y * (a3real)-1.0;
	trans[2] = pose_in->translation.z * (a3real)-1.0;
	rot[0] = pose_in->angles.x * (a3real)-1.0;
	rot[1] = pose_in->angles.y * (a3real)-1.0;
	rot[2] = pose_in->angles.z * (a3real)-1.0;
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
	trans[0] = pose_left->translation.x + pose_right->translation.x;
	trans[1] = pose_left->translation.y + pose_right->translation.y;
	trans[2] = pose_left->translation.z + pose_right->translation.z;
	rot[0] = pose_left->angles.x + pose_right->angles.x;
	rot[1] = pose_left->angles.y + pose_right->angles.y;
	rot[2] = pose_left->angles.z + pose_right->angles.z;
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
	trans[0] = (pose1->translation.x * u) + (pose0->translation.x * ((a3real)1.0 - u));
	trans[1] = (pose1->translation.y * u) + (pose0->translation.y * ((a3real)1.0 - u));
	trans[2] = (pose1->translation.z * u) + (pose0->translation.z * ((a3real)1.0 - u));
	rot[0] = (pose1->angles.x * u) + (pose0->angles.x * ((a3real)1.0 - u));
	rot[1] = (pose1->angles.y * u) + (pose0->angles.y * ((a3real)1.0 - u));
	rot[2] = (pose1->angles.z * u) + (pose0->angles.z * ((a3real)1.0 - u));
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
	trans[0] = a3cubic(pose0->translation.x, pose1->translation.x, pose2->translation.x, pose3->translation.x, u);
	trans[1] = a3cubic(pose0->translation.y, pose1->translation.y, pose2->translation.y, pose3->translation.y, u);
	trans[2] = a3cubic(pose0->translation.z, pose1->translation.z, pose2->translation.z, pose3->translation.z, u);
	rot[0] = a3cubic(pose0->angles.x, pose1->angles.x, pose2->angles.x, pose3->angles.x, u);
	rot[1] = a3cubic(pose0->angles.y, pose1->angles.y, pose2->angles.y, pose3->angles.y, u);
	rot[2] = a3cubic(pose0->angles.z, pose1->angles.z, pose2->angles.z, pose3->angles.z, u);
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
	//a3real3 translation;
	//a3real3 rotation;
	//a3real3 scale;

	//translation[0] = pose_left->translation.x - pose_right->translation.x;
	//translation[1] = pose_left->translation.y - pose_right->translation.y;
	//translation[2] = pose_left->translation.z - pose_right->translation.z;

	//rotation[0] = pose_left->angles.x - pose_right->angles.x;
	//rotation[1] = pose_left->angles.y - pose_right->angles.y;
	//rotation[2] = pose_left->angles.z - pose_right->angles.z;

	//scale[0] = pose_left->scale.x - pose_right->scale.x;
	//scale[1] = pose_left->scale.y - pose_right->scale.y;
	//scale[2] = pose_left->scale.z - pose_right->scale.z;

	//a3spatialPoseOpConstruct(pose_out, translation, rotation, scale);

	a3_SpatialPose rightArg = a3spatialPoseDOpNegate(*pose_right);
	a3spatialPoseOpConcatenate(pose_out, pose_left, &rightArg);

	// done
	return pose_out;
}

// pointer-based scale operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpScale(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in, const a3real u)
{
	a3real3 translation;
	a3real3 rotation;
	a3real3 scale;

	translation[0] = pose_in->translation.x * u;
	translation[1] = pose_in->translation.y * u;
	translation[2] = pose_in->translation.z * u;

	rotation[0] = pose_in->angles.x * u;
	rotation[1] = pose_in->angles.y * u;
	rotation[2] = pose_in->angles.z * u;

	scale[0] = powf(pose_in->scale.x, u);
	scale[1] = powf(pose_in->scale.y, u);
	scale[2] = powf(pose_in->scale.z, u);

	a3spatialPoseOpConstruct(pose_out, translation, rotation, scale);

	// done
	return pose_out;
}

// pointer-based triangular operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpTriangular(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1,
	a3_SpatialPose const* pose2, const a3real u0, const a3real u1)
{
	a3_SpatialPose firstLerp = a3spatialPoseDOpLERP(*pose0, *pose1, u0);
	a3spatialPoseOpLERP(pose_out, &firstLerp, pose2, u1);

	// done
	return pose_out;
}

// pointer-based bi-nearest operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpBiNearest(a3_SpatialPose* pose_out,
	a3_SpatialPose const* pose0_initial, a3_SpatialPose const* pose0_terminal,
	a3_SpatialPose const* pose1_initial, a3_SpatialPose const* pose1_terminal,
	const a3real u0, const a3real u1, const a3real u01)
{

	// done
	return pose_out;
}

// pointer-based bi-linear operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpBiLinear(a3_SpatialPose* pose_out,
	a3_SpatialPose const* pose0_initial, a3_SpatialPose const* pose0_terminal,
	a3_SpatialPose const* pose1_initial, a3_SpatialPose const* pose1_terminal,
	const a3real u0, const a3real u1, const a3real u01)
{

	// done
	return pose_out;
}

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// pointer-based bi-cubic operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpBiCubic(a3_SpatialPose* pose_out,
	const a3_SpatialPose* poseSet0, //Array of 4 poses
	const a3_SpatialPose* poseSet1, //Array of 4 poses
	const a3_SpatialPose* poseSet2, //Array of 4 poses
	const a3_SpatialPose* poseSet3, //Array of 4 poses
	const a3real3* uArray) //Array of 5 a3real3's
{

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
inline a3_SpatialPose a3spatialPoseDOpConstruct(a3real3p const translation, a3real3p const rotation, a3real3p const scale)
{
	a3_SpatialPose result = { 0 };
	a3spatialPoseOpConstruct(&result, translation, rotation, scale);
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
	// ...

	// done
	return result;
}

// data-based scale operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpScale(a3_SpatialPose const pose_in, a3real const u)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}

// data-based triangular operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpTriangular(a3_SpatialPose const pose0, a3_SpatialPose const pose1,
	a3_SpatialPose const pose2, a3real const u0, a3real const u1)
{
	a3_SpatialPose result = { 0 };
	// ...

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
	// ...

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
	// ...

	// done
	return result;
}

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// data-based bi-cubic operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpBiCubic(
	a3_SpatialPose const* poseSet0, //Array of 4 poses
	a3_SpatialPose const* poseSet1, //Array of 4 poses
	a3_SpatialPose const* poseSet2, //Array of 4 poses
	a3_SpatialPose const* poseSet3, //Array of 4 poses
	a3real3 const* uArray) //Array of 5 a3real3's
{
	a3_SpatialPose result = { 0 };
	// ...

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
inline a3_HierarchyPose* a3hierarchyPoseOpConstruct(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3real3p const translation, a3real3p const rotation, a3real3p const scale)
{
	for (a3ui32 i = 0; i < numNodes; i++) {
		a3spatialPoseOpConstruct(&pose_out->pose[i], translation, rotation, scale);
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

	// done
	return pose_out;
}

// pointer-based scale operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpScale(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose_in, a3real const u)
{

	// done
	return pose_out;
}

// pointer-based triangular operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpTriangular(a3_HierarchyPose* pose_out, a3ui32 numNodes, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1,
	a3_HierarchyPose* const pose2, a3real const u0, a3real const u1)
{

	// done
	return pose_out;
}

// pointer-based bi-nearest operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiNearest(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{

	// done
	return pose_out;
}

// pointer-based bi-linear operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiLinear(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{

	// done
	return pose_out;
}

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// pointer-based bi-cubic operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseOpBiCubic(a3_HierarchyPose* pose_out, a3ui32 numNodes,
	a3_HierarchyPose* const* poseSet0, //Array of 4 poses
	a3_HierarchyPose* const* poseSet1, //Array of 4 poses
	a3_HierarchyPose* const* poseSet2, //Array of 4 poses
	a3_HierarchyPose* const* poseSet3, //Array of 4 poses
	a3real3 const* uArray) //Array of 5 a3real3's
{

	// done
	return pose_out;
}

// The equation for finding the circumcenter of a triangle was derived from this website
//https://gamedev.stackexchange.com/questions/60630/how-do-i-find-the-circumcenter-of-a-triangle-in-3d
inline a3i32 a3_findCircumcenter(Circumcircle* circum_out, Triangle* tri)
{
	if (circum_out)
	{
		//a3real3 aToC;
		//aToC[0] = tri->pointC[0] - tri->pointA[0];
		//aToC[1] = tri->pointC[1] - tri->pointA[1];
		//aToC[2] = tri->pointC[2] - tri->pointA[2];

		//a3real acMag = a3real3LengthSquared(aToC);

		//a3real3 aTob;
		//aTob[0] = tri->pointB[0] - tri->pointA[0];
		//aTob[1] = tri->pointB[1] - tri->pointA[1];
		//aTob[2] = tri->pointB[2] - tri->pointA[2];
		//
		//a3real abMag = a3real3LengthSquared(aTob);

		//a3real3 crossed;
		//a3real3Cross(crossed, aToC, aTob);

		//a3real crossedMag = a3real3LengthSquared(crossed);


		//a3real3 crossedAB;
		//a3real3Cross(crossedAB, crossed, aTob);
		//a3real3MulS(crossedAB, acMag);
		//a3real3 acCrossed;
		//a3real3Cross(acCrossed, aToC, crossed);
		//a3real3MulS(acCrossed, abMag);

		//a3real3 crossedSums;
		//a3real3Sum(crossedSums, crossedAB, acCrossed);
		//
		//a3real3 aToCenter;
		//a3real2DivS(crossedSums, (crossedMag * 2.0));
		//
		////Set Center
		//a3real3Sum(circum_out->center, tri->pointA, aToCenter);

		////Set radius
		//circum_out->radius = a3real3Length(aToCenter);

		a3real2 abMid;
		a3real2Set(abMid, (tri->pointA[0] + tri->pointB[0]) / (a3real)2.0, (tri->pointA[1] + tri->pointB[1]) / (a3real)2.0);

		a3real2 bcMid;
		a3real2Set(bcMid, (tri->pointC[0] + tri->pointB[0]) / (a3real)2.0, (tri->pointC[1] + tri->pointB[1]) / (a3real)2.0);

		a3real abSlope = (tri->pointB[0] - tri->pointA[0]) / (tri->pointB[1] - tri->pointA[1]);
		a3real bcSlope = (tri->pointC[0] - tri->pointB[0]) / (tri->pointC[1] - tri->pointB[1]);

		//Expanded formula for solving for x using point-slope equation
		circum_out->center[0] = ((abSlope * abMid[0]) - abMid[1] - (bcSlope * bcMid[0]) + bcMid[1]) / (abSlope - bcSlope);
		circum_out->center[1] = (abSlope * (circum_out->center[0] - abMid[0])) + abMid[1];

		a3real2 diff;
		diff[0] = circum_out->center[0] - tri->pointA[0];
		diff[1] = circum_out->center[1] - tri->pointA[1];

		circum_out->radius = a3real2Length(diff);

		return 1;
	}

	return -1;
}

//Given a set of points, calculate the triangulation of said points and return the triangles in that triangulation
inline a3i32 a3_calculateDelaunayTriangulation(Triangle* triArray_out, const a3vec2* triSet, const a3real* triCount)
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


}


//-----------------------------------------------------------------------------

// cubic function
inline a3real a3cubic(a3real p0, a3real p1, a3real m0, a3real m1, a3real t) {
	/*return (a3real)(((a3real)2.0 * (a3real)pow(t, (a3real)3.0) - (a3real)3.0 * (a3real)pow(t, (a3real)2.0) + (a3real)1.0) * p0)
		+ (((a3real)pow(t, (a3real)3.0) - (a3real)2.0 * (a3real)pow(t, (a3real)2.0) + t) * m0)
		+ (((a3real)-2.0 * (a3real)pow(t, (a3real)3.0) + (a3real)3.0 * (a3real)pow(t, (a3real)2.0)) * p1)
		+ (((a3real)pow(t, (a3real)3.0) - (a3real)pow(t, (a3real)2.0)) * m1);*/
	return (a3real)
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
		);
}

#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H