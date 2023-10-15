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

	// done
	return pose_out;
}

// pointer-based copy operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpCopy(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in)
{

	// done
	return pose_out;
}
// pointer-based negate operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpNegate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in)
{

	// done
	return pose_out;
}

// pointer-based concatenate operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpConcatenate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_left, a3_SpatialPose const* pose_right)
{

	// done
	return pose_out;
}

// pointer-based nearest operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpNearest(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, const a3real u)
{

	// done
	return pose_out;
}

// pointer-based LERP operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{

	// done
	return pose_out;
}

// pointer-based cubic operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpCubic(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1,
	a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, const a3real u)
{

	// done
	return pose_out;
}

// pointer-based deconcatenate operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpDeconcatenate(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_left, a3_SpatialPose const* pose_right)
{

	// done
	return pose_out;
}

// pointer-based scale operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpScale(a3_SpatialPose* pose_out, a3_SpatialPose const* pose_in, const a3real u)
{

	// done
	return pose_out;
}

// pointer-based triangular operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpTriangular(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1,
	a3_SpatialPose const* pose2, const a3real u0, const a3real u1)
{

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
	// ...

	// done
	return result;
}

// data-based copy operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpCopy(a3_SpatialPose const pose_in)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}

// data-based negate operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpNegate(a3_SpatialPose const pose_in)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}

// data-based concatenate operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpConcatenate(a3_SpatialPose const pose_left, a3_SpatialPose const pose_right)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}

// data-based nearest operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpNearest(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}

// data-based LERP
inline a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}

// data-based cubic operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpCubic(a3_SpatialPose const pose0, a3_SpatialPose const pose1,
	a3_SpatialPose const* pose2, a3_SpatialPose const* pose3, const a3real u)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}

// data-based deconcatenate operation for single spatial pose
inline a3_SpatialPose a3spatialPoseDOpDeconcatenate(a3_SpatialPose const pose_left, a3_SpatialPose const pose_right);

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
inline a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out)
{

	// done
	return pose_out;
}

// pointer-based construct operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpConstruct(a3_HierarchyPose* pose_out, a3real3p const translation, a3real3p const rotation, a3real3p const scale)
{

	// done
	return pose_out;
}

// pointer-based copy operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpCopy(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in)
{

	// done
	return pose_out;
}

// pointer-based negate operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpNegate(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in)
{

	// done
	return pose_out;
}

// pointer-based concatenate operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpConcatenate(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_left, a3_HierarchyPose* const pose_right)
{

	// done
	return pose_out;
}

// pointer-based nearest operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpNearest(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1, a3real const u)
{

	// done
	return pose_out;
}

// pointer-based LERP operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u)
{

	// done
	return pose_out;
}

// pointer-based cubic operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpCubic(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1,
	a3_HierarchyPose* const* pose2, a3_HierarchyPose* const* pose3, const a3real u)
{

	// done
	return pose_out;
}

// pointer-based deconcatenate operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpDeconcatenate(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_left, a3_HierarchyPose* const pose_right)
{

	// done
	return pose_out;
}

// pointer-based scale operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpScale(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose_in, a3real const u)
{

	// done
	return pose_out;
}

// pointer-based triangular operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpTriangular(a3_HierarchyPose* pose_out, a3_HierarchyPose* const pose0, a3_HierarchyPose* const pose1,
	a3_HierarchyPose* const pose2, a3real const u0, a3real const u1)
{

	// done
	return pose_out;
}

// pointer-based bi-nearest operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpBiNearest(a3_HierarchyPose* pose_out,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{

	// done
	return pose_out;
}

// pointer-based bi-linear operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpBiLinear(a3_HierarchyPose* pose_out,
	a3_HierarchyPose* const pose0_initial, a3_HierarchyPose* const pose0_terminal,
	a3_HierarchyPose* const pose1_initial, a3_HierarchyPose* const pose1_terminal,
	a3real const u0, a3real const u1, a3real const u01)
{

	// done
	return pose_out;
}

//NOTE - Organize this however you want, I just need it here for drafting the testing interface - Dillon
// pointer-based bi-cubic operation for single hierarchy pose
inline a3_HierarchyPose* a3hierarchyPoseDOpBiCubic(a3_HierarchyPose* pose_out,
	a3_HierarchyPose* const* poseSet0, //Array of 4 poses
	a3_HierarchyPose* const* poseSet1, //Array of 4 poses
	a3_HierarchyPose* const* poseSet2, //Array of 4 poses
	a3_HierarchyPose* const* poseSet3, //Array of 4 poses
	a3real3 const* uArray) //Array of 5 a3real3's
{

	// done
	return pose_out;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H