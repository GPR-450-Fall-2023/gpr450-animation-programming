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
		const a3byte tab[2]	=		"	";
		const a3byte space[2] =		" ";
		const a3byte newline[2] =	"\n";
		a3byte* tok;

		a3byte header[50] = "Header";

		a3i32 numSegments = 0;
		a3i32 numFrames  = 0;
		a3f32 frameRate = 1;
		a3f32 scaleFactor = 1;

		a3i32 poseIndex = 0;

		a3i32 jointIndex = 0;
		a3i32 jointParentIndex = -1;
		a3byte jointName[50];
		a3byte jointParentName[50];

		a3f32 translationxyz[3] = { 0,0,0 };
		a3boolean degrees = true;
		a3real radToDeg = (a3f32)180 / (a3f32)3.14159;
		a3f32 rotationxyz[3] = { 0,0,0 };
		a3f32 boneLength = 1;
		a3byte boneLengthAxis = 'X';

		a3_SpatialPoseEulerOrder eulerRotationOrder = 0;
		a3_SpatialPose* spatialPose;

		while (header[0] == 'H') {								//[Header]
			fgets(line, 200, fptr);
			if (line[0] != '#' && line[0] == '[') {
				if (line[1] == 'S' && line[2] == 'e') {	//[SegmentNames&Hierarchy]
					strcpy(header, "SegmentNames&Hierarchy");
				}
			}
			if (line[0] != '#' && line[0] != '[') {
				tok = strtok(line, space);
				if (tok[0] == 'F' && tok[4] == 'T') {				//FileType
				}
				else if (tok[0] == 'D' && tok[4] == 'T') {			//DataType
				}
				else if (tok[0] == 'F' && tok[4] == 'V') {			//FileVersion
				}
				else if (tok[0] == 'N' && tok[3] == 'S') {			//NumSegments
					numSegments = atoi(strtok(0, newline));
				}
				else if (tok[0] == 'N' && tok[3] == 'F') {			//NumFrames
					numFrames = atoi(strtok(0, newline));
					numFrames++;
				}
				else if (tok[0] == 'D' && tok[4] == 'F') {			//DataFrameRate
					tok = strtok(0, newline);
					frameRate = (a3f32)atof(tok);
				}
				else if (tok[0] == 'E') {							//EulerRotationOrder
					tok = strtok(0, newline);
					if (tok[0] == 'X') {
						if (tok[1] == 'Y') {
							eulerRotationOrder = 0;						//XYZ
						}
						else if (tok[1] == 'Z') {
							eulerRotationOrder = 4;						//XZY
						}
						else {
							printf("Error under Euler Rotation Order\n");
						}
					}
					else if (tok[0] == 'Y') {
						if (tok[1] == 'X') {
							eulerRotationOrder = 3;						//YXZ
						}
						else if (tok[1] == 'Z') {
							eulerRotationOrder = 1;						//YZX
						}
						else {
							printf("Error under Euler Rotation Order\n");
						}
					}
					else if (tok[0] == 'Z') {
						if (tok[1] == 'X') {
							eulerRotationOrder = a3poseEulerOrder_zxy;						//ZXY
						}
						else if (tok[1] == 'Y') {
							eulerRotationOrder = a3poseEulerOrder_zyx;						//ZYX
						}
						else {
							printf("Error under Euler Rotation Order\n");
						}
					}
					else {
						printf("Error under Euler Rotation Order\n");
					}
				}
				else if (tok[0] == 'C') {							//CalibrationUnits
				}
				else if (tok[0] == 'R') {							//RotationUnits
					tok = strtok(0, newline);
					if (tok[0] == 'D') {
						degrees = true;
					}
					else if (tok[0] == 'R') {
						degrees = false;
					}
					else {
						printf("Error under Rotation Units\n");
					}
				}
				else if (tok[0] == 'G') {							//GlobalAxisofGravity
				}
				else if (tok[0] == 'B') {							//BoneLengthAxis
					tok = strtok(0, newline);
					boneLengthAxis = tok[0];
				}
				else if (tok[0] == 'S') {							//ScaleFactor
					tok = strtok(0, newline);
					scaleFactor = (a3f32)atof(tok);
				}
				else {
					printf("Error under [Header]\n");
				}
			}
		}
		//a3byte fileData[numSegments][numFrames][8][50];
		a3hierarchyCreate(hierarchy_out, numSegments, 0);
		a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, numFrames, eulerRotationOrder);

		//a3byte parentData[100][2][50];
		//a3byte fileData[100][100][8][50];

		while (fgets(line, 200, fptr)) {
			//printf(line);
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
					if (header[0] == 'S' && header[1] == 'e') {			//SegmentNames&Hierarchy
						tok = strtok(line, tab);							//Object Name
						strcpy(jointName, tok);
						tok = strtok(0, newline);							//Parent Object Name
						strcpy(jointParentName, tok);
						jointParentIndex = a3hierarchyGetNodeIndex(hierarchy_out, jointParentName);
						a3hierarchySetNode(hierarchy_out, jointIndex++, jointParentIndex, jointName);
					}
					else if (header[0] == 'B') {						//BasePosition
						tok = strtok(line, tab);							//Object Name
						strcpy(jointName, tok);
						tok = strtok(0, tab);								//X Translation
						translationxyz[0] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Y Translation
						translationxyz[1] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Z Translation
						translationxyz[2] = (a3real)atof(tok);
						tok = strtok(0, tab);								//X Rotation
						rotationxyz[0] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Y Rotation
						rotationxyz[1] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Z Rotation
						rotationxyz[2] = (a3real)atof(tok);
						tok = strtok(0, newline);							//Bone Length
						boneLength = scaleFactor * (a3real)atof(tok);

						jointIndex = a3hierarchyGetNodeIndex(hierarchy_out, jointName);
						spatialPose = poseGroup_out->hPoses[0].sPoses + jointIndex;

						a3spatialPoseSetTranslation(spatialPose, translationxyz[0], translationxyz[1], translationxyz[2]);
						
						if (!degrees) {
							rotationxyz[0] = rotationxyz[0] * (a3f32)180 / (a3f32)3.14159;
							rotationxyz[1] = rotationxyz[1] * (a3f32)180 / (a3f32)3.14159;
							rotationxyz[2] = rotationxyz[2] * (a3f32)180 / (a3f32)3.14159;
						}
						a3spatialPoseSetRotation(spatialPose, rotationxyz[0], rotationxyz[1], rotationxyz[2]);

						if (boneLengthAxis == 'X') {
							a3spatialPoseSetScale(spatialPose, boneLength, 1, 1);
						}
						else if (boneLengthAxis == 'Y') {
							a3spatialPoseSetScale(spatialPose, 1, boneLength, 1);
						}
						else if (boneLengthAxis == 'Z') {
							a3spatialPoseSetScale(spatialPose, 1, 1, boneLength);
						}

						poseGroup_out->channel[jointIndex] = a3poseChannel_orient_xyz | a3poseChannel_scale_xyz | a3poseChannel_translate_xyz;

						//a3spatialPoseConvert(&spatialPose->transform, spatialPose, poseGroup_out->channel[jointIndex], poseGroup_out->order);
					}
					else {												//Bone Specific Pose Data
						strcpy(jointName, header);							//Object Name
						tok = strtok(line, tab);							//Pose Index
						poseIndex = atoi(tok);
						tok = strtok(0, tab);								//X Translation
						translationxyz[0] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Y Translation
						translationxyz[1] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Z Translation
						translationxyz[2] = (a3real)atof(tok);
						tok = strtok(0, tab);								//X Rotation
						rotationxyz[0] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Y Rotation
						rotationxyz[1] = (a3real)atof(tok);
						tok = strtok(0, tab);								//Z Rotation
						rotationxyz[2] = (a3real)atof(tok);
						tok = strtok(0, newline);							//Bone Length
						boneLength = (a3real)atof(tok);

						jointIndex = a3hierarchyGetNodeIndex(hierarchy_out, jointName);
						spatialPose = &poseGroup_out->hPoses[poseIndex + 1].sPoses[jointIndex];// +jointIndex;
						//DONT FORGET TO SET POSE DURATION (1/FRAMERATE)

						a3spatialPoseSetTranslation(spatialPose, +translationxyz[0], +translationxyz[1], +translationxyz[2]);

						if (!degrees) {
							rotationxyz[0] = rotationxyz[0] * radToDeg;
							rotationxyz[1] = rotationxyz[1] * radToDeg;
							rotationxyz[2] = rotationxyz[2] * radToDeg;
						}
						a3spatialPoseSetRotation(spatialPose, +rotationxyz[0], +rotationxyz[1], +rotationxyz[2]);

						if (boneLengthAxis == 'X') {
							a3spatialPoseSetScale(spatialPose, boneLength, 1, 1);
						}
						else if (boneLengthAxis == 'Y') {
							a3spatialPoseSetScale(spatialPose, 1, boneLength, 1);
						}
						else if (boneLengthAxis == 'Z') {
							a3spatialPoseSetScale(spatialPose, 1, 1, boneLength);
						}

						//a3spatialPoseConvert(&spatialPose->transform, spatialPose, poseGroup_out->channel[jointIndex], poseGroup_out->order);
					}
				}
			}
		}
	}
	fclose(fptr);
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
