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

	a3_DemoMode0_Starter.h
	Demo mode interface: starter scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOMODE0_STARTER_H
#define __ANIMAL3D_DEMOMODE0_STARTER_H


//-----------------------------------------------------------------------------

#include "_a3_demo_utilities/a3_DemoSceneObject.h"

#include "_animation/a3_KeyframeAnimationController.h"

//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_DemoMode0_Starter							a3_DemoMode0_Starter;
typedef enum a3_DemoMode0_Starter_RenderProgramName			a3_DemoMode0_Starter_RenderProgramName;
typedef enum a3_DemoMode0_Starter_DisplayProgramName		a3_DemoMode0_Starter_DisplayProgramName;
typedef enum a3_DemoMode0_Starter_ActiveCameraName			a3_DemoMode0_Starter_ActiveCameraName;
typedef enum a3_DemoMode0_Starter_PipelineName				a3_DemoMode0_Starter_PipelineName;
typedef enum a3_DemoMode0_Starter_PassName					a3_DemoMode0_Starter_PassName;
typedef enum a3_DemoMode0_Starter_TargetName				a3_DemoMode0_Starter_TargetName;

typedef enum a3_DemoMode0_Starter_Play_Pause				a3_DemoMode0_Starter_Play_Pause;
typedef enum a3_DemoMode0_Starter_Playing_Direction			a3_DemoMode0_Starter_Playing_Direction;
typedef enum a3_DemoMode0_Starter_Slowmo_Speed				a3_DemoMode0_Starter_Slowmo_Speed;
typedef enum a3_DemoMode0_Starter_Terminus_Action			a3_DemoMode0_Starter_Terminus_Action;
typedef enum a3_DemoMode0_Starter_First_Keyframe			a3_DemoMode0_Starter_First_Keyframe;
typedef enum a3_DemoMode0_Starter_Last_Keyframe				a3_DemoMode0_Starter_Last_Keyframe;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// maximum unique objects
	enum a3_DemoMode0_Starter_ObjectMaxCount
	{
		starterMaxCount_sceneObject = 8,
		starterMaxCount_cameraObject = 1,
		starterMaxCount_projector = 1,
	};

	// scene object rendering program names
	enum a3_DemoMode0_Starter_RenderProgramName
	{
		starter_renderSolid,			// solid color
		starter_renderTexture,			// textured

		starter_render_max
	};

	// final display modes
	enum a3_DemoMode0_Starter_DisplayProgramName
	{
		starter_displayTexture,			// display simple texture

		starter_display_max
	};

	// active camera names
	enum a3_DemoMode0_Starter_ActiveCameraName
	{
		starter_cameraSceneViewer,		// scene viewing camera

		starter_camera_max
	};

	// pipeline names
	enum a3_DemoMode0_Starter_PipelineName
	{
		starter_forward,				// forward lighting pipeline

		starter_pipeline_max
	};

	// render passes
	enum a3_DemoMode0_Starter_PassName
	{
		starter_passScene,				// render scene objects
		starter_passComposite,			// composite layers

		starter_pass_max
	};

	// render target names
	enum a3_DemoMode0_Starter_TargetName
	{
		starter_scene_finalcolor = 0,	// final display color
		starter_scene_fragdepth,		// fragment depth
	
		starter_target_scene_max,
	};

	// play/pause states
	enum a3_DemoMode0_Starter_Play_Pause
	{
		starter_play = 0,				// playing
		starter_pause,					// paused

		starter_play_pause_max,
	};

	// start of clip
	enum a3_DemoMode0_Starter_First_Keyframe
	{
		starter_normal_playback_f = 0,	// play normally
		starter_first_keyframe,			// set to first keyframe of clip

		starter_first_keyframe_max,
	};

	// end of clip
	enum a3_DemoMode0_Starter_Last_Keyframe
	{
		starter_normal_playback_l = 0,	// play normally
		starter_last_keyframe,			// set to last keyframe of clip

		starter_last_keyframe_max,
	};

	// playback directions
	enum a3_DemoMode0_Starter_Playing_Direction
	{
		starter_playing_forward = 0,	// playing forward
		starter_playing_backward,		// playing backward

		starter_playing_max,
	};

	// slowmo speeds
	enum a3_DemoMode0_Starter_Slowmo_Speed
	{
		starter_speed_normal = 0,		// 1x speed
		starter_speed_half,				// 0.5x speed
		starter_speed_quarter,			// 0.25x speed

		starter_slowmo_speed_max,
	};

	// terminus actions
	enum a3_DemoMode0_Starter_Terminus_Action
	{
		starter_terminus_stop = 0,		// stop when clip ends
		starter_terminus_loop,			// repeat when clip ends
		starter_terminus_ping_pong,		// rewind when clip ends then repeat

		starter_terminus_max,
	};


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	struct a3_DemoMode0_Starter
	{
		a3_DemoMode0_Starter_RenderProgramName render;
		a3_DemoMode0_Starter_DisplayProgramName display;
		a3_DemoMode0_Starter_ActiveCameraName activeCamera;

		a3_DemoMode0_Starter_PipelineName pipeline;
		a3_DemoMode0_Starter_PassName pass;
		a3_DemoMode0_Starter_TargetName targetIndex[starter_pass_max], targetCount[starter_pass_max];

		a3_DemoMode0_Starter_Play_Pause playPause;
		a3_DemoMode0_Starter_Playing_Direction forwardBackward;
		a3_DemoMode0_Starter_Slowmo_Speed slowmo;
		a3_DemoMode0_Starter_Terminus_Action terminus;
		a3_DemoMode0_Starter_First_Keyframe first;
		a3_DemoMode0_Starter_Last_Keyframe last;

		a3boolean shouldSpeedUp;
		a3boolean shouldSlowDown;
		a3boolean togglePause;
		a3boolean shouldRewind;

		// objects
		union {
			a3_DemoSceneObject object_scene[starterMaxCount_sceneObject];
			struct {
				a3_DemoSceneObject
					obj_skybox[1];
				a3_DemoSceneObject
					obj_plane[1],
					obj_box[1],
					obj_sphere[1],
					obj_cylinder[1],
					obj_capsule[1],
					obj_torus[1];
				a3_DemoSceneObject
					obj_teapot[1];
			};
		};
		union {
			a3_DemoSceneObject object_camera[starterMaxCount_cameraObject];
			struct {
				a3_DemoSceneObject
					obj_camera_main[1];
			};
		};
		union {
			a3_DemoProjector projector[starterMaxCount_projector];
			struct {
				a3_DemoProjector
					proj_camera_main[1];
			};
		};

		/////// DATA HERE - Dillon /////////

		/*a3_ClipController clipCtrl1;
		a3_ClipController clipCtrl2;
		a3_ClipController clipCtrl3;*/
		a3_ClipControllerPool clipCtrlPool;
		a3ui32 currentController;
		a3ui32 numOfControllers;
		a3_ClipPool clipPool;
		a3_KeyframePool keyPool;
		a3ui32 currentClip;
		a3ui32 numOfKeyframes;
		a3ui32 numOfClips;
	};

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOMODE0_STARTER_H