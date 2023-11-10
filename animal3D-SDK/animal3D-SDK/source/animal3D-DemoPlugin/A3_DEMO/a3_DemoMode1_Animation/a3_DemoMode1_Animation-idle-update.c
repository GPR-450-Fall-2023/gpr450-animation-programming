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
void a3demo_update_objects(a3f64 const dt, a3_DemoSceneObject* sceneObjectBase,
	a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox);
void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_DemoSceneObject* sceneObject, a3real4x4p s);

void a3animation_update_graphics(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode,
	a3_DemoModelMatrixStack const* matrixStack, a3_HierarchyState const* activeHS)
{
	// active camera
	a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;
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
		a3real4x4SetScale(scaleMat.m, a3real_sixth);
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
				? a3vec3_z.v : a3vec3_y.v, scaleMat.v2.v), a3real_sixth);

			// up basis is cross of direction and right
			a3real3MulS(a3real3CrossUnit(scaleMat.v1.v,
				scaleMat.v2.v, scaleMat.v0.v), a3real_sixth);
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

void a3animation_update_fk(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup, const a3_RootMotionFlag rootFlag)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// FK pipeline
		//a3hierarchyPoseConcat(activeHS->localSpace,	// local: goal to calculate
		//	activeHS->animPose, // holds current sample pose
		//	baseHS->localSpace, // holds base pose (animPose is all identity poses)
		//	activeHS->hierarchy->numNodes);
		a3hierarchyPoseOpConcatenate(activeHS->localSpace,	// local: goal to calculate
			activeHS->hierarchy->numNodes,
			activeHS->animPose, // holds current sample pose
			(a3_HierarchyPose*)baseHS->localSpace); // holds base pose (animPose is all identity poses)
		/*a3hierarchyPoseConvert(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order,
			rootFlag);*/
		a3hierarchyPoseOpCONVERT(activeHS->localSpace, 
			activeHS->hierarchy->numNodes, 
			*poseGroup->channel,
			*poseGroup->order, 
			rootFlag);
		a3kinematicsSolveForward(activeHS);
	}
}

void a3animation_update_ik(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// IK pipeline
		// ****TO-DO: direct opposite of FK
		
		//a3kinematicsSolveInverse(activeHS);
		//
		//a3hierarchyPoseOpREVERT(activeHS->objectSpace,
		//	activeHS->hierarchy->numNodes,
		//	*poseGroup->channel,
		//	*poseGroup->order);
		//
		//a3hierarchyPoseOpDeconcatenate(activeHS->objectSpace,	// local: goal to calculate
		//	activeHS->hierarchy->numNodes,
		//	activeHS->animPose, // holds current sample pose
		//	(a3_HierarchyPose*)baseHS->localSpace); // holds base pose (animPose is all identity poses)
	}
}

void a3animation_update_skin(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS)
{
	if (activeHS->hierarchy == baseHS->hierarchy)
	{
		// FK pipeline extended for skinning and other applications
		a3hierarchyStateUpdateLocalInverse(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);
	}
}

