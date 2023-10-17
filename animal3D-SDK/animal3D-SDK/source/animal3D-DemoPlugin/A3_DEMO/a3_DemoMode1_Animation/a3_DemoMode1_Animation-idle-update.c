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

#include <stdio.h>


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

void a3animation_update(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	a3ui32 i;
	a3_DemoModelMatrixStack matrixStack[animationMaxCount_sceneObject];
	 
	a3_HierarchyState* baseHS = demoMode->hierarchyState_skel,
		* activeHS = demoMode->hierarchyState_skel + 1,
		* pose0HS = demoMode->hierarchyState_skel + 2,
		* pose1HS = demoMode->hierarchyState_skel + 3;

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

	// turning play/pause/slowmo/forward/backward into effect
	/*if (demoMode->togglePause) {
		if (demoState->playbackDirection == 0) {
			clipCtrl->playbackDirection = clipCtrl->lastPlaybackDirection;
		}
		else {
			clipCtrl->playbackDirection = 0;
		}
		demoMode->togglePause = false;
	}*/

	//if (demoMode->shouldRewind) {	//backward
	//	clipCtrl->playbackDirection *= -1;
	//	demoMode->shouldRewind = false;
	//}


	// skeletal idle animation
	if (demoState->updateAnimation)
	{
		i = (a3ui32)(demoState->timer_display->totalTime);
		a3ui32 start = 28;//Hardcoded to idle keyframes for now
		a3ui32 end = 52;
		a3ui32 length = end - start; 
		demoMode->hierarchyKeyPose_display[0] = ((i + 0) % (length)) + start;
		demoMode->hierarchyKeyPose_display[1] = ((i + 1) % (length)) + start;
		demoMode->hierarchyKeyPose_param = (a3real)(demoState->timer_display->totalTime - (a3f64)i);
	}

	//Pose 0
	a3hierarchyPoseLerp(pose0HS->objectSpace,	// use as temp storage
		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->hierarchyKeyPose_display[0] + 1,
		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->hierarchyKeyPose_display[1] + 1,
		demoMode->hierarchyKeyPose_param,
		demoMode->hierarchy_skel->numNodes);
	a3hierarchyPoseConcat(pose0HS->localSpace,	// goal to calculate
		baseHS->localSpace, // holds base pose
		pose0HS->objectSpace, // temp storage
		demoMode->hierarchy_skel->numNodes);
	a3hierarchyPoseConvert(pose0HS->localSpace,
		demoMode->hierarchy_skel->numNodes,
		demoMode->hierarchyPoseGroup_skel->channel,
		demoMode->hierarchyPoseGroup_skel->order);
	a3kinematicsSolveForward(pose0HS);
	a3hierarchyStateUpdateObjectInverse(pose0HS);
	a3hierarchyStateUpdateObjectBindToCurrent(pose0HS, baseHS);

	// skeletal dance animation
	if (demoState->updateAnimation)
	{
		i = (a3ui32)(demoState->timer_display->totalTime);
		a3ui32 start = 54; //Hardcoded to dance keyframes for now
		a3ui32 end = 78;
		a3ui32 length = end - start; //Hardcoded to idle keyframes for now
		demoMode->hierarchyKeyPose_display[0] = ((i + 0) % (length)) + start;
		demoMode->hierarchyKeyPose_display[1] = ((i + 1) % (length)) + start;
		demoMode->hierarchyKeyPose_param = (a3real)(demoState->timer_display->totalTime - (a3f64)i);
	}

	//Pose 1
	a3hierarchyPoseLerp(pose1HS->objectSpace,	// use as temp storage
		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->hierarchyKeyPose_display[0] + 1,
		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->hierarchyKeyPose_display[1] + 1,
		demoMode->hierarchyKeyPose_param,
		demoMode->hierarchy_skel->numNodes);
	a3hierarchyPoseConcat(pose1HS->localSpace,	// goal to calculate
		baseHS->localSpace, // holds base pose
		pose1HS->objectSpace, // temp storage
		demoMode->hierarchy_skel->numNodes);
	a3hierarchyPoseConvert(pose1HS->localSpace,
		demoMode->hierarchy_skel->numNodes,
		demoMode->hierarchyPoseGroup_skel->channel,
		demoMode->hierarchyPoseGroup_skel->order);
	a3kinematicsSolveForward(pose1HS);
	a3hierarchyStateUpdateObjectInverse(pose1HS);
	a3hierarchyStateUpdateObjectBindToCurrent(pose1HS, baseHS);

	
	//////// TODO - Blend new pose from inputs based on current blend function
	switch (demoMode->blendMode)
	{
	case blend_identity:
		a3hierarchyPoseOpIdentity(activeHS->localSpace, activeHS->hierarchy->numNodes);
		break;
	case blend_construct:
		//a3hierarchyPoseOpConstruct(activeHS->localSpace, activeHS->hierarchy->numNodes, 0, 0, 0);
		break;
	case blend_copy:
		a3hierarchyPoseOpCopy(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace);
		break;
	case blend_negate:
		a3hierarchyPoseOpNegate(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace);
		break;
	case blend_concatenate:
		a3hierarchyPoseOpConcatenate(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace, pose1HS->localSpace);
		break;
	case blend_nearest:
		a3hierarchyPoseOpNearest(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace, pose1HS->localSpace, .5f);
		break;
	case blend_lerp:
		a3hierarchyPoseOpLERP(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace, pose1HS->localSpace, .5f);
		break;
	case blend_cubic:
		//a3hierarchyPoseOpCubic(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace, pose1HS->localSpace, )
		break;
	case blend_deconcatenate:
		a3hierarchyPoseOpDeconcatenate(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace, pose1HS->localSpace);
		break;
	case blend_scale:
		a3hierarchyPoseOpScale(activeHS->localSpace, activeHS->hierarchy->numNodes, pose0HS->localSpace, .5f);
		break;
	case blend_triangular:
			
		break;
	case blend_binearest:
		break;
	case blend_bilinear:
		break;
	case blend_bicubic:
		break;
	default:
		printf("Invalid blend function");
		break;
	}

	//Output pose
	//a3hierarchyPoseLerp(activeHS->objectSpace,	// use as temp storage
	//	demoMode->hierarchyPoseGroup_skel->hpose + demoMode->hierarchyKeyPose_display[0] + 1,
	//	demoMode->hierarchyPoseGroup_skel->hpose + demoMode->hierarchyKeyPose_display[1] + 1,
	//	demoMode->hierarchyKeyPose_param,
	//	demoMode->hierarchy_skel->numNodes);
	//a3hierarchyPoseConcat(activeHS->localSpace,	// goal to calculate
	//	baseHS->localSpace, // holds base pose
	//	activeHS->objectSpace, // temp storage
	//	demoMode->hierarchy_skel->numNodes);
	a3hierarchyPoseConvert(activeHS->localSpace,
		demoMode->hierarchy_skel->numNodes,
		demoMode->hierarchyPoseGroup_skel->channel,
		demoMode->hierarchyPoseGroup_skel->order);
	a3kinematicsSolveForward(activeHS);
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
			a3real4x4Concat(activeHS->objectSpace->pose[i].transform.m, scaleMat.m);
			a3real4x4Product(mvp_joint->m, mvp_obj.m, scaleMat.m);

			// bone transform
			p = demoMode->hierarchy_skel->nodes[i].parentIndex;
			if (p >= 0)
			{
				// position is parent joint's position
				scaleMat.v3 = activeHS->objectSpace->pose[p].transform.v3;

				// direction basis is from parent to current
				a3real3Diff(scaleMat.v2.v,
					activeHS->objectSpace->pose[i].transform.v3.v, scaleMat.v3.v);

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
			*t_skin = activeHS->objectSpaceBindToCurrent->pose[i].transform;

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
	}
	
}


//-----------------------------------------------------------------------------
