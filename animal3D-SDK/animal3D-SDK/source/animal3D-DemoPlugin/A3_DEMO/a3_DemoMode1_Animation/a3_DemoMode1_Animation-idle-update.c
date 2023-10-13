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

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_DemoMode1_Animation-idle-update.c
	Handle testing interface and playback logic for animating skeleton
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

	return q;
}


//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update_objects(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox);
void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_DemoSceneObject* sceneObject, a3real4x4p s);


// Called in update, handle the testing interface
void a3demo_animation_testingInterfaceInput(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	// Try cycle through hierarchyState when F pressed
	if (a3keyboardIsPressed(demoState->keyboard, a3key_F))
	{
		demoMode->hierarchyStateIndex++;

		if (demoMode->hierarchyStateIndex >= 4)
		{
			demoMode->hierarchyStateIndex = 1;
		}

		printf("%i", demoMode->hierarchyStateIndex);
	}

	// Debug inputs
	//printf("%i", (int) demoState->hierarchyState);
	//printf("Button pressed this frame: %i", a3keyboardIsPressed(demoState->keyboard, a3key_Y));
	//printf("\n");
}


void a3animation_update(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	a3ui32 i;
	a3_DemoModelMatrixStack matrixStack[animationMaxCount_sceneObject];

	a3ui32 actualIndex = demoMode->hierarchyStateIndex + 1;

	//Current hierarchy state that we are displaying and modifying				base state with the bind pose
	a3_HierarchyState* activeHS = demoMode->hierarchyState_skel + actualIndex, *baseHS = demoMode->hierarchyState_skel;

	// active camera
	a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(demoState, dt,
		demoMode->object_scene, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_camera, animationMaxCount_cameraObject, 1, 0);

	a3demo_updateProjectorViewProjectionMat(demoMode->proj_camera_main);

	// apply scales to objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
		a3demo_applyScale_internal(demoMode->object_scene + i, scaleMat.m);

	// update skybox
	a3demo_update_bindSkybox(demoMode->obj_camera_main, demoMode->obj_skybox);

	// update matrix stack data
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m, activeCameraObject->modelMat.m, activeCameraObject->modelMatInv.m,
			demoMode->object_scene[i].modelMat.m, a3mat4_identity.m);
	}

	/////////////// Animation Update /////////////////////

	a3_ClipController* clipCtrl = &demoMode->clipCtrlPool.clipControllers[demoMode->currentController];

	//Set clip if switched to different one in input
	if (demoMode->currentClip != demoMode->previousFrameClip) {
		a3clipControllerSetClip(clipCtrl, clipCtrl->clipPool, demoMode->currentClip);
	}

	// turning play/pause/slowmo/forward/backward into effect
	if (demoMode->togglePause) {
		if(clipCtrl->playbackDirection == 0) {
			clipCtrl->playbackDirection = clipCtrl->lastPlaybackDirection;
		}
		else {
			clipCtrl->playbackDirection = 0;
		}
		demoMode->togglePause = false;
	}

	if (demoMode->shouldRewind) {	//backward
		clipCtrl->playbackDirection *= -1;
		demoMode->shouldRewind = false;
	}

	//Clear terminal
	//system("cls");

	//Update all controllers
	for (a3ui32 index = 0; index < demoMode->clipCtrlPool.count; index++)
	{
		a3clipControllerUpdate(&demoMode->clipCtrlPool.clipControllers[index], (a3real)dt);
	}

	//Save index of previous clip to check if clip has changed later
	demoMode->previousFrameClip = demoMode->currentClip;
	demoMode->previousFrameClipCtrl = demoMode->currentController;

	//////////////////////////////////////////////////////


	// skeletal, determine current and next keyframe for interpolation
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip]; //Current clip
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe]; //Current keyframe
	a3_Keyframe nextKeyframe = keyframe;
	if (demoState->updateAnimation && activeHS != baseHS)
	{
		if (clipCtrl->playbackDirection >= 0) //Use getNextKeyframe of forwardTransition
		{
			clip.forwardTransition.getNextKeyframe(clipCtrl, &nextKeyframe, 1);
		}
		else if (clipCtrl->keyframe >= clipCtrl->firstKeyframeOfCurrentPlayback) //Use last clip backwardTransition.getNextKeyframe
		{
			clipCtrl->clipPool->clip[clipCtrl->lastClip].backwardTransition.getNextKeyframe(clipCtrl, &nextKeyframe, 1);
		}
		else //Use getNextKeyframe from this clips backwardTransition
		{
			clip.backwardTransition.getNextKeyframe(clipCtrl, &nextKeyframe, 1);
		}
	}

	//Interpolation
	switch (demoMode->interpFunc)
	{
	case interp_smoothstep:
		a3hierarchyPoseSmoothstep(activeHS->objectSpace,	// use as temp storage
			keyframe.data,
			nextKeyframe.data,
			clipCtrl->keyframeParameter,
			demoMode->hierarchy_skel->numNodes);
		break;

	case interp_lerp:
		a3hierarchyPoseLerp(activeHS->objectSpace,	// use as temp storage
			keyframe.data,
			nextKeyframe.data,
			clipCtrl->keyframeParameter,
			demoMode->hierarchy_skel->numNodes);
		break;

	case interp_nearest:
		a3hierarchyPoseNearest(activeHS->objectSpace,	// use as temp storage
			keyframe.data,
			nextKeyframe.data,
			clipCtrl->keyframeParameter,
			demoMode->hierarchy_skel->numNodes);
		break;

	case interp_step:
	default:
		a3hierarchyPoseStep(activeHS->objectSpace,	// use as temp storage
			keyframe.data,
			demoMode->hierarchy_skel->numNodes);
		break;
	}

	//Combine matrices
	a3hierarchyPoseConcat(activeHS->localSpace,	// goal to calculate
		baseHS->localSpace, // holds base pose
		activeHS->objectSpace, // temp storage
		demoMode->hierarchy_skel->numNodes);

	//Create transform matrices
	a3hierarchyPoseConvert(activeHS->localSpace,
		demoMode->hierarchy_skel->numNodes,
		demoMode->hierarchyPoseGroup_skel->channel,
		demoMode->hierarchyPoseGroup_skel->order);

	//Solve for kinematics
	a3kinematicsSolveForward(activeHS);

	//Skinning data
	a3hierarchyStateUpdateObjectInverse(activeHS);
	a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);


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
			a3real4x4Concat(activeHS->objectSpace->sPoses[i].transform.m, scaleMat.m);
			a3real4x4Product(mvp_joint->m, mvp_obj.m, scaleMat.m);
			
			// bone transform
			p = demoMode->hierarchy_skel->nodes[i].parentIndex;
			if (p >= 0)
			{
				// position is parent joint's position
				scaleMat.v3 = activeHS->objectSpace->sPoses[p].transform.v3;

				// direction basis is from parent to current
				a3real3Diff(scaleMat.v2.v,
					activeHS->objectSpace->sPoses[i].transform.v3.v, scaleMat.v3.v);

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
			*t_skin = activeHS->objectSpaceBindToCurrent->sPoses[i].transform;
		
			// calculate DQ
			{
				a3real4 d = { a3real_zero };
				a3demo_mat2quat_safe(dq_skin->r.q, t_skin->m);
				a3real3ProductS(d, t_skin->v3.v, a3real_half);
				a3quatProduct(dq_skin->d.q, d, dq_skin->r.q);
			}
		}
		
		// upload
		a3bufferRefill(demoState->ubo_transformMVP, 0, mvp_size, demoMode->mvp_joint);
		a3bufferRefill(demoState->ubo_transformMVPB, 0, mvp_size, demoMode->mvp_bone);
		a3bufferRefill(demoState->ubo_transformBlend, 0, t_skin_size, demoMode->t_skin);
		a3bufferRefillOffset(demoState->ubo_transformBlend, 0, t_skin_size, dq_skin_size, demoMode->dq_skin);

		//a3demo_animation_testingInterfaceInput(demoState, demoMode, dt);
	}
}


//-----------------------------------------------------------------------------
