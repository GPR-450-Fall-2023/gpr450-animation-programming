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

	a3_DemoMode1_Animation-load.c
	Demo mode implementations: animation scene.

	********************************************
	*** LOADING FOR ANIMATION SCENE MODE     ***
	********************************************
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_DemoMode1_Animation-idle-update.c
	Initialize hierarchy data, including hierarchy states, poses, etc.
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"

#include "../a3_DemoState.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------

const a3byte filePath[1024] = "../../../../resource/animdata/egnaro/egnaro_anim.txt";
a3ui32 a3countClips(const a3byte filePath[1024]);
a3ui32 a3readClipPoolFromFile(a3_ClipPool* clipPool, a3_KeyframePool* keyframePool, const a3byte filePath[1024]);

// utility to load animation
void a3animation_init_animation(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode)
{
	// general counters
	a3ui32 j, p;

	// object pointers
	a3_Hierarchy* hierarchy = 0;
	a3_HierarchyState* hierarchyState = 0;
	a3_HierarchyPoseGroup* hierarchyPoseGroup = 0;
	a3_SpatialPose* spatialPose = 0;

	a3_FileStream fileStream[1] = { 0 };
	const a3byte* const geometryStream = "./data/anim_data_gpro_base.dat";

	// stream animation assets
	if (demoState->streaming && a3fileStreamOpenRead(fileStream, geometryStream))
	{
		// load hierarchy assets
		hierarchy = demoMode->hierarchy_skel;
		a3hierarchyLoadBinary(hierarchy, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (!demoState->streaming || a3fileStreamOpenWrite(fileStream, geometryStream))
	{
		// manually set up a skeleton
		// first is the hierarchy: the general non-spatial relationship between bones
		const a3ui32 jointCount = 32;

		// indices of joints, their parents and branching joints
		a3ui32 jointIndex = 0;
		a3i32 jointParentIndex = -1;
		a3i32 rootJointIndex, upperSpineJointIndex, clavicleJointIndex, pelvisJointIndex;

		// initialize hierarchy
		hierarchy = demoMode->hierarchy_skel;
		a3hierarchyCreate(hierarchy, jointCount, 0);

		// set up joint relationships
		jointParentIndex = rootJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:root");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_lower");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_mid");
		jointParentIndex = upperSpineJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_upper");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:neck");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:head");
		jointParentIndex = upperSpineJointIndex;
		jointParentIndex = clavicleJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:clavicle");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulderblade_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulder_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:elbow_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:forearm_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:wrist_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hand_r");
		jointParentIndex = clavicleJointIndex;
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulderblade_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulder_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:elbow_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:forearm_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:wrist_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hand_l");
		jointParentIndex = rootJointIndex;
		jointParentIndex = pelvisJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:pelvis");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hip_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:knee_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shin_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:ankle_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:foot_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:toe_r");
		jointParentIndex = pelvisJointIndex;
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hip_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:knee_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shin_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:ankle_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:foot_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:toe_l");

		// save hierarchy assets
		a3hierarchySaveBinary(hierarchy, fileStream);
	
		// done
		a3fileStreamClose(fileStream);
	}


	// scene objects
	demoMode->obj_skeleton->position.y = +a3real_four;
	demoMode->obj_skeleton->euler.z = +a3real_oneeighty;
	//demoMode->obj_skeleton->euler.x = -a3real_ninety; //Commented out because this causes the rig to be lying on its back

	// next set up hierarchy poses
	hierarchy = demoMode->hierarchy_skel;
	hierarchyPoseGroup = demoMode->hierarchyPoseGroup_skel;
	hierarchyPoseGroup->hierarchy = 0;


	// allocate poses
	a3hierarchyPoseGroupCreate(hierarchyPoseGroup, hierarchy, 82, a3poseEulerOrder_xyz); //base pose + 81 keyframes (hardcode for now)

	// define "bind pose" or "base pose" or the initial transformation 
	//	description for each joint (not a literal transform)
	p = 0;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, 0.0f, 0.0f, +3.6f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz | a3poseChannel_scale_xyz | a3poseChannel_translate_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_lower");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, -90.0f, -5.0f);
	a3spatialPoseSetTranslation(spatialPose, 0.0f, -0.1f, +0.1f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_mid");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
	a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_upper");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -5.0f);
	a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:neck");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:head");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:clavicle");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, +90.0f, 0.0f);
	a3spatialPoseSetTranslation(spatialPose, -0.1f, +0.1f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +0.5f, -0.1f, -0.5f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_translate_yz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, +30.0f, -10.0f);
	a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, +0.5f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +20.0f);
	a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_z;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_x;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -10.0f);
	a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, -0.5f, -0.1f, -0.5f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_translate_yz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, -30.0f, +10.0f);
	a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, +0.5f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -20.0f);
	a3spatialPoseSetTranslation(spatialPose, -2.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_z;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_x;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
	a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:pelvis");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, 0.0f, 0.0f, -0.1f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, +90.0f, +10.0f);
	a3spatialPoseSetTranslation(spatialPose, +1.0f, -0.1f, +0.5f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -20.0f);
	a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_z;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_x;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +90.0f);
	a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
	a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_z;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_r");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +0.2f, 0.0f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, -90.0f, -10.0f);
	a3spatialPoseSetTranslation(spatialPose, -1.0f, -0.1f, +0.5f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +20.0f);
	a3spatialPoseSetTranslation(spatialPose, -2.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_z;
	
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_x;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -90.0f);
	a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_xyz;

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -10.0f);
	a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, 0.0f);
	hierarchyPoseGroup->channel[j] = a3poseChannel_orient_z;
	
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_l");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, -0.2f, 0.0f, 0.0f);


	// each remaining pose represents a "delta" from the base
	// initialize the changes where applicable
	// (note: the channels describe which pose components can change)
	p = 2;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetRotation(spatialPose, +45.0f, +60.0f, +90.0f);	// rotate whole figure about all axes


	p = 3;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetScale(spatialPose, 2.0f, 2.0f, 2.0f);	// uniformly scale whole figure up to 200%


	p = 4;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
	spatialPose = hierarchyPoseGroup->hPoses[p].sPoses + j;
	a3spatialPoseSetTranslation(spatialPose, +3.0f, +4.0f, +5.0f);	// shift whole figure by some vector


	// load from file
	a3hierarchyPoseGroupLoadHTR(demoMode->hierarchyPoseGroup_skel, demoMode->hierarchy_skel,
		"../../../../resource/animdata/egnaro/egnaro_skel_anim.htr");
	
	// finally set up hierarchy states

	// base state for skeleton
	hierarchyState = demoMode->hierarchyState_skel;
	hierarchyState->hierarchy = 0;
	a3hierarchyStateCreate(hierarchyState, hierarchy);
	a3hierarchyPoseCopy(hierarchyState->localSpace, hierarchyPoseGroup->hPoses, hierarchy->numNodes);
	a3hierarchyPoseConvert(hierarchyState->localSpace, hierarchy->numNodes, hierarchyPoseGroup->channel, hierarchyPoseGroup->order);
	a3kinematicsSolveForward(hierarchyState);
	a3hierarchyStateUpdateObjectInverse(hierarchyState);

	// real-time state

	for (a3ui32 i = 1; i < 4; i++)
	{
		hierarchyState = demoMode->hierarchyState_skel + i;
		hierarchyState->hierarchy = 0;
		a3hierarchyStateCreate(hierarchyState, hierarchy);
	}

	demoMode->hierarchyStateIndex = state_one;
	demoMode->hierarchyPoseIndex = pose_one;

	//////////////////// Animation Data /////////////////////////
	//////// Many values here are hardcoded for now /////////////
	
	// Creating Pool of Keyframes
	demoMode->numOfKeyframes = 81; //Just keyframes, don't make a keyframe for base pose (i.e. 81 keyframes, 82 h poses)
	a3keyframePoolCreate(&demoMode->keyPool, demoMode->numOfKeyframes, 0);

	// Creating Pool of Clips
	demoMode->numOfClips = a3countClips(filePath);
	demoMode->currentClip = 0;
	a3clipPoolCreate(&demoMode->clipPool, &demoMode->keyPool, demoMode->numOfClips);

	// Initializing Keyframes
	for (a3ui32 i = 0; i < demoMode->numOfKeyframes; i++) {
		a3keyframeInit(&demoMode->keyPool.keyframe[i], (a3real)(1.0 / 2.0), (hierarchyPoseGroup->hPoses + i + 1));
		
		/////////// TEST DATA /////////////
		if (i == 0) continue;//Dont mess with base pose
		j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
		a3real t = (a3real)(10.0 / demoMode->numOfKeyframes) * i; //Evenly distrubute a translation from 0, 0, 0 to 10, 10, 10 across keyframes
		a3real r = (a3real)(360 / demoMode->numOfKeyframes) * i; //Evenly distrubute a rotation from 0 to 360 across keyframes
		a3real s = (a3real)(((3.0 / demoMode->numOfKeyframes) * i) + 1); //Evenly distrubute a scale from 1 to 4 across keyframes
		a3spatialPoseSetTranslation(&demoMode->keyPool.keyframe[i].data->sPoses[j], t, t, t);
		a3spatialPoseSetRotation(&demoMode->keyPool.keyframe[i].data->sPoses[j], r, r, r);
		a3spatialPoseSetScale(&demoMode->keyPool.keyframe[i].data->sPoses[j], s, s, s);
	}

	a3readClipPoolFromFile(&demoMode->clipPool, &demoMode->keyPool, filePath);

	//////////

	// Initializing Clips
	/*a3clipInit(&demoMode->clipPool.clip[0], "Clip " + (1), &demoMode->clipPool, &demoMode->keyPool, 0, 5);
	a3clipInit(&demoMode->clipPool.clip[1], "Clip " + (2), &demoMode->clipPool, &demoMode->keyPool, 8, 14);
	a3clipInit(&demoMode->clipPool.clip[2], "Clip " + (3), &demoMode->clipPool, &demoMode->keyPool, 8, 14);
	a3clipInit(&demoMode->clipPool.clip[3], "Clip " + (4), &demoMode->clipPool, &demoMode->keyPool, 12, 18);
	a3clipInit(&demoMode->clipPool.clip[4], "Clip " + (5), &demoMode->clipPool, &demoMode->keyPool, 16, 20);*/
	// I didnt put this in a for loop because it was getting annoying
	// to think of a mathematical function to make the clips share keyframes
	// but also not go out of bounds, so i just did it manually
	// we can change this later

	//Test assingmnet of clip 0 transition indices pointing to the next clip in the pool
	/*demoMode->clipPool.clip[0].forwardTransition.index = 1;
	demoMode->clipPool.clip[0].forwardTransition.getNextKeyframe = a3getNextKeyframeSkipFromNextClip;
	demoMode->clipPool.clip[0].forwardTransition.transitionFunction = a3terminusForwardSkipPlayback;
	demoMode->clipPool.clip[0].backwardTransition.index = 1;
	demoMode->clipPool.clip[0].backwardTransition.getNextKeyframe = a3getNextKeyframeSkipFromNextClip;
	demoMode->clipPool.clip[0].backwardTransition.transitionFunction = a3terminusReverseSkipPlayback;*/

	// Initializing Clip Controllers
	a3clipControllerPoolCreate(&demoMode->clipCtrlPool, &demoMode->clipPool, 1);
	demoMode->currentClip = 0;
	a3clipControllerInit(&demoMode->clipCtrlPool.clipControllers[0], "Clip Controller 1", &demoMode->clipPool, demoMode->currentClip);
	demoMode->currentController = 0;
	demoMode->numOfControllers = 1;

	//Reset settings
	/*demoMode->shouldSpeedUp = false;
	demoMode->shouldSlowDown = false;
	demoMode->togglePause = false;
	demoMode->shouldRewind = false;*/

	////// Test Code
	//demoMode->clipCtrl1.playbackDirection = 1;
	//demoMode->clipCtrl1.terminusAction = PING_PONG;

	//Used to move playhead to the end of the clip so we can test backward movement (like the stop terminus)
	/*a3_Clip clip = demoMode->clipCtrlPool.clipControllers[0].clipPool->clip[demoMode->clipCtrlPool.clipControllers[0].clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clip.keyframeCount - 1];
	demoMode->clipCtrlPool.clipControllers[0].clipTime = clip.duration;
	demoMode->clipCtrlPool.clipControllers[0].clipParameter = 1;
	demoMode->clipCtrlPool.clipControllers[0].keyframe = clip.keyframeCount - 1;
	demoMode->clipCtrlPool.clipControllers[0].keyframeTime = keyframe.duration;
	demoMode->clipCtrlPool.clipControllers[0].keyframeParameter = 1;
	demoMode->clipCtrlPool.clipControllers[0].playbackDirection = -1;
	demoMode->clipCtrlPool.clipControllers[0].lastPlaybackDirection = -1;*/
	//////
}