void a3animation_update_applyEffectors(a3_DemoMode1_Animation* demoMode,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		a3_DemoSceneObject* sceneObject = demoMode->obj_skeleton;
		a3ui32 j = sceneObject->sceneGraphIndex;

		// need to properly transform joints to their parent frame and vice-versa
		a3mat4 const controlToSkeleton = demoMode->sceneGraphState->localSpaceInv->pose[j].transformMat;
		a3vec4 controlLocator_neckLookat, controlLocator_wristEffector, controlLocator_wristConstraint, controlLocator_wristBase;
		a3mat4 jointTransform_neck = a3mat4_identity, jointTransform_wrist = a3mat4_identity, jointTransform_elbow = a3mat4_identity, jointTransform_shoulder = a3mat4_identity;
		a3ui32 j_neck, j_wrist, j_elbow, j_shoulder;

		// NECK LOOK-AT
		{
			// look-at effector
			sceneObject = demoMode->obj_skeleton_neckLookat_ctrl;
			a3real4Real4x4Product(controlLocator_neckLookat.v, controlToSkeleton.m,
				demoMode->sceneGraphState->localSpace->pose[sceneObject->sceneGraphIndex].transformMat.v3.v);
			j = j_neck = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:Neck");
			jointTransform_neck = activeHS->objectSpace->pose[j].transformMat;

			// ****TO-DO: 
			// make "look-at" matrix
			// in this example, +Z is towards locator, +Y is up

			// ****TO-DO: 
			// reassign resolved transforms to OBJECT-SPACE matrices
			// resolve local and animation pose for affected joint
			//	(instead of doing IK for whole skeleton when only one joint has changed)

		}

		// RIGHT ARM REACH
		{
			// right wrist effector
			sceneObject = demoMode->obj_skeleton_wristEffector_r_ctrl;
			a3real4Real4x4Product(controlLocator_wristEffector.v, controlToSkeleton.m,
				demoMode->sceneGraphState->localSpace->pose[sceneObject->sceneGraphIndex].transformMat.v3.v);
			
			
			//right wrist
			j = j_wrist = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightHand");
			jointTransform_wrist = activeHS->objectSpace->pose[j].transformMat;

			// right wrist constraint
			sceneObject = demoMode->obj_skeleton_wristConstraint_r_ctrl;
			a3real4Real4x4Product(controlLocator_wristConstraint.v, controlToSkeleton.m,
				demoMode->sceneGraphState->localSpace->pose[sceneObject->sceneGraphIndex].transformMat.v3.v);

			//Right elbow
			j = j_elbow = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightForeArm");
			jointTransform_elbow = activeHS->objectSpace->pose[j].transformMat;

			// right wrist base (right shoulder)
			j = j_shoulder = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightArm");
			jointTransform_shoulder = activeHS->objectSpace->pose[j].transformMat;
			controlLocator_wristBase = jointTransform_shoulder.v3;

			// ****TO-DO: 
			// solve positions and orientations for joints
			// in this example, +X points away from child, +Y is normal
			// 1) check if solution exists
			//	-> get vector between base and end effector; if it extends max length, straighten limb
			//	-> position of end effector's target is at the minimum possible distance along this vector
			
			//Vector and distance between base and end effectors
			a3real3 baseToEnd;
			a3real3Diff(baseToEnd, controlLocator_wristEffector.xyz.v, controlLocator_wristBase.xyz.v);
			a3real endEffectorDist = a3real3Length(baseToEnd);

			//Vector between base and constraint
			a3real3 baseToConstraint;
			a3real3Diff(baseToConstraint, controlLocator_wristConstraint.xyz.v, controlLocator_wristBase.xyz.v);
			//a3real constraintEffectorDist = a3real3Length(baseToConstraint);

			/*a3real3 shoulderElbow;
			a3real3Diff(shoulderElbow, jointTransform_elbow.v3.xyz.v, jointTransform_shoulder.v3.xyz.v);
			a3real3 elbowWrist;
			a3real3Diff(elbowWrist, jointTransform_wrist.v3.xyz.v, jointTransform_elbow.v3.xyz.v);*/

			//a3real shoulderElbowDist = a3real3Distance(jointTransform_shoulder.v3.xyz.v, jointTransform_elbow.v3.xyz.v); //Length of shoulder-elbow bone
			//a3real elbowWristDist = a3real3Distance(jointTransform_elbow.v3.xyz.v, jointTransform_wrist.v3.xyz.v); //Length of elbow-wrist bone
			a3real shoulderElbowDist = a3real3Distance(baseHS->objectSpace->pose[j_shoulder].translate.v, baseHS->objectSpace->pose[j_elbow].translate.v); //Length of shoulder-elbow bone
			a3real elbowWristDist = a3real3Distance(baseHS->objectSpace->pose[j_elbow].translate.v, baseHS->objectSpace->pose[j_wrist].translate.v); //Length of elbow-wrist bone
			a3real chainLength = shoulderElbowDist + elbowWristDist; //Total chain length

			printf("Effector Dist: %f   Chain Length: %f   ", endEffectorDist, chainLength);
			if (endEffectorDist < chainLength)
			{
				printf("Solvable");
			}
			else //Arm should be straight out towards end effector, no solution
			{
				//Normalize base to end as direction
				a3real3 normalizedBaseToEnd;
				a3real3SetReal3(normalizedBaseToEnd, baseToEnd);
				a3real3Normalize(normalizedBaseToEnd);

				///////// Position //////////

				//Shoulder doesn't move

				//Get position of elbow along the straight line between the base and end effector
				//Then Translate into world space by adding shoulder world space position
				a3real3ProductS(jointTransform_elbow.v3.xyz.v, normalizedBaseToEnd, shoulderElbowDist);
				a3real3Sum(jointTransform_elbow.v3.xyz.v, jointTransform_elbow.v3.xyz.v, jointTransform_shoulder.v3.xyz.v); 
				
				//Get positions of wrist along the straight line between the base and end effector
				//Then Translate into world space by adding elbow world space position
				a3real3ProductS(jointTransform_wrist.v3.xyz.v, normalizedBaseToEnd, elbowWristDist);
				a3real3Sum(jointTransform_wrist.v3.xyz.v, jointTransform_wrist.v3.xyz.v, jointTransform_elbow.v3.xyz.v);
			}
			printf("\n");

			////////// Rotation //////////
			
			//N = baseToEnd x baseToConstraint
			a3real3 planeNormal;
			a3real3Cross(planeNormal, baseToEnd, baseToConstraint);
			a3real3Normalize(planeNormal);

			//T1 = normalize(S - W)
			a3real3 shoulderTangent;
			a3real3Diff(shoulderTangent, jointTransform_elbow.v3.xyz.v, jointTransform_shoulder.v3.xyz.v);
			a3real3Normalize(shoulderTangent);

			//B1 = T1 x N
			a3real3 shoulderBitangent;
			a3real3Cross(shoulderBitangent, shoulderTangent, planeNormal);
			a3real3Normalize(shoulderBitangent);

			//T2 = normalize(W - E)
			a3real3 elbowTangent;
			a3real3Diff(elbowTangent, jointTransform_wrist.v3.xyz.v, jointTransform_elbow.v3.xyz.v);
			a3real3Normalize(elbowTangent);

			//B2 = T2 x N
			a3real3 elbowBitangent;
			a3real3Cross(elbowBitangent, elbowTangent, planeNormal);
			a3real3Normalize(elbowTangent);

			//Set first three columns of shoulder worldspace transform matrix to basis vectors
			a3real4Set(jointTransform_shoulder.v0.v, shoulderTangent[0], shoulderTangent[1], shoulderTangent[2], 0);
			a3real4Set(jointTransform_shoulder.v1.v, shoulderBitangent[0], shoulderBitangent[1], shoulderBitangent[2], 0);
			a3real4Set(jointTransform_shoulder.v2.v, planeNormal[0], planeNormal[1], planeNormal[2], 0);
			//Position already set

			//Set first three columns of elbow worldspace transform matrix to basis vectors
			a3real4Set(jointTransform_elbow.v0.v, elbowTangent[0], elbowTangent[1], elbowTangent[2], 0);
			a3real4Set(jointTransform_elbow.v1.v, elbowBitangent[0], elbowBitangent[1], elbowBitangent[2], 0);
			a3real4Set(jointTransform_elbow.v2.v, planeNormal[0], planeNormal[1], planeNormal[2], 0);
			//Position already set

			// ****TO-DO: 
			// reassign resolved transforms to OBJECT-SPACE matrices
			// work from root to leaf too get correct transformations
			/*a3mat4 invShoulder;
			a3real4x4GetInverse(invShoulder.m, jointTransform_shoulder.m);

			a3real4x4Product(activeHS->localSpace->pose[j_shoulder].transformMat.m,
				invShoulder.m,
				activeHS->localSpace->pose[j_wrist].transformMat.m);

			a3mat4 invElbow;
			a3real4x4GetInverse(invElbow.m, jointTransform_elbow.m);

			a3real4x4Product(activeHS->localSpace->pose[j_elbow].transformMat.m,
				invElbow.m,
				activeHS->localSpace->pose[j_wrist].transformMat.m);*/

			activeHS->objectSpace->pose[j_shoulder].transformMat = jointTransform_shoulder;
			activeHS->objectSpace->pose[j_elbow].transformMat = jointTransform_elbow;
			activeHS->objectSpace->pose[j_wrist].transformMat = jointTransform_wrist;

			j = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightShoulder");
			a3kinematicsSolveInverseSingle(activeHS, j_shoulder, j);
			a3spatialPoseOpREVERT(&activeHS->objectSpace->pose[j_shoulder],
				*poseGroup->channel,
				*poseGroup->order);
			a3spatialPoseOpDeconcatenate(&activeHS->animPose->pose[j_shoulder],
				&activeHS->objectSpace->pose[j_shoulder],
				&baseHS->objectSpace->pose[j_shoulder]);

			a3kinematicsSolveInverseSingle(activeHS, j_elbow, j_shoulder);
			a3spatialPoseOpREVERT(&activeHS->objectSpace->pose[j_elbow],
				*poseGroup->channel,
				*poseGroup->order);
			a3spatialPoseOpDeconcatenate(&activeHS->animPose->pose[j_elbow],
				&activeHS->objectSpace->pose[j_elbow],
				&baseHS->objectSpace->pose[j_elbow]);

			a3kinematicsSolveInverseSingle(activeHS, j_wrist, j_elbow);
			a3spatialPoseOpREVERT(&activeHS->objectSpace->pose[j_wrist],
				*poseGroup->channel,
				*poseGroup->order);
			a3spatialPoseOpDeconcatenate(&activeHS->animPose->pose[j_wrist],
				&activeHS->objectSpace->pose[j_wrist],
				&baseHS->animPose->pose[j_wrist]);
		}
	}
}

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

