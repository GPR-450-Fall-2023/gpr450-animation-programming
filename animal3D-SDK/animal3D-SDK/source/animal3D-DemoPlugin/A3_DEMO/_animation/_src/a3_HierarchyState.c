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

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_DemoMode1_Animation-idle-update.c
	Implemented logic for functions relating to hierarchy states and hierarchy poses
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>

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
		//One hierarchy pose struct per hpose
		const a3ui32 dataSize = sizeof(a3_HierarchyPose) * poseCount
			+ sizeof(a3_SpatialPose) * hierarchy->numNodes * poseCount
			+ sizeof(a3_SpatialPoseChannel) * hierarchy->numNodes;
		poseGroup_out->hPoses = (a3_HierarchyPose*)malloc(dataSize);

		// set pointers

		//Cast to hierarchy pose for correct byte size, add poseCount number of hierarchies to starting address, cast to spatial pose
		poseGroup_out->sPoses = (a3_SpatialPose*)((a3_HierarchyPose*)poseGroup_out->hPoses + poseCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)((a3_SpatialPose*)poseGroup_out->sPoses + (poseCount * hierarchy->numNodes));
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->order = order;
		
		// reset all data
		poseGroup_out->sPoseCount = hierarchy->numNodes;
		poseGroup_out->hPoseCount = poseCount;

		//Initialize every spatial pose
		for (a3ui32 sIndex = 0; sIndex < poseCount * hierarchy->numNodes; sIndex++)
		{
			//Default values
			a3spatialPoseInit(poseGroup_out->sPoses + sIndex, 0, 0, 0);
		}

		//Initialize every hierarchy pose
		for (a3ui32 hIndex = 0; hIndex < poseCount; hIndex++)
		{
			//At each iteration, it references the next group of spatial poses of length numNodes
			(poseGroup_out->hPoses + hIndex)->sPoses = (poseGroup_out->sPoses + (hIndex * hierarchy->numNodes));
		}

		//Initialize channels
		for (a3ui32 cIndex = 0; cIndex < hierarchy->numNodes; cIndex++)
		{
			*(poseGroup_out->channel) = a3poseChannel_translate_xyz | a3poseChannel_orient_xyz | a3poseChannel_scale_xyz;
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
		free(poseGroup->hPoses);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->sPoses = 0;

		// done
		return 1;
	}
	return -1;
}

//-----------------------------------------------------------------------------

