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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

//a3i32 a3hierarchyPoseCreate(a3_HierarchyPose* pose_out, const a3ui32 sPoseCount)
//{
//	if (pose_out && !pose_out->sPoses)
//	{
//		//Allocate memory for spatial poses and set to 0
//		const a3ui32 dataSize = sizeof(a3_SpatialPose) * sPoseCount;
//		a3ui32 i;
//		pose_out->sPoses = (a3_SpatialPose*)malloc(dataSize);
//		memset(pose_out->sPoses, 0, dataSize);
//
//		return 1;
//	}
//	return -1;
//}
//
//
//a3i32 a3hierarchyPoseCeanup(a3_HierarchyPose* pose_out)
//{
//
//	// validate param exists and is initialized
//	if (pose_out && pose_out->sPoses)
//	{
//		// release everything (one free)
//		//free(???);
//		free(pose_out->sPoses);
//
//		// reset pointers
//		pose_out->sPoses = 0;
//
//		// done
//		return 1;
//	}
//	return -1;
//}


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount, const a3_SpatialPoseEulerOrder order)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements

		// allocate everything (one malloc)
		//??? = (...)malloc(sz);
		const a3ui32 dataSize = sizeof(a3_HierarchyPose) * poseCount;
		poseGroup_out->hPoses = (a3_HierarchyPose*)malloc(dataSize);

		if (poseGroup_out->hPoses)
		{
			memset(poseGroup_out->hPoses, 0, dataSize);
		}
		
		
		const a3ui32 spatialSize = sizeof(a3_SpatialPose) * hierarchy->numNodes;
		poseGroup_out->sPoses = (a3_SpatialPose*)malloc(spatialSize);

		if (poseGroup_out->sPoses)
		{
			memset(poseGroup_out->sPoses, 0, spatialSize);
		}
		

		const a3ui32 channelSize = sizeof(a3_SpatialPoseEulerOrder) * hierarchy->numNodes;
		poseGroup_out->channel = (a3_SpatialPoseEulerOrder*)malloc(channelSize);

		if (poseGroup_out->channel)
		{
			memset(poseGroup_out->channel, 0, channelSize);
		}

		// set pointers
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->order = order;

		// reset all data
		poseGroup_out->sPoseCount = hierarchy->numNodes;
		poseGroup_out->hPoseCount = poseCount;

		//Set reference of hierarchy pose to spatial poses
		for (a3ui32 i = 0; i < poseCount; i++)
		{
			//Create each pose group
			a3hierarchyPoseGroupCreate(&poseGroup_out->hPoses[i], poseGroup_out->hierarchy, poseGroup_out->hPoseCount, poseGroup_out->order);
			//(poseGroup_out->hPoses[i].sPoses) = (poseGroup_out->sPoses);
		}

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		//free(???);
		free(poseGroup->sPoses);

		// reset pointers
		poseGroup->hierarchy = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements

		// allocate everything (one malloc)
		//??? = (...)malloc(sz);
		const a3ui32 sampleSize = sizeof(a3_HierarchyPose);
		state_out->objectSpaceBindToCurrent = (a3_HierarchyPose*)malloc(sampleSize);

		const a3ui32 localSize = sizeof(a3_HierarchyPose);
		state_out->localSpace = (a3_HierarchyPose*)malloc(localSize);

		const a3ui32 objectSize = sizeof(a3_HierarchyPose);
		state_out->objectSpace = (a3_HierarchyPose*)malloc(objectSize);

		// set pointers
		state_out->hierarchy = hierarchy;

		// reset all data
		state_out->localSpace = 0;
		state_out->objectSpace = 0;
		state_out->objectSpaceBindToCurrent = 0;

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		// release everything (one free)
		//free(???);
		//free(state->localSpace);
		//free(state->objectSpace);
		//free(state->objectSpaceBindToCurrent);

		// reset pointers
		state->hierarchy = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hPoseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hPoseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}


//-----------------------------------------------------------------------------
