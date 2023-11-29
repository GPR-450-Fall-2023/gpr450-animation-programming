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

a3ui32 a3_jsonKeyToEnumKey(a3_DemoMode1_Animation* demoMode, a3byte const jsonKey[a3_blend_node_name_length], void* data)
{
	if (jsonKey == "node_num")
	{
		demoMode->blendNodeCount = (a3ui32)data;
	}
	else if (jsonKey == "root")
	{

	}
}


void a3animation_initBlendTree(a3_DemoMode1_Animation* demoMode)
{
	// Character controller blend tree
	
	// Variable initialization
	demoMode->idleBlendThreshold = 0;
	demoMode->walkBlendThreshold = 2;
	demoMode->runBlendThreshold = 4;
	demoMode->isJumping = false;
	demoMode->timeSinceJump = 0;
	demoMode->jumpFadeInTime = .8f;
	demoMode->jumpFadeOutTime = .6f;
	demoMode->jumpLerpParam = 0;
	demoMode->jumpHeight = 5;
	demoMode->jumpDuration = (a3real) demoMode->clipPool[0].clip[demoMode->jumpClipCtrl->clipIndex].duration_sec;

	// Node for getting spatial data from jump clip controller
	a3_BlendNode* jumpCCNode = a3_CreateBlendNode(a3_BlendOp_EvaluateClipController);
	jumpCCNode->info.miscData[0] = demoMode->jumpClipCtrl;
	jumpCCNode->info.miscData[1] = demoMode->hierarchyPoseGroup_skel;

	// Node for getting spatial data from idle clip controller
	a3_BlendNode* idleCCNode = a3_CreateBlendNode(a3_BlendOp_EvaluateClipController);
	idleCCNode->info.miscData[0] = demoMode->idleClipCtrl;
	idleCCNode->info.miscData[1] = demoMode->hierarchyPoseGroup_skel;


	// Node for getting spatial data from walk clip controller
	a3_BlendNode* walkCCNode = a3_CreateBlendNode(a3_BlendOp_EvaluateClipController);
	walkCCNode->info.miscData[0] = demoMode->walkClipCtrl;
	walkCCNode->info.miscData[1] = demoMode->hierarchyPoseGroup_skel;


	// Node for getting spatial data from run clip controller
	a3_BlendNode* runCCNode = a3_CreateBlendNode(a3_BlendOp_EvaluateClipController);
	runCCNode->info.miscData[0] = demoMode->runClipCtrl;
	runCCNode->info.miscData[1] = demoMode->hierarchyPoseGroup_skel;

		
	// Node used for blending between idle, walk, and run depending on velocity
	a3_BlendNode* blendGroundPoseNode = a3_CreateBlendNode(a3_BlendOp_Blend_3);
	blendGroundPoseNode->info.paramData[0] = &(demoMode->ctrlVelocityMagnitude);
	blendGroundPoseNode->info.paramData[1] = &(demoMode->idleBlendThreshold);
	blendGroundPoseNode->info.paramData[2] = &(demoMode->walkBlendThreshold);
	blendGroundPoseNode->info.paramData[3] = &(demoMode->runBlendThreshold);
	blendGroundPoseNode->info.spatialDataNodes[0] = idleCCNode;
	blendGroundPoseNode->info.spatialDataNodes[1] = walkCCNode;
	blendGroundPoseNode->info.spatialDataNodes[2] = runCCNode;

	// Lerps between ground and jump anims, allows for gradual transition
	a3_BlendNode* jumpGroundLerpNode = a3_CreateBlendNode(a3_BlendOp_Lerp);
	jumpGroundLerpNode->info.spatialDataNodes[0] = blendGroundPoseNode;
	jumpGroundLerpNode->info.spatialDataNodes[1] = jumpCCNode;
	jumpGroundLerpNode->info.paramData[0] = &(demoMode->jumpLerpParam);

	// Node for updating jump variables and applying them for jump
	a3_BlendNode* handleJumpNode = a3_CreateBlendNode(a3_BlendOp_HandleJump);
	handleJumpNode->info.paramData[0] = &(demoMode->jumpDuration);
	handleJumpNode->info.paramData[1] = &(demoMode->jumpHeight);
	handleJumpNode->info.paramData[2] = &(demoMode->jumpFadeInTime);
	handleJumpNode->info.paramData[3] = &(demoMode->jumpFadeOutTime);
	handleJumpNode->info.miscData[0] = &(demoMode->timeSinceJump);
	handleJumpNode->info.miscData[1] = &(demoMode->jumpLerpParam);
	handleJumpNode->info.miscData[2] = &(demoMode->isJumping);
	handleJumpNode->info.miscData[3] = demoMode->ctrlNode;
	handleJumpNode->info.spatialDataNodes[0] = jumpGroundLerpNode;

	// Node for branching depending on whether character should be jumping or not
	a3_BlendNode* jumpBranchNode = a3_CreateBlendNode(a3_BlendOp_BoolBranch);
	jumpBranchNode->info.miscData[0] = &(demoMode->isJumping);
	jumpBranchNode->info.spatialDataNodes[0] = handleJumpNode; // True, we are jumping
	jumpBranchNode->info.spatialDataNodes[1] = blendGroundPoseNode; // False, we are on ground

	demoMode->blendTree.root = jumpBranchNode;

	/////////////////// Temp Code //////////////////

	//Set using "node_num" key in file, set directly when loading data
	demoMode->blendNodeCount = 8;

	//Read root node block using "root" key, stores ID of root node

	//Create an array of string identifiers for each data type (blend operations, blend nodes, etc.)
	//Create a parallel enum with values corresponding to those indices names corresponding to the strings in those indices
	
	// LOOKUP FUNCTIONS
	//Need a function that takes in a json key/string identifier and returns a blend operation
	//Need another function that takes in a json key/string identifier and returns a blend node
	//Need another function that takes in a json key/string identifier and returns the address / a pointer to a variable in demoMode
	//Need another function that takes in a node id and json key/string identifier (miscData0, paramData3) and returns 
		//the address/pointer to the location in the blend tree info struct of that node

	//When data is read in, it just creates the nodes from the file in memory and sets the IDs of things like:
		//node id
		//spatialDataNodes
		//misc data
		//param data
		//blend operation

	//At the end when all IDs are set, use the lookup functions to do a pass through each node to start creating references foe:
		//spatialDataNodes
		//the root node
		//misc data variables
		//param data variables
		//blend operation pointers
	

	////////////////////////////////////////////////
}