void a3animation_update_animation(a3_DemoMode1_Animation* demoMode, a3f64 const dt,
	a3boolean const updateIK)
{
	a3_HierarchyState* activeHS_fk = demoMode->hierarchyState_skel_fk;
	a3_HierarchyState* activeHS_ik = demoMode->hierarchyState_skel_ik;
	a3_HierarchyState* activeHS = demoMode->hierarchyState_skel_final;
	a3_HierarchyState const* baseHS = demoMode->hierarchyState_skel_base;
	a3_HierarchyPoseGroup const* poseGroup = demoMode->hierarchyPoseGroup_skel;

	// switch controller to see different states
	// A is idle, arms down; B is skin test, arms out
	a3_ClipController* clipCtrl_fk = demoMode->clipCtrlA;
	a3ui32 sampleIndex0, sampleIndex1;

	// resolve FK state
	// update clip controller, keyframe lerp
	a3clipControllerUpdate(clipCtrl_fk, dt);
	sampleIndex0 = demoMode->clipPool->keyframe[clipCtrl_fk->keyframeIndex].sampleIndex0;
	sampleIndex1 = demoMode->clipPool->keyframe[clipCtrl_fk->keyframeIndex].sampleIndex1;
	a3hierarchyPoseLerp(activeHS_fk->animPose,
		poseGroup->hpose + sampleIndex0, poseGroup->hpose + sampleIndex1,
		(a3real)clipCtrl_fk->keyframeParam, activeHS_fk->hierarchy->numNodes);
	// run FK pipeline
	a3animation_update_fk(activeHS_fk, baseHS, poseGroup, 
		demoMode->clipPool->clip[clipCtrl_fk->clipIndex].rootMotion);

	// resolve IK state
	// copy FK to IK
	a3hierarchyPoseCopy(
		activeHS_ik->animPose,	// dst: IK anim
		activeHS_fk->animPose,	// src: FK anim
		//baseHS->animPose,	// src: base anim
		activeHS_ik->hierarchy->numNodes);
	// run FK
	a3animation_update_fk(activeHS_ik, baseHS, poseGroup,
		demoMode->clipPool->clip[clipCtrl_fk->clipIndex].rootMotion);
	if (updateIK)
	{
		// invert object-space
		a3hierarchyStateUpdateObjectInverse(activeHS_ik);
		// run solvers
		a3animation_update_applyEffectors(demoMode, activeHS_ik, baseHS, poseGroup);
		// run full IK pipeline (if not resolving with effectors)
		//a3animation_update_ik(activeHS_ik, baseHS, poseGroup);
	}

	// blend FK/IK to final
	// testing: copy source to final
	a3hierarchyPoseCopy(activeHS->animPose,	// dst: final anim
		//activeHS_fk->animPose,	// src: FK anim
		activeHS_ik->animPose,	// src: IK anim
		//baseHS->animPose,	// src: base anim (identity)
		activeHS->hierarchy->numNodes);

	//Spine look rotation
	{
		//a3vec3 rotateSpine = { demoMode->pitch, 0, 0 };
		////a3hierarchyPoseOpRotateBoneName(activeHS->animPose, activeHS->hierarchy, rotateSpine, "mixamorig:Spine"); //Rotate at one bone
		//a3hierarchyPoseOpRotateBoneRange(activeHS->animPose, activeHS->hierarchy, rotateSpine, "mixamorig:Spine", "mixamorig:Spine2"); //Blend rotation across 3 bones
	}

	// run FK pipeline (skinning optional)
	a3animation_update_fk(activeHS, baseHS, poseGroup,
		demoMode->clipPool->clip[clipCtrl_fk->clipIndex].rootMotion);
	a3animation_update_skin(activeHS, baseHS);
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

void a3animation_update_sceneGraph(a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	a3ui32 i;
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(dt, demoMode->object_scene, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(dt, demoMode->obj_camera_main, 1, 1, 0);

	a3demo_updateProjectorViewProjectionMat(demoMode->proj_camera_main);

	// apply scales to objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		a3demo_applyScale_internal(demoMode->object_scene + i, scaleMat.m);
	}

	// update skybox
	a3demo_update_bindSkybox(demoMode->obj_camera_main, demoMode->obj_skybox);

	for (i = 0; i < animationMaxCount_sceneObject; ++i)
		demoMode->sceneGraphState->localSpace->pose[i].transformMat = demoMode->object_scene[i].modelMat;
	a3kinematicsSolveForward(demoMode->sceneGraphState);
	a3hierarchyStateUpdateLocalInverse(demoMode->sceneGraphState);
	a3hierarchyStateUpdateObjectInverse(demoMode->sceneGraphState);
}

void a3animation_update(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	//a3_HierarchyState* activeHS = demoMode->hierarchyState_skel + 1, * baseHS = demoMode->hierarchyState_skel;

	////printf("Movement: (%f, %f)   Rotate: (%f, %f)\n", demoMode->axis_l[0], demoMode->axis_l[1], demoMode->axis_r[0], demoMode->axis_r[1]);

	//a3handleLocomotionInput(demoState, demoMode, dt);

	//// skeletal
	//if (demoState->updateAnimation)
	//{
	//	a3real const dtr = (a3real)dt;
	//	a3_ClipController* clipCtrl = demoMode->clipCtrlA;

	//	// update controllers
	//	a3clipControllerUpdate(demoMode->clipCtrl, dt);
	//	a3clipControllerUpdate(demoMode->clipCtrlA, dt);
	//	a3clipControllerUpdate(demoMode->clipCtrlB, dt);
	//	
	//	/*system("cls");
	//	printf("Clip Index: %i\nClip Param: %f\nClip Time: %f\nKeyframe Index: %i\nKeyframe Param: %f\nKeyframe Time: %f\nPlayback Speed: %f\n\n",
	//		demoMode->clipCtrlA->clipIndex,
	//		demoMode->clipCtrlA->clipParam,
	//		demoMode->clipCtrlA->clipTime_sec,
	//		demoMode->clipCtrlA->keyframeIndex,
	//		demoMode->clipCtrlA->keyframeParam,
	//		demoMode->clipCtrlA->keyframeTime_sec,
	//		demoMode->clipCtrlA->playback_sec);*/

	//	//////////////////// TESTING TRANSITION BRANCHING //////////////////////////
	//	//a3_Clip* currentClip = &demoMode->clipCtrlA->clipPool->clip[demoMode->clipCtrlA->clipIndex];

	//	////Null check
	//	//if (currentClip->transitionForward[0].clipTransitionBranch)
	//	//{
	//	//	//Call function pointer and pass in demoMode
	//	//	if ((*currentClip->transitionForward[0].clipTransitionBranch)(demoMode))
	//	//	{
	//	//		//If true, print
	//	//		//printf("Function pointer works on forward input");
	//	//	}
	//	//}
	//	/////////////////////////////////////////////////////////////////////////////

	//	// STEP
	////	a3hierarchyPoseCopy(activeHS->animPose,
	////		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipCtrl->keyframeIndex,
	////		demoMode->hierarchy_skel->numNodes);

	//	// LERP
	//	a3hierarchyPoseLerp(activeHS->animPose,
	//		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex0,
	//		demoMode->hierarchyPoseGroup_skel->hpose + demoMode->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex1,
	//		(a3f32)clipCtrl->keyframeParam, demoMode->hierarchy_skel->numNodes);
	//	
	//	a3vec3 rotateSpine = { demoMode->pitch, 0, 0 };
	//	//a3hierarchyPoseOpRotateBoneName(activeHS->animPose, activeHS->hierarchy, rotateSpine, "mixamorig:Spine");
	//	a3hierarchyPoseOpRotateBoneRange(activeHS->animPose, activeHS->hierarchy, rotateSpine, "mixamorig:Spine", "mixamorig:Spine2");

	//	// FK pipeline
	//	a3hierarchyPoseOpConcatenate(activeHS->localSpace,
	//		demoMode->hierarchy_skel->numNodes,
	//		baseHS->localSpace,
	//		activeHS->animPose);
	//	//a3hierarchyPoseConvert(activeHS->localSpace, demoMode->hierarchy_skel->numNodes, demoMode->hierarchyPoseGroup_skel->channel, demoMode->hierarchyPoseGroup_skel->order);
	//	a3hierarchyPoseOpCONVERT(activeHS->localSpace, demoMode->hierarchy_skel->numNodes, *demoMode->hierarchyPoseGroup_skel->channel, 
	//		*demoMode->hierarchyPoseGroup_skel->order, demoMode->clipPool->clip[clipCtrl->clipIndex].rootMotion);
	//	//a3kinematicsSolveForward(activeHS);
	//	//a3mat4 activeM4 = a3mat4_identity;
	//	a3hierarchyPoseOpFK(activeHS->objectSpace, activeHS->localSpace, activeHS->hierarchy->nodes, activeHS->hierarchy->numNodes);
	//	a3hierarchyStateUpdateObjectInverse(activeHS);
	//	a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);
	//}
	{
		a3ui32 i;
		a3_DemoModelMatrixStack matrixStack[animationMaxCount_sceneObject];

		// active camera
		a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
		a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

		// skeletal
		if (demoState->updateAnimation)
			a3animation_update_animation(demoMode, dt, 1);

		// update scene graph local transforms
		a3animation_update_sceneGraph(demoMode, dt);

		// update matrix stack data using scene graph
		for (i = 0; i < animationMaxCount_sceneObject; ++i)
		{
			a3demo_updateModelMatrixStack(matrixStack + i,
				activeCamera->projectionMat.m,
				demoMode->sceneGraphState->objectSpace->pose[demoMode->obj_camera_main->sceneGraphIndex].transformMat.m,
				demoMode->sceneGraphState->objectSpaceInv->pose[demoMode->obj_camera_main->sceneGraphIndex].transformMat.m,
				demoMode->sceneGraphState->objectSpace->pose[demoMode->object_scene[i].sceneGraphIndex].transformMat.m,
				a3mat4_identity.m);
		}

		// prepare and upload graphics data
		a3animation_update_graphics(demoState, demoMode, matrixStack, demoMode->hierarchyState_skel_final);

		// testing: reset IK effectors to lock them to FK result
		{
			//void a3animation_load_resetEffectors(a3_DemoMode1_Animation * demoMode,
			//	a3_HierarchyState * hierarchyState, a3_HierarchyPoseGroup const* poseGroup);
			//a3animation_load_resetEffectors(demoMode,
			//	demoMode->hierarchyState_skel_final, demoMode->hierarchyPoseGroup_skel);
		}

		// ****TO-DO:
		// process input

		a3handleLocomotionInput(demoState, demoMode, dt);

		// apply input
		//demoMode->obj_skeleton_ctrl->position.x = +(demoMode->pos.x);
		//demoMode->obj_skeleton_ctrl->position.y = +(demoMode->pos.y);
		//demoMode->obj_skeleton_ctrl->euler.z = -a3trigValid_sind(demoMode->rot);

		// Update obj_skeleton_ctrl with values from ctrlNode
		a3animation_updateSkeletonCtrl(demoMode);
	}
}


//-----------------------------------------------------------------------------
