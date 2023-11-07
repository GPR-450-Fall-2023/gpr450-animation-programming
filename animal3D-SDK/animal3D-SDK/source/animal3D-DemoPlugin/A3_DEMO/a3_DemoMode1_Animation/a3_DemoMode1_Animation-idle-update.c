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
	
	a3_DemoMode1_Animation-idle-update.c
	Demo mode implementations: animation scene.

	********************************************
	*** UPDATE FOR ANIMATION SCENE MODE      ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// UTILS

inline a3real4r a3demo_mat2quat_safe(a3real4 q, a3real4x4 const m)
{
	// ****TO-DO: 
	//	-> convert rotation part of matrix to quaternion
	//	-> NOTE: this is for testing dual quaternion skinning only; 
	//		quaternion data would normally be computed with poses

	a3real4SetReal4(q, a3vec4_w.v);

	// done
	return q;
}

inline a3real4x2r a3demo_mat2dquat_safe(a3real4x2 Q, a3real4x4 const m)
{
	// ****TO-DO: 
	//	-> convert matrix to dual quaternion
	//	-> NOTE: this is for testing dual quaternion skinning only; 
	//		quaternion data would normally be computed with poses

	a3demo_mat2quat_safe(Q[0], m);
	a3real4SetReal4(Q[1], a3vec4_zero.v);

	// done
	return Q;
}


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update_objects(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox);
void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_DemoSceneObject* sceneObject, a3real4x4p s);

void a3animation_updateSkeletonCtrl(a3_DemoMode1_Animation* demoMode)
{
	a3_SpatialPose* node = demoMode->ctrlNode;
	a3_DemoSceneObject* ctrl = demoMode->obj_skeleton_ctrl;

	ctrl->position.x = node->translate.x;
	ctrl->position.y = node->translate.y;
	ctrl->position.z = node->translate.z;
	
	ctrl->euler.x = node->rotate.x;
	ctrl->euler.y = node->rotate.y;
	ctrl->euler.z = node->rotate.z;

	ctrl->scale.x = node->scale.x;
	ctrl->scale.y = node->scale.y;
	ctrl->scale.z = node->scale.z;
}

