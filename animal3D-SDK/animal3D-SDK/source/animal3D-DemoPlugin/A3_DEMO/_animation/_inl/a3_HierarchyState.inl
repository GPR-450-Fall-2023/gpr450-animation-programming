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
		//Reset each spatial pose
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3spatialPoseReset(pose_inout[i].sPoses);
		}

		return 1;
	}
	return -1;
}

// convert full hierarchy pose to hierarchy transforms
inline a3i32 a3hierarchyPoseConvert(const a3_HierarchyPose* pose_inout, const a3ui32 nodeCount, const a3_SpatialPoseChannel* channel, const a3_SpatialPoseEulerOrder order)
{
	if (pose_inout && nodeCount)
	{
		//Update transform matrix of every spatial pose in h pose
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			a3spatialPoseConvert(&(pose_inout->sPoses + i)->transform, (pose_inout->sPoses) + 1,
				a3poseChannel_translate_xyz | a3poseChannel_orient_xyz | a3poseChannel_scale_xyz, a3poseEulerOrder_xyz);
		}
	}
	return -1;
}

// copy full hierarchy pose
inline a3i32 a3hierarchyPoseCopy(const a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, const a3ui32 nodeCount)
{
	if (pose_out && pose_in && nodeCount)
	{
		//Copy each spatial pose
		for (a3ui32 i = 0; i < nodeCount; i++)
		{
			//Copying values not addresses
			*(pose_out->sPoses + i) = *(pose_in->sPoses + i);
		}
	}
	return -1;
}

//Lerp between poses
inline a3i32 a3hierarchyPoseLerp(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1,
	const a3real parameter, const a3ui32 numNodes)
{
	if (pose_out && pose0 && pose1 && numNodes)
	{
		///////////////////////////////////////////////
		// TODO - IMPLEMENT
		///////////////////////////////////////////////

		pose_out = (a3_HierarchyPose*)pose0;

		return 1;
	}
	return -1;
}

//
inline a3i32 a3hierarchyPoseConcat(a3_HierarchyPose* pose_out, const a3_HierarchyPose* basePose,
	const a3_HierarchyPose* tempStorage, const a3ui32 numNodes)
{
	if (pose_out && basePose && tempStorage && numNodes)
	{
		///////////////////////////////////////////////
		// TODO - IMPLEMENT
		///////////////////////////////////////////////

		pose_out = (a3_HierarchyPose*)basePose;

		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse object-space matrices
inline a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState *state)
{
	if (state)
	{
		///////////////////////////////////////////////
		// TODO - IMPLEMENT
		///////////////////////////////////////////////

		return 1;
	}
	return -1;
}

//Update objectBindToCurrent matrix
inline a3i32 a3hierarchyStateUpdateObjectBindToCurrent(a3_HierarchyState* activeHS, const a3_HierarchyState* baseHS)
{
	if (activeHS && baseHS)
	{
		///////////////////////////////////////////////
		// TODO - IMPLEMENT
		///////////////////////////////////////////////

		activeHS = (a3_HierarchyState*)baseHS;

		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATE_INL
#endif	// __ANIMAL3D_HIERARCHYSTATE_H