void a3animation_load_resetEffectors(a3_DemoMode1_Animation* demoMode,
	a3_HierarchyState* hierarchyState, a3_HierarchyPoseGroup const* poseGroup)
{
	a3_DemoSceneObject* sceneObject = demoMode->obj_skeleton;
	a3ui32 j = sceneObject->sceneGraphIndex;

	// need to properly transform joints to their parent frame
	a3mat4 const skeletonToControl = demoMode->sceneGraphState->localSpace->pose[j].transformMat;
	a3vec4 controlLocator;

	// look-at effector
	// position in front of neck
	j = a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "mixamorig:Neck");
	sceneObject = demoMode->obj_skeleton_neckLookat_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->pose[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x;
	sceneObject->position.y = controlLocator.y + a3real_four;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// right wrist effector
	// position on wrist
	j = a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "mixamorig:RightHand");
	sceneObject = demoMode->obj_skeleton_wristEffector_r_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->pose[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x;
	sceneObject->position.y = controlLocator.y;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// right wrist constraint
	// position behind elbow
	j = a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "mixamorig:RightForeArm");
	sceneObject = demoMode->obj_skeleton_wristConstraint_r_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->pose[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x;
	sceneObject->position.y = controlLocator.y - a3real_half;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// right wrist base
	//j = a3hierarchyGetNodeIndex(demoMode->hierarchy_skel, "mixamorig:RightArm");
}


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
	const a3byte* const geometryStream = "./data/gpro_base_anim_working.dat";


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
		a3hierarchyCreate(demoMode->sceneGraph, animationMaxCount_sceneObject, 0);
		a3hierarchySetNode(demoMode->sceneGraph, 0, -1, "scene_world_root");
		a3hierarchySetNode(demoMode->sceneGraph, 1, 0, "scene_camera_main");
		a3hierarchySetNode(demoMode->sceneGraph, 2, 0, "scene_light_main");
		a3hierarchySetNode(demoMode->sceneGraph, 3, 0, "scene_skybox");
		a3hierarchySetNode(demoMode->sceneGraph, 4, 0, "scene_skeleton_ctrl");
		a3hierarchySetNode(demoMode->sceneGraph, 5, 4, "scene_skeleton_neckLookat_ctrl");
		a3hierarchySetNode(demoMode->sceneGraph, 6, 4, "scene_skeleton_wristEff_r_ctrl");
		a3hierarchySetNode(demoMode->sceneGraph, 7, 4, "scene_skeleton_wristCon_r_ctrl");
		a3hierarchySetNode(demoMode->sceneGraph, 8, 4, "scene_skeleton");

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

	// map relevant objects to scene graph
	demoMode->obj_world_root->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_world_root");
	demoMode->obj_camera_main->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_camera_main");
	demoMode->obj_light_main->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_light_main");
	demoMode->obj_skybox->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skybox");
	demoMode->obj_skeleton_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skeleton_ctrl");
	demoMode->obj_skeleton_neckLookat_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skeleton_neckLookat_ctrl");
	demoMode->obj_skeleton_wristEffector_r_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skeleton_wristEff_r_ctrl");
	demoMode->obj_skeleton_wristConstraint_r_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skeleton_wristCon_r_ctrl");
	demoMode->obj_skeleton->sceneGraphIndex = a3hierarchyGetNodeIndex(demoMode->sceneGraph, "scene_skeleton");

	// scene graph state
	demoMode->sceneGraphState->hierarchy = 0;
	a3hierarchyStateCreate(demoMode->sceneGraphState, demoMode->sceneGraph);


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
		/*j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_pistol");
		a3clipControllerInit(demoMode->clipCtrlA, "xbot_ctrlA", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_skintest");
		a3clipControllerInit(demoMode->clipCtrlB, "xbot_ctrlB", demoMode->clipPool, j, rate, fps);*/

		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_f");//"xbot_idle_pistol");
		a3clipControllerInit(demoMode->clipCtrlA, "xbot_ctrlA", demoMode->clipPool, j, rate, fps);
		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_skintest");
		a3clipControllerInit(demoMode->clipCtrlB, "xbot_ctrlB", demoMode->clipPool, j, rate, fps);

		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_f");//"xbot_idle_pistol");
		a3clipControllerInit(demoMode->idleClipCtrl, "xbot_idleCtrl", demoMode->clipPool, j, rate, fps);

		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_jump_f");//"xbot_idle_pistol");
		a3clipControllerInit(demoMode->jumpClipCtrl, "xbot_jumpCtrl", demoMode->clipPool, j, rate, fps);




		const a3real CYCLE_DURATION = 1.2f;



		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_f");//"xbot_idle_pistol");
		float distributeTime = demoMode->clipPool[0].clip[j].keyframeCount / CYCLE_DURATION;
		a3clipDistributeDuration(&(demoMode->clipPool[0]), j, distributeTime);
		a3clipControllerInit(demoMode->walkClipCtrl, "xbot_walkCtrl", demoMode->clipPool, j, rate, fps);

		j = a3clipGetIndexInPool(demoMode->clipPool, "xbot_run_f");//"xbot_idle_pistol");
		distributeTime = demoMode->clipPool[0].clip[j].keyframeCount / CYCLE_DURATION;
		a3clipDistributeDuration(&(demoMode->clipPool[0]), j, distributeTime);
		a3clipControllerInit(demoMode->runClipCtrl, "xbot_runCtrl", demoMode->clipPool, j, rate, fps);

		// finally set up hierarchy states
		// base state for skeleton
		hierarchyState = demoMode->hierarchyState_skel_base;
		hierarchyState->hierarchy = 0;
		a3hierarchyStateCreate(hierarchyState, hierarchy);
		a3hierarchyPoseCopy(hierarchyState->localSpace, hierarchyPoseGroup->hpose, hierarchy->numNodes);
		a3hierarchyPoseConvert(hierarchyState->localSpace, hierarchy->numNodes, hierarchyPoseGroup->channel, hierarchyPoseGroup->order, a3root_All);
		a3kinematicsSolveForward(hierarchyState);
		a3hierarchyPoseRestore(hierarchyState->objectSpace, hierarchy->numNodes, hierarchyPoseGroup->channel, hierarchyPoseGroup->order);
		a3hierarchyStateUpdateObjectInverse(hierarchyState);

		// FK state
		hierarchyState = demoMode->hierarchyState_skel_fk;
		hierarchyState->hierarchy = 0;
		a3hierarchyStateCreate(hierarchyState, hierarchy);

		// IK state
		hierarchyState = demoMode->hierarchyState_skel_ik;
		hierarchyState->hierarchy = 0;
		a3hierarchyStateCreate(hierarchyState, hierarchy);

		// final blend state
		hierarchyState = demoMode->hierarchyState_skel_final;
		hierarchyState->hierarchy = 0;
		a3hierarchyStateCreate(hierarchyState, hierarchy);


		/////////// TESTING TRANSITION BRANCHING ////////////////////
		//Set test clip transition that returns true when receiving forward input
		a3i32 idleClipIndex = a3clipGetIndexInPool(demoMode->clipPool, "xbot_idle_f");
		demoMode->clipPool[0].clip[idleClipIndex].transitionForward[0].clipTransitionBranch = &a3checkForInputBranchFunction;
		demoMode->clipPool[0].clip[idleClipIndex].transitionForward[0].flag = a3clip_playFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[idleClipIndex].transitionForward[0].parameters = demoMode;

		demoMode->clipPool[0].clip[idleClipIndex].transitionReverse[0].clipTransitionBranch = &a3checkForInputBranchFunction;
		demoMode->clipPool[0].clip[idleClipIndex].transitionReverse[0].flag = a3clip_playFlag | a3clip_reverseFlag | a3clip_terminusFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[idleClipIndex].transitionReverse[0].parameters = demoMode;


		a3i32 walkClipIndex = a3clipGetIndexInPool(demoMode->clipPool, "xbot_walk_f");
		demoMode->clipPool[0].clip[walkClipIndex].rootMotion = a3root_AllRot | a3root_YPosition;

		demoMode->clipPool[0].clip[walkClipIndex].transitionForward[0].flag = a3clip_playFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[walkClipIndex].transitionForward[0].parameters = demoMode;

		demoMode->clipPool[0].clip[walkClipIndex].transitionReverse[0].flag = a3clip_playFlag | a3clip_reverseFlag | a3clip_terminusFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[walkClipIndex].transitionReverse[0].parameters = demoMode;


		a3i32 runClipIndex = a3clipGetIndexInPool(demoMode->clipPool, "xbot_run_f");
		demoMode->clipPool[0].clip[runClipIndex].rootMotion = a3root_AllRot | a3root_YPosition;

		demoMode->clipPool[0].clip[runClipIndex].transitionForward[0].flag = a3clip_playFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[runClipIndex].transitionForward[0].parameters = demoMode;

		demoMode->clipPool[0].clip[runClipIndex].transitionReverse[0].flag = a3clip_playFlag | a3clip_reverseFlag | a3clip_terminusFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[runClipIndex].transitionReverse[0].parameters = demoMode;


		a3i32 jumpClipIndex = a3clipGetIndexInPool(demoMode->clipPool, "xbot_jump_f");
		demoMode->clipPool[0].clip[jumpClipIndex].rootMotion = a3root_AllRot | a3root_YPosition;

		demoMode->clipPool[0].clip[jumpClipIndex].transitionForward[0].flag = a3clip_playFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[jumpClipIndex].transitionForward[0].parameters = demoMode;

		demoMode->clipPool[0].clip[jumpClipIndex].transitionReverse[0].flag = a3clip_playFlag | a3clip_reverseFlag | a3clip_terminusFlag | a3clip_overstepFlag;
		demoMode->clipPool[0].clip[jumpClipIndex].transitionReverse[0].parameters = demoMode;
			


		demoMode->clipPool[0].clip[idleClipIndex].transitionForward[0].trueClipIndex = walkClipIndex;
		demoMode->clipPool[0].clip[idleClipIndex].transitionReverse[0].trueClipIndex = walkClipIndex;
		demoMode->clipPool[0].clip[idleClipIndex].transitionForward[0].falseClipIndex = idleClipIndex;
		demoMode->clipPool[0].clip[idleClipIndex].transitionReverse[0].falseClipIndex = idleClipIndex;

		demoMode->clipPool[0].clip[walkClipIndex].transitionForward[0].falseClipIndex = idleClipIndex;
		demoMode->clipPool[0].clip[walkClipIndex].transitionReverse[0].falseClipIndex = idleClipIndex;
		demoMode->clipPool[0].clip[walkClipIndex].transitionForward[0].trueClipIndex = walkClipIndex;
		demoMode->clipPool[0].clip[walkClipIndex].transitionReverse[0].trueClipIndex = walkClipIndex;
		/////////////////////////////////////////////////////////////

		// xbot
		demoMode->obj_skeleton->euler.x = a3real_ninety;
		demoMode->obj_skeleton->euler.y = a3real_oneeighty;

		// control node
		// Original control init parameters
		// Looks like it didn't really matter since it was being immediately overwritten in update with demoMode->pos, etc.
		demoMode->obj_skeleton_ctrl->position.y = +a3real_four;
		demoMode->obj_skeleton_ctrl->euler.z = a3real_oneeighty;

		// Control Node - Joey
		demoMode->obj_skeleton_ctrl->position = (a3vec3){ .x = 0, .y = 0, .z = 0 };
		demoMode->obj_skeleton_ctrl->euler = (a3vec3){ .x = 0, .y = 0, .z = 0 };
		demoMode->obj_skeleton_ctrl->scale = (a3vec3){ .x = 1, .y = 1, .z = 1 };

		demoMode->xcontrolSensitivity = 2;
		demoMode->mouseSensitivity = (a3real).25;

		demoMode->pitchLimits.x = -50;
		demoMode->pitchLimits.y = 50;

		demoMode->pitch = 0;

		// effectors
		// do one update to get first pose for target IK frame
		{
			void a3animation_update_animation(a3_DemoMode1_Animation * demoMode, a3f64 const dt, a3boolean const updateIK);
			void a3animation_update_sceneGraph(a3_DemoMode1_Animation * demoMode, a3f64 const dt);
			for (p = 0; p < 3; ++p)
			{
				a3animation_update_animation(demoMode, 0.0, 0);
				a3animation_update_sceneGraph(demoMode, 0.0);
				a3animation_load_resetEffectors(demoMode, demoMode->hierarchyState_skel_fk, hierarchyPoseGroup);
			}
		}
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
	a3ui32 i, j;

	// initialize callbacks
	a3_DemoModeCallbacks* const callbacks = demoState->demoModeCallbacks + demoState_modeAnimation;
	callbacks->demoMode = demoMode;
	callbacks->handleInput = (a3_DemoMode_EventCallback)a3animation_input;
	callbacks->handleUpdate = (a3_DemoMode_EventCallback)a3animation_update;
	callbacks->handleRender = (a3_DemoMode_EventCallbackConst)a3animation_render;
	callbacks->handleKeyPress = (a3_DemoMode_InputCallback)a3animation_input_keyCharPress;
	callbacks->handleKeyHold = (a3_DemoMode_InputCallback)a3animation_input_keyCharHold;

	// initialize cameras dependent on viewport
	demoMode->proj_camera_main->aspect = demoState->frameAspect;
	a3demo_updateProjectorProjectionMat(demoMode->proj_camera_main);
	a3demo_setProjectorSceneObject(demoMode->proj_camera_main, demoMode->obj_camera_main);

	// initialize cameras not dependent on viewport

	// animation
	demoMode->sceneGraphState->hierarchy = demoMode->sceneGraph;
	demoMode->hierarchyPoseGroup_skel->hierarchy = demoMode->hierarchy_skel;
	demoMode->clipCtrl->clipPool = demoMode->clipPool;
	demoMode->clipCtrl->clip = demoMode->clipPool->clip + demoMode->clipCtrl->clipIndex;
	demoMode->clipCtrl->keyframe = demoMode->clipPool->keyframe + demoMode->clipCtrl->keyframeIndex;
	for (i = 0, j = sizeof(demoMode->hierarchyState_skel) / sizeof(a3_HierarchyState); i < j; ++i)
		demoMode->hierarchyState_skel[i].hierarchy = demoMode->hierarchy_skel;
}


