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
	
	a3_HierarchyState.inl
	Implementation of inline transform hierarchy state operations.
*/


#ifdef __ANIMAL3D_HIERARCHYSTATE_H
#ifndef __ANIMAL3D_HIERARCHYSTATE_INL
#define __ANIMAL3D_HIERARCHYSTATE_INL


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

// get offset to hierarchy pose in contiguous set
inline a3i32 a3hierarchyPoseGroupGetPoseOffsetIndex(const a3_HierarchyPoseGroup *poseGroup, const a3ui32 poseIndex)
{
	if (poseGroup && poseGroup->hierarchy)
		return (poseIndex * poseGroup->hierarchy->numNodes);
	return -1;
}

// get offset to single node pose in contiguous set
inline a3i32 a3hierarchyPoseGroupGetNodePoseOffsetIndex(const a3_HierarchyPoseGroup *poseGroup, const a3ui32 poseIndex, const a3ui32 nodeIndex)
{
	if (poseGroup && poseGroup->hierarchy)
		return (poseIndex * poseGroup->hierarchy->numNodes + nodeIndex);
	return -1;
}

//-----------------------------------------------------------------------------

// reset full hierarchy pose
inline a3i32 a3hierarchyPoseReset(const a3_HierarchyPose* pose_inout, const a3ui32 nodeCount)
{
	if (pose_inout && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseReset(pose_inout->pose + i);
		return i;
	}
	return -1;
}

// convert full hierarchy pose to hierarchy transforms
inline a3i32 a3hierarchyPoseConvert(const a3_HierarchyPose* pose_inout, const a3ui32 nodeCount, const a3_SpatialPoseChannel* channel, 
	const a3_SpatialPoseEulerOrder* order, const a3_RootMotionFlag rootFlag)
{
	if (pose_inout && nodeCount && channel)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			if (i == 1)
				a3spatialPoseConvert(pose_inout->pose + i, channel[i], order[i], rootFlag);
			else
				a3spatialPoseConvert(pose_inout->pose + i, channel[i], order[i], a3root_All);
		return i;
	}
	return -1;
}

// restore full hierarchy pose from hierarchy transforms
inline a3i32 a3hierarchyPoseRestore(const a3_HierarchyPose* pose_inout, const a3ui32 nodeCount, const a3_SpatialPoseChannel* channel, const a3_SpatialPoseEulerOrder* order)
{
	if (pose_inout && nodeCount && channel)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseRestore(pose_inout->pose + i, channel[i], order[i]);
		return i;
	}
	return -1;
}

// copy full hierarchy pose
inline a3i32 a3hierarchyPoseCopy(const a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, const a3ui32 nodeCount)
{
	if (pose_out && pose_in && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseCopy(pose_out->pose + i, pose_in->pose + i);
		return i;
	}
	return -1;
}

// concat full hierarchy pose
inline a3i32 a3hierarchyPoseConcat(const a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_lhs, const a3_HierarchyPose* pose_rhs, const a3ui32 nodeCount)
{
	if (pose_out && pose_lhs && pose_rhs && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseConcat(pose_out->pose + i, pose_lhs->pose + i, pose_rhs->pose + i);
		return i;
	}
	return -1;
}

// deconcat full hierarchy pose
inline a3i32 a3hierarchyPoseDeconcat(const a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_lhs, const a3_HierarchyPose* pose_rhs, const a3ui32 nodeCount)
{
	if (pose_out && pose_lhs && pose_rhs && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseDeconcat(pose_out->pose + i, pose_lhs->pose + i, pose_rhs->pose + i);
		return i;
	}
	return -1;
}

// lerp full hierarchy pose
inline a3i32 a3hierarchyPoseLerp(const a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_0, const a3_HierarchyPose* pose_1, const a3real u, const a3ui32 nodeCount)
{
	if (pose_out && pose_0 && pose_1 && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseLerp(pose_out->pose + i, pose_0->pose + i, pose_1->pose + i, u);
		return i;
	}
	return -1;
}


// update inverse local-space matrices
inline a3i32 a3hierarchyStateUpdateLocalInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i;
		for (i = 0; i < state->hierarchy->numNodes; ++i)
			a3real4x4TransformInverse(state->localSpaceInv->pose[i].transformMat.m,
				state->localSpace->pose[i].transformMat.m);
		return i;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse object-space matrices
inline a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState *state)
{
	if (state && state->hierarchy)
	{
		a3index i;
		for (i = 0; i < state->hierarchy->numNodes; ++i)
			a3real4x4TransformInverse(state->objectSpaceInv->pose[i].transformMat.m,
				state->objectSpace->pose[i].transformMat.m);
		return i;
	}
	return -1;
}

// update inverse object-space bind-to-current matrices
inline a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState* state, const a3_HierarchyState* state_bind)
{
	if (state && state->hierarchy && state_bind && state_bind->hierarchy)
	{
		a3index i;
		for (i = 0; i < state->hierarchy->numNodes; ++i)
			a3real4x4Product(state->objectSpaceBindToCurrent->pose[i].transformMat.m,
				state->objectSpace->pose[i].transformMat.m,
				state_bind->objectSpaceInv->pose[i].transformMat.m);
		return i;
	}
	return -1;
}


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATE_INL
#endif	// __ANIMAL3D_HIERARCHYSTATE_H