//Initialize hierarchy pose
a3i32 a3hierarchyPoseInitialize(a3_HierarchyPose* pose_inout, const a3_SpatialPose* sPoses)
{
	if (pose_inout && sPoses)
	{
		//Cast from const to normal pointer
		pose_inout->sPoses = (a3_SpatialPose*)sPoses;

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
		const a3ui32 NUM_POSES = 3; // number of poses we are creating
		const a3ui32 hSize = sizeof(a3_HierarchyPose);
		const a3ui32 sSize = sizeof(a3_SpatialPose) * hierarchy->numNodes;
		const a3ui32 dataSize = (hSize + sSize);

		// allocate everything (one malloc)
		//Spatial pose array is of length hierarchy->numNodes and we have 3 of them
		state_out->objectSpaceBindToCurrent = (a3_HierarchyPose*)malloc(dataSize * NUM_POSES);

		//Initialize to 0
		if (state_out->objectSpaceBindToCurrent)
		{
			memset(state_out->objectSpaceBindToCurrent, 0, dataSize * NUM_POSES);
		}

		// set pointers
		state_out->hierarchy = hierarchy;

		state_out->objectSpaceBindToCurrent->sPoses = (a3_SpatialPose*)((a3byte*)state_out->objectSpaceBindToCurrent + hSize); //Skip over hierarchy
		state_out->localSpace = (a3_HierarchyPose*)((a3byte*)state_out->objectSpaceBindToCurrent->sPoses + sSize); //Skip over spatial pose array
		state_out->localSpace->sPoses = (a3_SpatialPose*)((a3byte*)state_out->localSpace + hSize); //Skip over spatial pose array
		state_out->objectSpace = (a3_HierarchyPose*)((a3byte*)state_out->localSpace->sPoses + sSize); //Two hposes past objectSpaceBindToCurrent
		state_out->objectSpace->sPoses = (a3_SpatialPose*)((a3byte*)state_out->objectSpace + hSize); //Two hposes past objectSpaceBindToCurrent

		// reset all data
		a3hierarchyPoseInitialize(state_out->objectSpaceBindToCurrent, 0);
		a3hierarchyPoseInitialize(state_out->localSpace, 0);
		a3hierarchyPoseInitialize(state_out->objectSpace, 0);

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
		free(state->objectSpaceBindToCurrent);

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
	if (poseGroup_out == NULL) {
		printf("Error with poseGroup_out\n");
	}
	if (hierarchy_out == NULL) {
		printf("Error with hierarchy_out\n");
	}
	FILE* fptr;
	fptr = fopen(resourceFilePath, "r");
	if (fptr == NULL) {
		printf("Error with resourceFilePath");
	}

	//if (poseGroup_out && !poseGroup_out->hPoseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{
		a3byte line[200];
		const a3byte tab[2]	=	"	";
		const a3byte space[2] = " ";
		a3byte* tok;

		a3byte header[50];

		a3i32 numSegments;

		while (fgets(line, 200, fptr)) {
			if (line[0] != '#') {
				if (line[0] == '[') {
					if (line[1] == 'H') {							//[Header]
						strcpy(header, "Header");
					}
					else if (line[1] == 'S' && line[2] == 'e') {	//[SegmentNames&Hierarchy]
						strcpy(header, "SegmentNames&Hierarchy");
					}
					else if (line[1] == 'B') {						//[BasePosition]
						strcpy(header, "BasePosition");
					}
					else if (line[1] == 'E') {						//[EndOfFile]
						fclose(fptr);
						return 1;
					}
					else {											//[Bone Name]
						tok = strtok(line, "[");
						tok = strtok(tok, "]");
						strcpy(header, tok);
					}
				}
				else {
					if (header[0] == 'H') {								//Header
						tok = strtok(line, space);
						if (tok[0] == 'F' && tok[4] == 'T') {				//FileType
							
						}
						else if (tok[0] == 'D' && tok[4] == 'T') {			//DataType
							
						}
						else if (tok[0] == 'F' && tok[4] == 'V') {			//FileVersion
							
						}
						else if (tok[0] == 'N' && tok[3] == 'S') {			//NumSegments
							numSegments = atoi(strtok(0, "\n"));
						}
						else if (tok[0] == 'N' && tok[3] == 'F') {			//NumFrames
							
						}
						else if (tok[0] == 'D' && tok[4] == 'F') {			//DataFrameRate
							
						}
						else if (tok[0] == 'E') {							//EulerRotationOrder
							
						}
						else if (tok[0] == 'C') {							//CalibrationUnits
							
						}
						else if (tok[0] == 'R') {							//RotationUnits
							
						}
						else if (tok[0] == 'G') {							//GlobalAxisofGravity
							
						}
						else if (tok[0] == 'B') {							//BoneLengthAxis
							
						}
						else if (tok[0] == 'S') {							//ScaleFactor
							
						}

					}
					else if (header[0] == 'S' && header[1] == 'e') {	//SegmentNames&Hierarchy
						
					}
					else if (header[0] == 'B') {						//BasePosition
						
					}
					else {												//Bone Name
						
					}
				}
			}
		}
		//tok = strtok(line, s);
		//tok = strtok(0, s);
		//strcpy(fileData[lineCount][0], tok);
		//tok = strtok(0, s);
		//strcpy(fileData[lineCount][1], tok);
		//tok = strtok(0, s);
		//strcpy(fileData[lineCount][2], tok);
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
