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

	a3_DemoMode0_Starter_initialize.c
	Demo mode implementations: starter scene.

	********************************************
	*** LOADING FOR STARTER SCENE MODE       ***
	********************************************
*/

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_DemoMode0_Starter-load.c
	Demo mode implementations: keyframe and clip controller

	LOADING FOR KEYFRAME AND CLIP CONTROLLER
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode0_Starter.h"

#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3starter_input(a3_DemoState* demoState, a3_DemoMode0_Starter* demoMode, a3f64 const dt);
void a3starter_update(a3_DemoState* demoState, a3_DemoMode0_Starter* demoMode, a3f64 const dt);
void a3starter_render(a3_DemoState const* demoState, a3_DemoMode0_Starter const* demoMode, a3f64 const dt);
void a3starter_input_keyCharPress(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode, a3i32 const asciiKey, a3i32 const state);
void a3starter_input_keyCharHold(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode, a3i32 const asciiKey, a3i32 const state);

void a3starter_loadValidate(a3_DemoState* demoState, a3_DemoMode0_Starter* demoMode)
{
	// initialize callbacks
	a3_DemoModeCallbacks* const callbacks = demoState->demoModeCallbacks + demoState_modeStarter;
	callbacks->demoMode = demoMode;
	callbacks->handleInput =	(a3_DemoMode_EventCallback)		a3starter_input;
	callbacks->handleUpdate =	(a3_DemoMode_EventCallback)		a3starter_update;
	callbacks->handleRender =	(a3_DemoMode_EventCallbackConst)a3starter_render;
	callbacks->handleKeyPress = (a3_DemoMode_InputCallback)		a3starter_input_keyCharPress;
	callbacks->handleKeyHold =	(a3_DemoMode_InputCallback)		a3starter_input_keyCharHold;

	// initialize cameras dependent on viewport
	demoMode->proj_camera_main->aspect = demoState->frameAspect;
	a3demo_updateProjectorProjectionMat(demoMode->proj_camera_main);
	a3demo_setProjectorSceneObject(demoMode->proj_camera_main, demoMode->obj_camera_main);

	// initialize cameras not dependent on viewport
}