void a3handleLocomotionInput(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	//a3_DemoMode1_Animation_InputMode

	a3real realDT = (a3real)dt;


	// Define constants for animation inputs
	const a3real MOVE_MAGNITUDE = 2.5;
	const a3real ROT_MAGNITUDE = 180;

	const a3real VEL_MAGNITUDE = 4;
	const a3real ACC_MAGNITUDE = 4;

	const a3real ANG_VEL_MAGNITUDE = 45;
	const a3real ANG_ACC_MAGNITUDE = 45;

	const a3real INTEGRATE1_DT_MULT = 3;
	const a3real INTEGRATE2_DT_MULT = 6;

	a3vec2 posInput;
	posInput.x = (a3real)demoMode->axis_l[0];
	posInput.y = (a3real)demoMode->axis_l[1];

	a3real rotInput = (a3real)demoMode->axis_r[0];;

	demoMode->ctrlInputsRegistered = posInput.x != 0 || posInput.y != 0 || rotInput != 0;


	a3vec2 posResult = { demoMode->ctrlNode->translate.x, demoMode->ctrlNode->translate.y };
	a3real rotResult = { demoMode->ctrlNode->rotate.z };


	a3vec2 posIntegrateResult;
	a3real rotIntegrateResult;

	///////// Spine Rotation
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		demoMode->pitch -= (a3real)demoMode->axis_r[1] * demoMode->xcontrolSensitivity;
		rotInput *= -demoMode->xcontrolSensitivity;
	}
	else
	{
		demoMode->pitch -= (a3real)demoMode->axis_r[1] * demoMode->mouseSensitivity;
		rotInput *= demoMode->mouseSensitivity;
	}

	demoMode->pitch = a3clamp(demoMode->pitchLimits.x, demoMode->pitchLimits.y, demoMode->pitch);
	//////////////


	switch (demoMode->ctrl_position)
	{
	case animation_input_direct:
		posResult = (a3vec2){ posInput.x * MOVE_MAGNITUDE, posInput.y * MOVE_MAGNITUDE };
		break;

	case animation_input_euler:

		demoMode->ctrlVelocity = (a3vec2){ posInput.x * VEL_MAGNITUDE, posInput.y * VEL_MAGNITUDE };

		posResult = fIntegrateEuler2(posResult, demoMode->ctrlVelocity, realDT);

		break;

	case animation_input_kinematic:
		break;

	case animation_input_interpolate1:

		posInput.x *= MOVE_MAGNITUDE;
		posInput.y *= MOVE_MAGNITUDE;

		posIntegrateResult = fIntegrateInterpolation2(posResult, posInput, realDT * INTEGRATE1_DT_MULT);

		posResult.x = posIntegrateResult.x;
		posResult.y = posIntegrateResult.y;

		break;

	case animation_input_interpolate2:

		posInput.x *= VEL_MAGNITUDE;
		posInput.y *= VEL_MAGNITUDE;

		demoMode->ctrlVelocity = fIntegrateInterpolation2(demoMode->ctrlVelocity, posInput, realDT * INTEGRATE2_DT_MULT);

		posResult = fIntegrateEuler2(posResult, demoMode->ctrlVelocity, realDT);

		break;
	}


	switch (demoMode->ctrl_rotation)
	{
	case animation_input_direct:
		rotResult = rotInput * ROT_MAGNITUDE;
		break;

	case animation_input_euler:

		demoMode->ctrlAngularVelocity = rotInput * ANG_VEL_MAGNITUDE;

		rotResult = fIntegrateEuler1(rotResult, demoMode->ctrlAngularVelocity, realDT);

		break;

	case animation_input_kinematic:
		break;

	case animation_input_interpolate1:

		rotInput *= ROT_MAGNITUDE;

		rotIntegrateResult = fIntegrateInterpolation1(rotResult, rotInput, realDT * INTEGRATE1_DT_MULT);

		rotResult = rotIntegrateResult;

		break;

	case animation_input_interpolate2:

		rotInput *= ROT_MAGNITUDE;

		demoMode->ctrlAngularVelocity = fIntegrateInterpolation1(demoMode->ctrlAngularVelocity, rotInput, realDT * INTEGRATE2_DT_MULT);

		rotResult = fIntegrateEuler1(rotResult, demoMode->ctrlAngularVelocity, realDT);

		break;
	}

	// Make sure rotation is between 0 and 360 degrees
	rotResult = fmodf(rotResult, 360.0f);

	demoMode->ctrlNode->translate = (a3vec4){ posResult.x, posResult.y, 0, demoMode->ctrlNode->translate.w };
	demoMode->ctrlNode->rotate = (a3vec4){ 0, 0, rotResult, demoMode->ctrlNode->translate.w };

}

