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


	// skeletal
	if (demoState->updateAnimation)
	{
		a3real const dtr = (a3real)dt;
		a3_ClipController* clipCtrl = demoMode->clipCtrlA;

		// update controllers
		a3clipControllerUpdate(demoMode->clipCtrl, dt);
		a3clipControllerUpdate(demoMode->clipCtrlA, dt);
		a3clipControllerUpdate(demoMode->clipCtrlB, dt);

		// STEP
	//	a3hierarchyPoseCopy(activeHS->animPose,
	//		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipCtrl->keyframeIndex,
	//		demoMode->hierarchy_skel->numNodes);

		// LERP
		a3hierarchyPoseLerp(activeHS->animPose,
			demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex0,
			demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex1,
			(a3f32)clipCtrl->keyframeParam, demoMode->hierarchy_skel->numNodes);

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
		demoMode->obj_skeleton_ctrl->position.x = +(demoMode->pos.x);
		demoMode->obj_skeleton_ctrl->position.y = +(demoMode->pos.y);
		demoMode->obj_skeleton_ctrl->euler.z = -a3trigValid_sind(demoMode->rot);
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
	}
}


//-----------------------------------------------------------------------------