void a3starter_load(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode)
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
	for (i = 0; i < starterMaxCount_sceneObject; ++i)
		a3demo_initSceneObject(demoMode->object_scene + i);
	for (i = 0; i < starterMaxCount_cameraObject; ++i)
		a3demo_initSceneObject(demoMode->object_camera + i);
	for (i = 0; i < starterMaxCount_projector; ++i)
		a3demo_initProjector(demoMode->projector + i);

	currentSceneObject = demoMode->obj_skybox;
	currentSceneObject->scaleMode = 1;
	currentSceneObject->scale.x = 256.0f;

	currentSceneObject = demoMode->obj_plane;
	currentSceneObject->scaleMode = 2;
	currentSceneObject->scale.x = 24.0f;
	currentSceneObject->scale.y = 24.0f;
	currentSceneObject->scale.z = 0.001953125;
	currentSceneObject->position.z = -sceneObjectHeight;

	currentSceneObject = demoMode->obj_box;
	currentSceneObject->scaleMode = 2;
	currentSceneObject->scale.x = 5.0f;
	currentSceneObject->scale.y = 4.0f;
	currentSceneObject->scale.z = 3.0f;
	a3real3Set(currentSceneObject->position.v, +1.0f * sceneObjectDistance, 0.0f, sceneObjectHeight);

	currentSceneObject = demoMode->obj_sphere;
	currentSceneObject->scaleMode = 1;
	currentSceneObject->scale.x = 2.0f;
	a3real3Set(currentSceneObject->position.v, -1.0f * sceneObjectDistance, 0.0f, sceneObjectHeight);

	currentSceneObject = demoMode->obj_cylinder;
	currentSceneObject->scaleMode = 2;
	currentSceneObject->scale.x = 5.0f;
	currentSceneObject->scale.y = 2.0f;
	currentSceneObject->scale.z = 2.0f;
	a3real3Set(currentSceneObject->position.v, +0.5f * sceneObjectDistance, +0.866f * sceneObjectDistance, sceneObjectHeight);

	currentSceneObject = demoMode->obj_capsule;
	currentSceneObject->scaleMode = 1;
	currentSceneObject->scale.x = 2.0f;
	a3real3Set(currentSceneObject->position.v, -0.5f * sceneObjectDistance, -0.866f * sceneObjectDistance, sceneObjectHeight);

	currentSceneObject = demoMode->obj_torus;
	currentSceneObject->scaleMode = 1;
	currentSceneObject->scale.x = 2.5f;
	a3real3Set(currentSceneObject->position.v, -0.5f * sceneObjectDistance, +0.866f * sceneObjectDistance, sceneObjectHeight);

	currentSceneObject = demoMode->obj_teapot;
	currentSceneObject->scaleMode = 0;
	a3real3Set(currentSceneObject->position.v, +0.5f * sceneObjectDistance, -0.866f * sceneObjectDistance, sceneObjectHeight);


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
	demoMode->render = starter_renderTexture;
	demoMode->display = starter_displayTexture;
	demoMode->activeCamera = starter_cameraSceneViewer;

	demoMode->pipeline = starter_forward;
	demoMode->pass = starter_passComposite;

	demoMode->targetIndex[starter_passScene] = starter_scene_finalcolor;
	demoMode->targetIndex[starter_passComposite] = starter_scene_finalcolor;

	demoMode->targetCount[starter_passScene] = starter_target_scene_max;
	demoMode->targetCount[starter_passComposite] = 1;

	// SETUP HERE - Dillon

	// Creating Pool of Keyframes
	demoMode->numOfKeyframes = 20;
	a3keyframePoolCreate(&demoMode->keyPool, demoMode->numOfKeyframes);

	// Creating Pool of Clips
	demoMode->numOfClips = 5;
	demoMode->currentClip = 0;
	a3clipPoolCreate(&demoMode->clipPool, &demoMode->keyPool, demoMode->numOfClips);

	// Initializing Keyframes UNCOMMENT THIS BEFORE SAYING DONE
	for (a3ui32 i = 0; i < demoMode->numOfKeyframes; i++) {
		a3keyframeInit(&demoMode->keyPool.keyframe[i], 0.5f, (a3real)i + 1);
	}

	//demoMode->keyPool.keyframe[0].data = 0;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[0], .41f);
	//demoMode->keyPool.keyframe[1].data = 1;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[1], .01f); //Tests skipping of keyframes
	//demoMode->keyPool.keyframe[2].data = 2;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[2], .5);
	//demoMode->keyPool.keyframe[3].data = 3;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[3], .5);

	//a3clipCalculateDuration(&demoMode->clipPool.clip[0]);
	//////////

	// Initializing Clips
	a3clipInit(&demoMode->clipPool.clip[0], "Clip " + (1), &demoMode->clipPool, &demoMode->keyPool, 1, 6);
	a3clipInit(&demoMode->clipPool.clip[1], "Clip " + (2), &demoMode->clipPool, &demoMode->keyPool, 7, 14);
	a3clipInit(&demoMode->clipPool.clip[2], "Clip " + (3), &demoMode->clipPool, &demoMode->keyPool, 8, 14);
	a3clipInit(&demoMode->clipPool.clip[3], "Clip " + (4), &demoMode->clipPool, &demoMode->keyPool, 12, 18);
	a3clipInit(&demoMode->clipPool.clip[4], "Clip " + (5), &demoMode->clipPool, &demoMode->keyPool, 16, 20);
	// I didnt put this in a for loop because it was getting annoying
	// to think of a mathematical function to make the clips share keyframes
	// but also not go out of bounds, so i just did it manually
	// we can change this later

	//Test assingmnet of clip 0 transition indices pointing to the next clip in the pool
	demoMode->clipPool.clip[0].forwardTransition.index = 1;
	demoMode->clipPool.clip[0].backwardTransition.index = 1;
	
	// Initializing Clip Controllers
	a3clipControllerPoolCreate(&demoMode->clipCtrlPool, &demoMode->clipPool, 3);
	a3clipControllerInit(&demoMode->clipCtrlPool.clipControllers[0], "Clip Controller 1", &demoMode->clipPool, 0);
	a3clipControllerInit(&demoMode->clipCtrlPool.clipControllers[1], "Clip Controller 2", &demoMode->clipPool, 2);
	a3clipControllerInit(&demoMode->clipCtrlPool.clipControllers[2], "Clip Controller 3", &demoMode->clipPool, 2);
	demoMode->currentController = 0;
	demoMode->numOfControllers = 3;

	//Reset settings
	demoMode->shouldSpeedUp = false;
	demoMode->shouldSlowDown = false;
	demoMode->togglePause = false;
	demoMode->shouldRewind = false;

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


//-----------------------------------------------------------------------------