void a3animation_update(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	a3ui32 i, j;
	a3_DemoModelMatrixStack matrixStack[animationMaxCount_sceneObject];

	a3_HierarchyState* activeHS = demoMode->hierarchyState_skel + 1, * baseHS = demoMode->hierarchyState_skel;

	// active camera
	a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(demoState, dt,
		demoMode->object_scene, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_scene_ctrl, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_camera, animationMaxCount_cameraObject, 1, 0);

	a3demo_updateProjectorViewProjectionMat(demoMode->proj_camera_main);

	// apply scales to objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		a3demo_applyScale_internal(demoMode->object_scene + i, scaleMat.m);
		a3demo_applyScale_internal(demoMode->object_scene_ctrl + i, scaleMat.m);
	}

	// update skybox
	a3demo_update_bindSkybox(demoMode->obj_camera_main, demoMode->obj_skybox);
	
	// update scene graph local transforms
	j = 0;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = a3mat4_identity;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = demoMode->object_camera->modelMat;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = a3mat4_identity;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = demoMode->obj_skeleton_ctrl->modelMat;
	// start of scene objects
	demoMode->sceneGraphState->localSpace->pose[j + 0].transformMat = demoMode->obj_skybox->modelMat;
	demoMode->sceneGraphState->localSpace->pose[j + 1].transformMat = demoMode->obj_skeleton->modelMat;
	a3kinematicsSolveForward(demoMode->sceneGraphState);
	a3hierarchyStateUpdateObjectInverse(demoMode->sceneGraphState);

	// update matrix stack data using scene graph
	for (i = 0; i < animationMaxCount_sceneObject; ++i, ++j)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m,
			demoMode->sceneGraphState->objectSpace->pose[demoMode->obj_camera_main->sceneGraphIndex].transformMat.m,
			demoMode->sceneGraphState->objectSpaceInv->pose[demoMode->obj_camera_main->sceneGraphIndex].transformMat.m,
			demoMode->sceneGraphState->objectSpace->pose[demoMode->obj_skeleton->sceneGraphIndex].transformMat.m,
			a3mat4_identity.m);
	}

	//printf("Movement: (%f, %f)   Rotate: (%f, %f)\n", demoMode->axis_l[0], demoMode->axis_l[1], demoMode->axis_r[0], demoMode->axis_r[1]);

	a3handleLocomotionInput(demoState, demoMode, dt);

	// skeletal
	if (demoState->updateAnimation)
	{
		a3real const dtr = (a3real)dt;
		a3_ClipController* clipCtrl = demoMode->clipCtrlA;

		// update controllers
		a3clipControllerUpdate(demoMode->clipCtrl, dt);
		a3clipControllerUpdate(demoMode->clipCtrlA, dt);
		a3clipControllerUpdate(demoMode->clipCtrlB, dt);
		
		/*system("cls");
		printf("Clip Index: %i\nClip Param: %f\nClip Time: %f\nKeyframe Index: %i\nKeyframe Param: %f\nKeyframe Time: %f\nPlayback Speed: %f\n\n",
			demoMode->clipCtrlA->clipIndex,
			demoMode->clipCtrlA->clipParam,
			demoMode->clipCtrlA->clipTime_sec,
			demoMode->clipCtrlA->keyframeIndex,
			demoMode->clipCtrlA->keyframeParam,
			demoMode->clipCtrlA->keyframeTime_sec,
			demoMode->clipCtrlA->playback_sec);*/

		//////////////////// TESTING TRANSITION BRANCHING //////////////////////////
		//a3_Clip* currentClip = &demoMode->clipCtrlA->clipPool->clip[demoMode->clipCtrlA->clipIndex];

		////Null check
		//if (currentClip->transitionForward[0].clipTransitionBranch)
		//{
		//	//Call function pointer and pass in demoMode
		//	if ((*currentClip->transitionForward[0].clipTransitionBranch)(demoMode))
		//	{
		//		//If true, print
		//		//printf("Function pointer works on forward input");
		//	}
		//}
		/////////////////////////////////////////////////////////////////////////////

		// STEP
	//	a3hierarchyPoseCopy(activeHS->animPose,
	//		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipCtrl->keyframeIndex,
	//		demoMode->hierarchy_skel->numNodes);

		// LERP
		a3hierarchyPoseLerp(activeHS->animPose,
			demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex0,
			demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex1,
			(a3f32)clipCtrl->keyframeParam, demoMode->hierarchy_skel->numNodes);
		
		a3vec3 rotateSpine = { demoMode->pitch, 0, 0 };
		//a3hierarchyPoseOpRotateBoneName(activeHS->animPose, activeHS->hierarchy, rotateSpine, "mixamorig:Spine");
		a3hierarchyPoseOpRotateBoneRange(activeHS->animPose, activeHS->hierarchy, rotateSpine, "mixamorig:Spine", "mixamorig:Spine2");

		/*printf("Spine: (%f, %f, %f)   Root: (%f, %f, %f)\n",
			activeHS->animPose->pose[1].rotate.x,
			activeHS->animPose->pose[1].rotate.y,
			activeHS->animPose->pose[1].rotate.z,
			activeHS->animPose->pose[0].rotate.x,
			activeHS->animPose->pose[0].rotate.y,
			activeHS->animPose->pose[0].rotate.z);*/
		//////////////////////////

		// FK pipeline
		a3hierarchyPoseConcat(activeHS->localSpace,	// goal to calculate
			baseHS->localSpace, // holds base pose
			activeHS->animPose, // holds current sample pose
			demoMode->hierarchy_skel->numNodes);
		a3hierarchyPoseConvert(activeHS->localSpace,
			demoMode->hierarchy_skel->numNodes,
			demoMode->hierarchyPoseGroup_skel->channel,
			demoMode->hierarchyPoseGroup_skel->order);
		a3kinematicsSolveForward(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);

		// ****TO-DO: 
		// process input

		// apply input
		// Deprecated, now getting values from spatial pose - Joey
		/*demoMode->obj_skeleton_ctrl->position.x = +(demoMode->pos.x);
		demoMode->obj_skeleton_ctrl->position.y = +(demoMode->pos.y);
		demoMode->obj_skeleton_ctrl->euler.z = -a3trigValid_sind(demoMode->rot);*/
	}


	// prepare and upload graphics data
	{
		a3addressdiff const skeletonIndex = demoMode->obj_skeleton - demoMode->object_scene;
		a3ui32 const mvp_size = demoMode->hierarchy_skel->numNodes * sizeof(a3mat4);
		a3ui32 const t_skin_size = sizeof(demoMode->t_skin);
		a3ui32 const dq_skin_size = sizeof(demoMode->dq_skin);
		a3mat4 const mvp_obj = matrixStack[skeletonIndex].modelViewProjectionMat;
		a3mat4* mvp_joint, * mvp_bone, * t_skin;
		a3dualquat* dq_skin;
		a3index i;
		a3i32 p;
		
		// update joint and bone transforms
		for (i = 0; i < demoMode->hierarchy_skel->numNodes; ++i)
		{
			mvp_joint = demoMode->mvp_joint + i;
			mvp_bone = demoMode->mvp_bone + i;
			t_skin = demoMode->t_skin + i;
			dq_skin = demoMode->dq_skin + i;

			// joint transform
			a3real4x4SetScale(scaleMat.m, a3real_quarter);
			a3real4x4Concat(activeHS->objectSpace->pose[i].transformMat.m, scaleMat.m);
			a3real4x4Product(mvp_joint->m, mvp_obj.m, scaleMat.m);
			
			// bone transform
			p = demoMode->hierarchy_skel->nodes[i].parentIndex;
			if (p >= 0)
			{
				// position is parent joint's position
				scaleMat.v3 = activeHS->objectSpace->pose[p].transformMat.v3;

				// direction basis is from parent to current
				a3real3Diff(scaleMat.v2.v,
					activeHS->objectSpace->pose[i].transformMat.v3.v, scaleMat.v3.v);

				// right basis is cross of some upward vector and direction
				// select 'z' for up if either of the other dimensions is set
				a3real3MulS(a3real3CrossUnit(scaleMat.v0.v,
					a3real2LengthSquared(scaleMat.v2.v) > a3real_zero
					? a3vec3_z.v : a3vec3_y.v, scaleMat.v2.v), a3real_quarter);
			
				// up basis is cross of direction and right
				a3real3MulS(a3real3CrossUnit(scaleMat.v1.v,
					scaleMat.v2.v, scaleMat.v0.v), a3real_quarter);
			}
			else
			{
				// if we are a root joint, make bone invisible
				a3real4x4SetScale(scaleMat.m, a3real_zero);
			}
			a3real4x4Product(mvp_bone->m, mvp_obj.m, scaleMat.m);

			// get base to current object-space
			*t_skin = activeHS->objectSpaceBindToCurrent->pose[i].transformMat;

			// calculate DQ
			a3demo_mat2dquat_safe(dq_skin->Q, t_skin->m);
		}
		
		// upload
		a3bufferRefill(demoState->ubo_transformMVP, 0, mvp_size, demoMode->mvp_joint);
		a3bufferRefill(demoState->ubo_transformMVPB, 0, mvp_size, demoMode->mvp_bone);
		a3bufferRefill(demoState->ubo_transformBlend, 0, t_skin_size, demoMode->t_skin);
		a3bufferRefillOffset(demoState->ubo_transformBlend, 0, t_skin_size, dq_skin_size, demoMode->dq_skin);


		// Update obj_skeleton_ctrl with values from ctrlNode
		a3animation_updateSkeletonCtrl(demoMode);
	}
}


//-----------------------------------------------------------------------------