// Initialize ctrl node with data from skeleton scene object
void a3animation_initCtrlNode(a3_DemoMode1_Animation* demoMode)
{
	demoMode->ctrlNode = (a3_SpatialPose*) malloc(sizeof(a3_SpatialPose));

	a3spatialPoseSetScale(demoMode->ctrlNode, demoMode->obj_skeleton_ctrl->scale.x,
		demoMode->obj_skeleton_ctrl->scale.y, demoMode->obj_skeleton_ctrl->scale.z);
	
	a3spatialPoseSetRotation(demoMode->ctrlNode, demoMode->obj_skeleton_ctrl->euler.x,
		demoMode->obj_skeleton_ctrl->euler.y, demoMode->obj_skeleton_ctrl->euler.z);
	
	a3spatialPoseSetTranslation(demoMode->ctrlNode, demoMode->obj_skeleton_ctrl->position.x,
		demoMode->obj_skeleton_ctrl->position.y, demoMode->obj_skeleton_ctrl->position.z);

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
	/*for (i = 0; i < animationMaxCount_cameraObject; ++i)
		a3demo_initSceneObject(demoMode->object_camera + i);*/
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
	a3animation_initCtrlNode(demoMode);
	a3animation_initBlendTree(demoMode);
}


//-----------------------------------------------------------------------------
