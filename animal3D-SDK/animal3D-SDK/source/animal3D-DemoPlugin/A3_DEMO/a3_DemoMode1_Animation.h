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

	a3_DemoMode1_Animation.h
	Demo mode interface: animation scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOMODE1_ANIMATION_H
#define __ANIMAL3D_DEMOMODE1_ANIMATION_H


//-----------------------------------------------------------------------------

#include "_a3_demo_utilities/a3_DemoSceneObject.h"

#include "_animation/a3_KeyframeAnimationController.h"
#include "_animation/a3_HierarchyStateBlend.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_DemoMode1_Animation						a3_DemoMode1_Animation;
typedef enum a3_DemoMode1_Animation_RenderProgramName		a3_DemoMode1_Animation_RenderProgramName;
typedef enum a3_DemoMode1_Animation_DisplayProgramName		a3_DemoMode1_Animation_DisplayProgramName;
typedef enum a3_DemoMode1_Animation_ActiveCameraName		a3_DemoMode1_Animation_ActiveCameraName;
typedef enum a3_DemoMode1_Animation_PipelineName			a3_DemoMode1_Animation_PipelineName;
typedef enum a3_DemoMode1_Animation_PassName				a3_DemoMode1_Animation_PassName;
typedef enum a3_DemoMode1_Animation_TargetName				a3_DemoMode1_Animation_TargetName;
typedef enum a3_DemoMode1_Animation_ControlTarget			a3_DemoMode1_Animation_ControlTarget;
typedef enum a3_DemoMode1_Animation_InputMode				a3_DemoMode1_Animation_InputMode;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// maximum unique objects
	enum a3_DemoMode1_Animation_ObjectMaxCount
	{
		animationMaxCount_sceneObject = 16,
		animationMaxCount_projector = 1,
		animationMaxCount_blendTrees = 10
	};

	// scene object rendering program names
	enum a3_DemoMode1_Animation_RenderProgramName
	{
		animation_renderSolid,			// solid color
		animation_renderTexture,		// textured
		animation_renderLambert,		// Lambert shading model
		animation_renderPhong,			// Phong shading model

		animation_render_max
	};

	// final display modes
	enum a3_DemoMode1_Animation_DisplayProgramName
	{
		animation_displayTexture,			// display simple texture

		animation_display_max
	};

	// active camera names
	enum a3_DemoMode1_Animation_ActiveCameraName
	{
		animation_cameraSceneViewer,		// scene viewing camera

		animation_camera_max
	};

	// pipeline names
	enum a3_DemoMode1_Animation_PipelineName
	{
		animation_forward,				// forward lighting pipeline

		animation_pipeline_max
	};

	// render passes
	enum a3_DemoMode1_Animation_PassName
	{
		animation_passScene,				// render scene objects
		animation_passComposite,			// composite layers

		animation_pass_max
	};

	// render target names
	enum a3_DemoMode1_Animation_TargetName
	{
		animation_scene_finalcolor = 0,	// final display color
		animation_scene_fragdepth,		// fragment depth
	
		animation_target_scene_max
	};

	// control targets
	enum a3_DemoMode1_Animation_ControlTarget
	{
		animation_ctrl_camera,
		animation_ctrl_character,
		animation_ctrl_neckLookat,
		animation_ctrl_wristEffector_r,
		animation_ctrl_wristConstraint_r,

		animation_ctrlmode_max
	};

	// input modes
	enum a3_DemoMode1_Animation_InputMode
	{
		animation_input_direct,
		animation_input_euler,
		animation_input_kinematic,
		animation_input_interpolate1,
		animation_input_interpolate2,

		animation_inputmode_max
	};


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	struct a3_DemoMode1_Animation
	{
		a3_DemoMode1_Animation_RenderProgramName render;
		a3_DemoMode1_Animation_DisplayProgramName display;
		a3_DemoMode1_Animation_ActiveCameraName activeCamera;

		a3_DemoMode1_Animation_PipelineName pipeline;
		a3_DemoMode1_Animation_PassName pass;
		a3_DemoMode1_Animation_TargetName targetIndex[animation_pass_max], targetCount[animation_pass_max];

		// scene graph
		a3_Hierarchy sceneGraph[1];
		a3_HierarchyState sceneGraphState[1];

		// animation controllers
		a3_ClipController clipCtrl[1], clipCtrlA[1], clipCtrlB[1];
		a3_ClipPool clipPool[1];

		// Blend tree
		a3_BlendTree blendTrees[animationMaxCount_blendTrees];

		// Moving these to inside blend tree so we can support multiple blend trees running at once
		/*a3_BlendNode* blendTreeNodes[64];
		a3ui32 blendNodeCount;*/

		// Blend clip controllers
		a3_ClipController idleClipCtrl[1], walkClipCtrl[1], runClipCtrl[1], jumpClipCtrl[1];

		a3boolean isJumping;
		a3real timeSinceJump;
		a3real jumpDuration;
		a3real jumpFadeInTime; // Ex. <.3, .3> would take .3 seconds to fully blend into jump, and then take .3 seconds to fully blend out of jump
		a3real jumpFadeOutTime;
		a3real jumpHeight;
		a3real jumpLerpParam; // How jump should be blending with ground movement;

		// skeletal animation
		union {
			a3_HierarchyState hierarchyState_skel[4];
			struct {
				a3_HierarchyState
					hierarchyState_skel_ik[1],
					hierarchyState_skel_fk[1],
					hierarchyState_skel_final[1],
					hierarchyState_skel_base[1];
			};
		};
		a3_Hierarchy hierarchy_skel[1];
		a3_HierarchyPoseGroup hierarchyPoseGroup_skel[1];
		a3mat4 mvp_joint[128], mvp_bone[128], t_skin[128];
		a3dualquat dq_skin[128];

		// control modes
		a3_DemoMode1_Animation_ControlTarget ctrl_target;
		a3_DemoMode1_Animation_InputMode ctrl_position, ctrl_rotation;

		// input axes & integration variables

		//Need to check if using keyboard for axis_r because input method changes from normalized stick to non-normalized mouse delta
		a3f64 axis_l[2], axis_r[2]; 
		a3vec2 pos, vel, acc;
		a3real rot, velr, accr;
		a3real pitch;
		a3vec2 pitchLimits;
		a3real xcontrolSensitivity;
		a3real mouseSensitivity;

		a3_SpatialPose* ctrlNode; // Input node that will update obj_skeleton_ctrl with values every frame
		a3vec2 ctrlVelocity;
		a3real ctrlAngularVelocity;
		a3real ctrlVelocityMagnitude;

		a3real idleBlendThreshold;
		a3real walkBlendThreshold;
		a3real runBlendThreshold;

		a3boolean ctrlInputsRegistered;

		// objects
		union {
			a3_DemoSceneObject object_scene[animationMaxCount_sceneObject];
			struct {
				a3_DemoSceneObject
					obj_world_root[1];
				a3_DemoSceneObject
					obj_camera_main[1];
				a3_DemoSceneObject
					obj_light_main[1];
				a3_DemoSceneObject
					obj_skybox[1];
				a3_DemoSceneObject
					obj_skeleton_ctrl[1];
				a3_DemoSceneObject
					obj_skeleton_neckLookat_ctrl[1],
					obj_skeleton_wristEffector_r_ctrl[1],
					obj_skeleton_wristConstraint_r_ctrl[1],
					obj_skeleton[1];
			};
		};
		union {
			a3_DemoProjector projector[animationMaxCount_projector];
			struct {
				a3_DemoProjector
					proj_camera_main[1];
			};
		};
	};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOMODE1_ANIMATION_H