a3ui32 a3countClips(const a3byte filePath[1024]) {
	//null check if file exists
	FILE* fptr = fopen(filePath, "r");
	if (fptr == NULL) {
		printf("no such file.\n");
		return 0;
	}

	// File Format:
	// @ clip_name duration_s first_frame last_frame reverse_transition forward_transition comments(ignored)

	//check if line has @ instead of # at front
	//counts amount of lines with @ and that's the amount of clips
	a3ui32 lineCount = 0;
	a3byte lineStarter[100];
	while (fgets(lineStarter, 100, fptr)) {
		if (lineStarter[0] == '#') {
			//skip line
		}
		if (lineStarter[0] == '@') {
			lineCount++;
			printf(lineStarter);
		}
	};

	fclose(fptr);
	return lineCount;
}

a3ui32 a3readClipPoolFromFile(a3_ClipPool* clipPool, a3_KeyframePool* keyframePool, const a3byte filePath[1024]) {
	//null check if clip pool exists
	if (clipPool == NULL) {
		printf("no such clip pool.\n");
		return 0;
	}

	//null check if keyframe pool exists
	if (keyframePool == NULL) {
		printf("no such keyframe pool.\n");
		return 0;
	}

	//null check if file exists
	FILE* fptr = fopen(filePath, "r");
	if (fptr == NULL) {
		printf("no such file.\n");
		return 0;
	}

	// File Format:
	// @ clip_name duration_s first_frame last_frame reverse_transition forward_transition comments(ignored)

	a3ui32 lineCount = 0;
	a3byte line[200];
	const a3byte s[4] = "	";
	a3byte* tok;
	a3byte fileData[25][6][32];
	while (fgets(line, 200, fptr)) {
		if (line[0] == '#') {

		}
		else if (line[0] == '@') {
			tok = strtok(line, s);
			tok = strtok(0, s);
			strcpy(fileData[lineCount][0], tok);
			tok = strtok(0, s);
			strcpy(fileData[lineCount][1], tok);
			tok = strtok(0, s);
			strcpy(fileData[lineCount][2], tok);
			tok = strtok(0, s);
			strcpy(fileData[lineCount][3], tok);
			tok = strtok(0, s);
			strcpy(fileData[lineCount][4], tok);
			tok = strtok(0, s);
			strcpy(fileData[lineCount][5], tok);
			fileData[lineCount][5][strcspn(fileData[lineCount][5], "\n")] = '\0'; //trim '\n'
			lineCount++;
		}
	};

	for (a3ui32 i = 0; i < lineCount; i++) {
		a3clipInit(&clipPool->clip[i], fileData[i][0], clipPool, keyframePool, atoi(fileData[i][2]), atoi(fileData[i][3]));

		//setting clip duration to fileData[i][1]
		clipPool->clip[lineCount].duration = (a3real)atof(fileData[i][1]);
	}

	for (a3ui32 i = 0; i < lineCount; i++) {
		a3byte prevOp[5] = "";
		a3byte nextOp[5] = "";
		a3byte prevClip[32] = "";
		a3byte nextClip[32] = "";
		a3i32 prevIndex = -1;
		a3i32 nextIndex = -1;
		a3boolean specRev, specFor;

		const a3byte s[4] = " ";
		a3byte* tok;

		//checking if there is reference to another clip in the transition
		if (strlen(fileData[i][4]) > 0) {
			specRev = true;
			tok = strtok(fileData[i][4], s);
			strcpy(prevOp, tok);
			tok = strtok(0, s);
			if (tok != NULL)
			{
				strcpy(prevClip, tok);
			}
		}
		else {
			specRev = false;
		}
		if (strlen(fileData[i][5]) > 0) {
			specFor = true;
			tok = strtok(fileData[i][5], s);
			strcpy(nextOp, tok);
			tok = strtok(0, s);
			if (tok != NULL)
			{
				strcpy(nextClip, tok);
			}
		}
		else {
			specFor = false;
		}

		//find which index the previous clip transition has
		if (specRev) {
			prevIndex = a3clipGetIndexInPool(clipPool, prevClip);
		}
		else {
			prevIndex = i;
		}
		if (prevIndex == -1) {
			printf("There is no clip referenced by the reverse transition\n");
			//return 0;
		}

		//find which index the next clip transition has
		if (specFor) {
			nextIndex = a3clipGetIndexInPool(clipPool, nextClip);
		}
		else {
			nextIndex = i;
		}
		if (nextIndex == -1) {
			printf("There is no clip referenced by the forward transition\n");
			//return 0;
		}

		//These are the default values used if prevIndex and nextIndex are -1
		//backIndex is used for transitions going backwards and vice versa for forwardIndex
		//a3ui32 backIndex = (i - 1 + lineCount) % lineCount; //Ensure we start positive but don't mess up mod
		//a3ui32 forwardIndex = (i + 1) % lineCount; // Gets next index (loops around to beginning)

		////Default to current clip
		if (prevIndex < 0)
		{
			prevIndex = i;
		}

		////Default to current clip
		if (nextIndex < 0)
		{
			nextIndex = i;
		}

		//setting backward/previous transition
		if (prevOp[0] == '|' && prevOp[1] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusPause, a3getNextKeyframeLoop);
		}
		else if (prevOp[0] == '>' && prevOp[1] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusForwardPlayback, a3getNextKeyframeFromNextClip);
		}
		else if (prevOp[0] == '>' && prevOp[1] == '|' && prevOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusForwardPause, a3getNextKeyframeFromNextClip);
		}
		else if (prevOp[0] == '<' && prevOp[1] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusReversePlayback, a3getNextKeyframeFromNextClip);
		}
		else if (prevOp[0] == '<' && prevOp[1] == '|' && prevOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusReversePause, a3getNextKeyframeFromNextClip);
		}
		else if (prevOp[0] == '>' && prevOp[1] == '>' && prevOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusForwardSkipPlayback, a3getNextKeyframeSkipFromNextClip);
		}
		else if (prevOp[0] == '>' && prevOp[1] == '>' && prevOp[2] == '|' && prevOp[3] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusForwardSkipPause, a3getNextKeyframeSkipFromNextClip);
		}
		else if (prevOp[0] == '<' && prevOp[1] == '<' && prevOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusReverseSkipPlayback, a3getNextKeyframeSkipFromNextClip);
		}
		else if (prevOp[0] == '<' && prevOp[1] == '<' && prevOp[2] == '|' && prevOp[3] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, a3terminusReverseSkipPause, a3getNextKeyframeSkipFromNextClip);
		}

		//setting forward transition
		if (nextOp[0] == '|' && nextOp[1] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusPause, a3getNextKeyframeLoop);
		}
		else if (nextOp[0] == '>' && nextOp[1] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusForwardPlayback, a3getNextKeyframeFromNextClip);
		}
		else if (nextOp[0] == '>' && nextOp[1] == '|' && nextOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusForwardPause, a3getNextKeyframeFromNextClip);
		}
		else if (nextOp[0] == '<' && nextOp[1] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusReversePlayback, a3getNextKeyframeFromNextClip);
		}
		else if (nextOp[0] == '<' && nextOp[1] == '|' && nextOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusReversePause, a3getNextKeyframeFromNextClip);
		}
		else if (nextOp[0] == '>' && nextOp[1] == '>' && nextOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusForwardSkipPlayback, a3getNextKeyframeSkipFromNextClip);
		}
		else if (nextOp[0] == '>' && nextOp[1] == '>' && nextOp[2] == '|' && nextOp[3] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusForwardSkipPause, a3getNextKeyframeSkipFromNextClip);
		}
		else if (nextOp[0] == '<' && nextOp[1] == '<' && nextOp[2] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusReverseSkipPlayback, a3getNextKeyframeSkipFromNextClip);
		}
		else if (nextOp[0] == '<' && nextOp[1] == '<' && nextOp[2] == '|' && nextOp[3] == '\0') {
			a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, a3terminusReverseSkipPause, a3getNextKeyframeSkipFromNextClip);
		}
	}

	fclose(fptr);
	return 1;
}


