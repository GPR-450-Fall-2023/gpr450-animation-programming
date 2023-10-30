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

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"

#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

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
	const a3byte* const geometryStream = "./data/gpro_base_anim.dat";


	// next set up hierarchy poses
	hierarchy = demoMode->hierarchy_skel;
	hierarchyPoseGroup = demoMode->hierarchyPoseGroup_skel;


	// stream animation assets
	if (demoState->streaming && a3fileStreamOpenRead(fileStream, geometryStream))
	{
		// load hierarchy assets
		a3hierarchyLoadBinary(demoMode->sceneGraph, fileStream);
		a3hierarchyLoadBinary(hierarchy, fileStream);
		hierarchyPoseGroup->hierarchy = hierarchy;
		a3hierarchyPoseGroupLoadBinary(hierarchyPoseGroup, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (!demoState->streaming || a3fileStreamOpenWrite(fileStream, geometryStream))
	{
		// set up scenegraph
		a3hierarchyCreate(demoMode->sceneGraph,
			(animationMaxCount_sceneObject + animationMaxCount_cameraObject + 1), 0);
		a3hierarchySetNode(demoMode->sceneGraph, 0, -1, "scene_world_root");
		a3hierarchySetNode(demoMode->sceneGraph, 1, 0, "scene_camera_main");
		a3hierarchySetNode(demoMode->sceneGraph, 2, 0, "scene_light_main");
		a3hierarchySetNode(demoMode->sceneGraph, 3, 0, "scene_skeleton_ctrl");
		a3hierarchySetNode(demoMode->sceneGraph, 4, 0, "scene_skybox");
		a3hierarchySetNode(demoMode->sceneGraph, 5, 3, "scene_skeleton");

		/*
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


			// allocate poses
			a3hierarchyPoseGroupCreate(hierarchyPoseGroup, hierarchy, 5, a3poseEulerOrder_xyz);

			// define "bind pose" or "base pose" or the initial transformation
			//	description for each joint (not a literal transform)
			p = 0;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, 0.0f, 0.0f, +3.6f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz | a3poseChannel_scale_xyz | a3poseChannel_translate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_lower");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, -90.0f, -5.0f);
			a3spatialPoseSetTranslation(spatialPose, 0.0f, -0.1f, +0.1f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_mid");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_upper");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -5.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:neck");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:head");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:clavicle");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, +90.0f, 0.0f);
			a3spatialPoseSetTranslation(spatialPose, -0.1f, +0.1f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, -0.1f, -0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_translate_yz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, +30.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +20.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, -0.5f, -0.1f, -0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_translate_yz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, -30.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -20.0f);
			a3spatialPoseSetTranslation(spatialPose, -2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:pelvis");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, 0.0f, 0.0f, -0.1f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, +90.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, +1.0f, -0.1f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -20.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +90.0f);
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_r");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +0.2f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, -90.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, -1.0f, -0.1f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +20.0f);
			a3spatialPoseSetTranslation(spatialPose, -2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -90.0f);
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_l");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, -0.2f, 0.0f, 0.0f);


			// each remaining pose represents a "delta" from the base
			// initialize the changes where applicable
			// (note: the channels describe which pose components can change)
			p = 2;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetRotation(spatialPose, +45.0f, +60.0f, +90.0f);	// rotate whole figure about all axes


			p = 3;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetScale(spatialPose, 2.0f, 2.0f, 2.0f);	// uniformly scale whole figure up to 200%


			p = 4;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].pose + j;
			a3spatialPoseSetTranslation(spatialPose, +3.0f, +4.0f, +5.0f);	// shift whole figure by some vector
		*/

		// load skeletal data from file
		hierarchyPoseGroup->hierarchy = 0;
		a3hierarchyPoseGroupLoadHTR(hierarchyPoseGroup, hierarchy,
			//"../../../../resource/animdata/egnaro/egnaro_skel_anim.htr");
			"../../../../resource/animdata/xbot/xbot_loco.htr");

		// edit assets as needed
		// mixamo assets have the wrong base pose; use first key as base and subtract from all
		p = 1;
		a3hierarchyPoseCopy(hierarchyPoseGroup->hpose, hierarchyPoseGroup->hpose + p, hierarchy->numNodes);
		for (; p < hierarchyPoseGroup->hposeCount; ++p)
			a3hierarchyPoseDeconcat(hierarchyPoseGroup->hpose + p, hierarchyPoseGroup->hpose + p,
				hierarchyPoseGroup->hpose, hierarchy->numNodes);

		// furthermore, end joints were removed, so they have no animation data; initialize it as identity
		for (j = a3hierarchyGetNodeIndex(hierarchy, "HeadTop_End"), p = 1;
			p < hierarchyPoseGroup->hposeCount; ++p)
			a3spatialPoseReset(hierarchyPoseGroup->hpose[p].pose + j);
		for (j = a3hierarchyGetNodeIndex(hierarchy, "LeftToe_End"), p = 1;
			p < hierarchyPoseGroup->hposeCount; ++p)
			a3spatialPoseReset(hierarchyPoseGroup->hpose[p].pose + j);
		for (j = a3hierarchyGetNodeIndex(hierarchy, "RightToe_End"), p = 1;
			p < hierarchyPoseGroup->hposeCount; ++p)
			a3spatialPoseReset(hierarchyPoseGroup->hpose[p].pose + j);

		// finally, append prefix names to match what is expected for skinning
		a3hierarchyPrefixNodeNames(demoMode->hierarchy_skel, "mixamorig:");

		// save hierarchy assets
		a3hierarchySaveBinary(demoMode->sceneGraph, fileStream);
		a3hierarchySaveBinary(hierarchy, fileStream);
		a3hierarchyPoseGroupSaveBinary(hierarchyPoseGroup, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}


	// egnaro
	//demoMode->obj_skeleton->euler.z = +a3real_oneeighty;
	//demoMode->obj_skeleton->euler.x = -a3real_ninety;

	// xbot
	demoMode->obj_skeleton->euler.x = a3real_ninety;
	demoMode->obj_skeleton->euler.y = a3real_oneeighty;

	// control node
	demoMode->obj_skeleton_ctrl->position.y = +a3real_four;
	demoMode->obj_skeleton_ctrl->euler.z = a3real_oneeighty;

	// map relevant objects to scene graph
	demoMode->obj_camera_main->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_camera_main");
	demoMode->obj_skeleton->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skeleton");
	demoMode->obj_skybox->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skybox");

	// scene graph state
	demoMode->sceneGraphState->hierarchy = 0;
	a3hierarchyStateCreate(demoMode->sceneGraphState, demoMode->sceneGraph);

	// finally set up hierarchy states
	// base state for skeleton
	hierarchyState = demoMode->hierarchyState_skel;
	hierarchyState->hierarchy = 0;
	a3hierarchyStateCreate(hierarchyState, hierarchy);
	a3hierarchyPoseCopy(hierarchyState->localSpace, hierarchyPoseGroup->hpose, hierarchy->numNodes);
	a3hierarchyPoseConvert(hierarchyState->localSpace, hierarchy->numNodes, hierarchyPoseGroup->channel, hierarchyPoseGroup->order);
	a3kinematicsSolveForward(hierarchyState);
	a3hierarchyPoseRestore(hierarchyState->objectSpace, hierarchy->numNodes, hierarchyPoseGroup->channel, hierarchyPoseGroup->order);
	a3hierarchyStateUpdateObjectInverse(hierarchyState);

	// real-time state
	hierarchyState = demoMode->hierarchyState_skel + 1;
	hierarchyState->hierarchy = 0;
	a3hierarchyStateCreate(hierarchyState, hierarchy);


	// clips and controllers
	{
		a3byte const* clipName[] = {
			"xbot",
			"xbot_base",
			"xbot_ymca",
			"xbot_idle_pistol",
			"xbot_skintest",
			"xbot_gangnam",
			"xbot_samba",
			"xbot_backflip",
			"xbot_idle_f",
			"xbot_jump_f",
			"xbot_walk_f",
			"xbot_run_f",
			"xbot_walk_s_l_f",
			"xbot_strafe_l_f",
			"xbot_turn_l_f",
			"xbot_walk_s_r_f",
			"xbot_strafe_r_f",
			"xbot_turn_r_f",
			"xbot_idle_m",
			"xbot_jump_m",
			"xbot_walk_m",
			"xbot_run_m",
			"xbot_walk_s_l_m",
			"xbot_strafe_l_m",
			"xbot_turn_l_m",
			"xbot_walk_s_r_m",
			"xbot_strafe_r_m",
			"xbot_turn_r_m",
		};
		a3ui32 const clipCount = sizeof(clipName) / sizeof(*clipName);
		a3ui32 const sampleIndexFirst[] = {
			1,1,25,134,167,222,519,1092,1233,1434,1475,1499,1517,1540,1557,1586,1609,1626,1655,1856,1909,1935,1953,1979,1996,2019,2045,2062,
		};
		a3ui32 const sampleIndexFinal[] = {
			2084,24,133,166,221,518,1091,1232,1433,1474,1498,1516,1539,1556,1585,1608,1625,1654,1855,1908,1934,1952,1978,1995,2018,2044,2061,2084,
		};
		a3ui32 const rate = 24;
		a3f64 const fps = (a3f64)rate;

		p = hierarchyPoseGroup->hposeCount;
		a3clipPoolCreate(demoMode->clipPool, clipCount, p - 1, p);
		for (j = 0; j < p; ++j)
			a3sampleInit(demoMode->clipPool->sample + j, j, fps);
		for (j = 0; j < p - 1; ++j)
			a3keyframeInit(demoMode->clipPool->keyframe + j,
				demoMode->clipPool->sample + j, demoMode->clipPool->sample + j + 1, fps);
		for (j = 0; j < clipCount; ++j)
		{
			a3clipInit(demoMode->clipPool->clip + j, clipName[j],
				demoMode->clipPool->keyframe + sampleIndexFirst[j],
				demoMode->clipPool->keyframe + sampleIndexFinal[j] - 1);
			a3clipCalculateDuration(demoMode->clipPool, j, fps);
		}

		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot");
		a3clipControllerInit(demoMode->clipCtrl, "xbot_ctrl", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_pistol");
		a3clipControllerInit(demoMode->clipCtrlA, "xbot_ctrlA", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_skintest");
		a3clipControllerInit(demoMode->clipCtrlB, "xbot_ctrlB", demoMode->clipPool, j, rate, fps);

		//Delaunay Clip Controllers
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_ymca");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[0], "xbot_ctrl_delaunay0_ymca", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_pistol");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[1], "xbot_ctrl_delaunay1_pistol", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_skintest");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[2], "xbot_ctrl_delaunay2_skintest", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_gangnam");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[3], "xbot_ctrl_delaunay3_gangnam", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_samba");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[4], "xbot_ctrl_delaunay4_samba", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_backflip");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[5], "xbot_ctrl_delaunay5_backflip", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[6], "xbot_ctrl_delaunay6_idle_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[7], "xbot_ctrl_delaunay7_jump_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_run_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[8], "xbot_ctrl_delaunay8_walk_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_s_l_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[9], "xbot_ctrl_delaunay9_run_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_strafe_l_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[10], "xbot_ctrl_delaunay10_walk_s_l_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_turn_l_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[11], "xbot_ctrl_delaunay11_strafe_l_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_s_r_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[12], "xbot_ctrl_delaunay12_turn_l_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_strafe_r_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[13], "xbot_ctrl_delaunay13_walk_s_r_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_turn_r_f");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[14], "xbot_ctrl_delaunay14_strafe_r_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_m");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[15], "xbot_ctrl_delaunay15_turn_r_f", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_m");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[16], "xbot_ctrl_delaunay16_idle_m", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_run_m");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[17], "xbot_ctrl_delaunay17_jump_m", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_s_l_m");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[18], "xbot_ctrl_delaunay18_walk_m", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_strafe_r_m");
		a3clipControllerInit(&demoMode->delaunayClipControllerSet[19], "xbot_ctrl_delaunay19_run_m", demoMode->clipPool, j, rate, fps);

		////Delaunay Clip Controllers
		//// 
		////Forward
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[0], "xbot_ctrl_delaunay6_idle_f", demoMode->clipPool, j, rate, fps);
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[1], "xbot_ctrl_delaunay8_walk_f", demoMode->clipPool, j, rate, fps);
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_run_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[2], "xbot_ctrl_delaunay11_strafe_l_f", demoMode->clipPool, j, rate, fps);
		//
		////Left
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_turn_l_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[3], "xbot_ctrl_delaunay9_run_f", demoMode->clipPool, j, rate, fps);
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_s_l_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[4], "xbot_ctrl_delaunay10_walk_s_l_f", demoMode->clipPool, j, rate, fps);
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_strafe_l_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[5], "xbot_ctrl_delaunay12_turn_l_f", demoMode->clipPool, j, rate, fps);
		//
		////Right
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_turn_r_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[6], "xbot_ctrl_delaunay14_turn_r_f", demoMode->clipPool, j, rate, fps);
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_s_r_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[7], "xbot_ctrl_delaunay13_walk_s_r_f", demoMode->clipPool, j, rate, fps);
		//j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_strafe_r_f");
		//a3clipControllerInit(&demoMode->delaunayClipControllerSet[8], "xbot_ctrl_delaunay14_strafe_r_f", demoMode->clipPool, j, rate, fps);
		//}
	}


	//Set delaunay as the displayed mode
	demoMode->toolMode = animation_tool_delaunay;

	//Delaunay Points and Triangulation
	{
		//Graph dimensions
		demoMode->graphStartX = (a3real)-.9;
		demoMode->graphStartY = (a3real)-.5;
		demoMode->graphViewWidth = (a3real).5;
		demoMode->graphViewHeight = (a3real).5;

		//Starting value of triangulation position controlled by mouse
		demoMode->triangulationPosition.x = demoMode->graphStartX;
		demoMode->triangulationPosition.y = demoMode->graphStartY;

		////Cleaner graph of animations that blend together more realistically
		//demoMode->delaunayPointSet[0].x = (a3real).5;
		//demoMode->delaunayPointSet[0].y = (a3real)0;
		//demoMode->delaunayPointSet[1].x = (a3real).5;
		//demoMode->delaunayPointSet[1].y = (a3real).5;
		//demoMode->delaunayPointSet[2].x = (a3real).5;
		//demoMode->delaunayPointSet[2].y = (a3real)1;
		//demoMode->delaunayPointSet[3].x = (a3real)0;
		//demoMode->delaunayPointSet[3].y = (a3real)0;
		//demoMode->delaunayPointSet[4].x = (a3real)0;
		//demoMode->delaunayPointSet[4].y = (a3real).5;
		//demoMode->delaunayPointSet[5].x = (a3real)0;
		//demoMode->delaunayPointSet[5].y = (a3real)1;
		//demoMode->delaunayPointSet[6].x = (a3real)1;
		//demoMode->delaunayPointSet[6].y = (a3real)0;
		//demoMode->delaunayPointSet[7].x = (a3real)1;
		//demoMode->delaunayPointSet[7].y = (a3real).5;
		//demoMode->delaunayPointSet[8].x = (a3real)1;
		//demoMode->delaunayPointSet[8].y = (a3real)1;

		//demoMode->delaunayPointCount = 9;

		//Random graph
		//Locations of points normalized to delaunay graph bounds
		demoMode->delaunayPointSet[0].x = (a3real)0;
		demoMode->delaunayPointSet[0].y = (a3real)0;
		demoMode->delaunayPointSet[1].x = (a3real)0;
		demoMode->delaunayPointSet[1].y = (a3real)1;
		demoMode->delaunayPointSet[2].x = (a3real)1;
		demoMode->delaunayPointSet[2].y = (a3real)0;
		demoMode->delaunayPointSet[3].x = (a3real)1;
		demoMode->delaunayPointSet[3].y = (a3real)1;
		demoMode->delaunayPointSet[4].x = (a3real).1;
		demoMode->delaunayPointSet[4].y = (a3real).5;
		demoMode->delaunayPointSet[5].x = (a3real).6;
		demoMode->delaunayPointSet[5].y = (a3real).7;
		demoMode->delaunayPointSet[6].x = (a3real).9;
		demoMode->delaunayPointSet[6].y = (a3real).2;
		demoMode->delaunayPointSet[7].x = (a3real)0;
		demoMode->delaunayPointSet[7].y = (a3real).1;
		demoMode->delaunayPointSet[8].x = (a3real).7;
		demoMode->delaunayPointSet[8].y = (a3real).2;
		demoMode->delaunayPointSet[9].x = (a3real).8;
		demoMode->delaunayPointSet[9].y = (a3real).3;
		demoMode->delaunayPointSet[10].x = (a3real).9;
		demoMode->delaunayPointSet[10].y = (a3real)0;
		demoMode->delaunayPointSet[11].x = (a3real).6;
		demoMode->delaunayPointSet[11].y = (a3real).1;
		demoMode->delaunayPointSet[12].x = (a3real).9;
		demoMode->delaunayPointSet[12].y = (a3real).7;
		demoMode->delaunayPointSet[13].x = (a3real).8;
		demoMode->delaunayPointSet[13].y = (a3real).1;
		demoMode->delaunayPointSet[14].x = (a3real).2;
		demoMode->delaunayPointSet[14].y = (a3real).8;
		demoMode->delaunayPointSet[15].x = (a3real).4;
		demoMode->delaunayPointSet[15].y = (a3real).1;
		demoMode->delaunayPointSet[16].x = (a3real)0;
		demoMode->delaunayPointSet[16].y = (a3real).5;
		demoMode->delaunayPointSet[17].x = (a3real).5;
		demoMode->delaunayPointSet[17].y = (a3real)0;
		demoMode->delaunayPointSet[18].x = (a3real).5;
		demoMode->delaunayPointSet[18].y = (a3real)1;
		demoMode->delaunayPointSet[19].x = (a3real)1;
		demoMode->delaunayPointSet[19].y = (a3real).5;

		//Number of poinsts/poses
		demoMode->delaunayPointCount = 20;

		//Run Delaunay Triangulation
		a3_calculateDelaunayTriangulation(demoMode->delaunayTriangles, &demoMode->triCount, demoMode->delaunayPointSet, &demoMode->delaunayPointCount);
	}
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
	demoMode->sceneGraphState->hierarchy = demoMode->sceneGraph;
	demoMode->hierarchyState_skel->hierarchy = demoMode->hierarchy_skel;
	demoMode->hierarchyPoseGroup_skel->hierarchy = demoMode->hierarchy_skel;
	demoMode->clipCtrl->clipPool = demoMode->clipPool;
	demoMode->clipCtrl->clip = demoMode->clipPool->clip + demoMode->clipCtrl->clipIndex;
	demoMode->clipCtrl->keyframe = demoMode->clipPool->keyframe + demoMode->clipCtrl->keyframeIndex;
}


void a3animation_init_blend_tree(a3_DemoMode1_Animation* demoMode)
{
	{ // Simple clip blend tree

		a3_BlendNode* combineLerpNode = a3_CreateBlendNode(a3_BlendOpLerp);
		a3_BlendNode* clipController0LerpNode = a3_CreateBlendNode(a3_BlendOpLerp);
		a3_BlendNode* clipController1LerpNode = a3_CreateBlendNode(a3_BlendOpLerp);

		combineLerpNode->dataNodes[0] = clipController0LerpNode;
		combineLerpNode->dataNodes[1] = clipController1LerpNode;
		
		demoMode->blendTreeLerpParam = .5;
		combineLerpNode->param[0] = &(demoMode->blendTreeLerpParam);

		demoMode->blendTree.root = combineLerpNode; // Just pass in root
	}

	{ // Test blend tree, just lerps between two poses
		/*a3_BlendNode* combineLerpNode = a3_CreateBlendNode(a3_BlendOpLerp);
		demoMode->blendTree.root = combineLerpNode;*/
	}
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
	a3animation_init_blend_tree(demoMode);
}


//-----------------------------------------------------------------------------