//-----------------------------------------------------------------------------

void a3animation_input(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt);
void a3animation_update(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt);
void a3animation_render(a3_DemoState const* demoState, a3_DemoMode1_Animation const* demoMode, a3f64 const dt);
void a3animation_input_keyCharPress(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode, a3i32 const asciiKey, a3i32 const state);
void a3animation_input_keyCharHold(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode, a3i32 const asciiKey, a3i32 const state);

void a3animation_loadValidate(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode)
{
	// initialize callbacks
	a3_DemoModeCallbacks* const callbacks = demoState->demoModeCallbacks + demoState_modeAnimation;
	callbacks->demoMode = demoMode;
	callbacks->handleInput =	(a3_DemoMode_EventCallback)		a3animation_input;
	callbacks->handleUpdate =	(a3_DemoMode_EventCallback)		a3animation_update;
	callbacks->handleRender =	(a3_DemoMode_EventCallbackConst)a3animation_render;
	callbacks->handleKeyPress = (a3_DemoMode_InputCallback)		a3animation_input_keyCharPress;
	callbacks->handleKeyHold =	(a3_DemoMode_InputCallback)		a3animation_input_keyCharHold;

	// initialize cameras dependent on viewport
	demoMode->proj_camera_main->aspect = demoState->frameAspect;
	a3demo_updateProjectorProjectionMat(demoMode->proj_camera_main);
	a3demo_setProjectorSceneObject(demoMode->proj_camera_main, demoMode->obj_camera_main);

	// initialize cameras not dependent on viewport

	// animation
	demoMode->hierarchyState_skel->hierarchy = demoMode->hierarchy_skel;
	demoMode->hierarchyPoseGroup_skel->hierarchy = demoMode->hierarchy_skel;
}


void a3animation_load(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode)
{
	a3ui32 i;

	a3_DemoSceneObject* currentSceneObject;
	a3_DemoProjector* projector;


	// camera's starting orientation depends on "vertical" axis
	// we want the exact same view in either case
	const a3real sceneCameraAxisPos = 20.0f;
	const a3vec3 sceneCameraStartPos = {
		+sceneCameraAxisPos,
		-sceneCameraAxisPos,
		+sceneCameraAxisPos + 5.0f,
	};
	const a3vec3 sceneCameraStartEuler = {
		+55.0f,
		+0.0f,
		+45.0f,
	};
	const a3f32 sceneObjectDistance = 8.0f;
	const a3f32 sceneObjectHeight = 2.0f;


	// all objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
		a3demo_initSceneObject(demoMode->object_scene + i);
	for (i = 0; i < animationMaxCount_cameraObject; ++i)
		a3demo_initSceneObject(demoMode->object_camera + i);
	for (i = 0; i < animationMaxCount_projector; ++i)
		a3demo_initProjector(demoMode->projector + i);

	currentSceneObject = demoMode->obj_skybox;
	currentSceneObject->scaleMode = 1;
	currentSceneObject->scale.x = 256.0f;

	currentSceneObject = demoMode->obj_skeleton;
	currentSceneObject->scaleMode = 0;


	// set up cameras
	projector = demoMode->proj_camera_main;
	projector->perspective = a3true;
	projector->fovy = a3real_fortyfive;
	projector->znear = 1.0f;
	projector->zfar = 1024.0f;
	projector->ctrlMoveSpeed = 10.0f;
	projector->ctrlRotateSpeed = 5.0f;
	projector->ctrlZoomSpeed = 5.0f;
	projector->sceneObject->position = sceneCameraStartPos;
	projector->sceneObject->euler = sceneCameraStartEuler;


	// set flags
	demoMode->render = animation_renderPhong;
	demoMode->display = animation_displayTexture;
	demoMode->activeCamera = animation_cameraSceneViewer;

	demoMode->pipeline = animation_forward;
	demoMode->pass = animation_passComposite;

	demoMode->targetIndex[animation_passScene] = animation_scene_finalcolor;
	demoMode->targetIndex[animation_passComposite] = animation_scene_finalcolor;

	demoMode->targetCount[animation_passScene] = animation_target_scene_max;
	demoMode->targetCount[animation_passComposite] = 1;


	// setup
	a3animation_init_animation(demoState, demoMode);
}


//-----------------------------------------------------------------------------
