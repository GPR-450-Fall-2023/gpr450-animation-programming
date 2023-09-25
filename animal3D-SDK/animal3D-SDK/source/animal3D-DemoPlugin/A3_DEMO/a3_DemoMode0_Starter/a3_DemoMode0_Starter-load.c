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

const a3byte filePath[1024] = "../../../../resource/animdata/sprite_anim.txt";
a3ui32 a3countClips(const a3byte filePath[1024]);
a3ui32 a3readClipPoolFromFile(a3_ClipPool* clipPool, a3_KeyframePool* keyframePool, const a3byte filePath[1024]);

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
	demoMode->numOfKeyframes = 100;
	a3keyframePoolCreate(&demoMode->keyPool, demoMode->numOfKeyframes);

	// Creating Pool of Clips
	//demoMode->numOfClips = a3countClips(filePath);
	demoMode->numOfClips = 12;
	demoMode->currentClip = 0;
	a3clipPoolCreate(&demoMode->clipPool, &demoMode->keyPool, demoMode->numOfClips);

	// Initializing Keyframes
	for (a3ui32 i = 0; i < demoMode->numOfKeyframes; i++) {
		a3real val = i / 3.0f;
		a3real3 newData = { (a3real)val, (a3real)val, (a3real)val };
		a3keyframeInit(&demoMode->keyPool.keyframe[i], 0.5f, newData);
	}

	//a3readClipPoolFromFile(&demoMode->clipPool, &demoMode->keyPool, filePath);

	//demoMode->keyPool.keyframe[0].data = 0;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[0], .41f);
	//demoMode->keyPool.keyframe[1].data = 1;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[1], .01f); //Tests skipping of keyframes
	//demoMode->keyPool.keyframe[2].data = 2;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[2], .5);
	//demoMode->keyPool.keyframe[3].data = 3;
	//a3keyframeSetDuration(&demoMode->keyPool.keyframe[3], .5);

	demoMode->keyPool.keyframe[0].data[0] = 0;
	demoMode->keyPool.keyframe[0].data[1] = 0;
	demoMode->keyPool.keyframe[0].data[2] = 0;

	demoMode->keyPool.keyframe[1].data[0] = 3;
	demoMode->keyPool.keyframe[1].data[1] = 0;
	demoMode->keyPool.keyframe[1].data[2] = 0;

	demoMode->keyPool.keyframe[2].data[0] = 3;
	demoMode->keyPool.keyframe[2].data[1] = 3;
	demoMode->keyPool.keyframe[2].data[2] = 0;

	demoMode->keyPool.keyframe[3].data[0] = 3;
	demoMode->keyPool.keyframe[3].data[1] = 3;
	demoMode->keyPool.keyframe[3].data[2] = 3;

	demoMode->keyPool.keyframe[4].data[0] = 3;
	demoMode->keyPool.keyframe[4].data[1] = 0;
	demoMode->keyPool.keyframe[4].data[2] = 3;

	demoMode->keyPool.keyframe[5].data[0] = 0;
	demoMode->keyPool.keyframe[5].data[1] = 0;
	demoMode->keyPool.keyframe[5].data[2] = 3;

	demoMode->keyPool.keyframe[6].data[0] = -50;
	demoMode->keyPool.keyframe[6].data[1] = -50;
	demoMode->keyPool.keyframe[6].data[2] = -50;

	demoMode->keyPool.keyframe[7].data[0] = -50;
	demoMode->keyPool.keyframe[7].data[1] = -50;
	demoMode->keyPool.keyframe[7].data[2] = -50;

	//a3clipCalculateDuration(&demoMode->clipPool.clip[0]);
	//////////

	// Initializing Clips
	a3clipInit(&demoMode->clipPool.clip[0], "Clip " + (1), &demoMode->clipPool, &demoMode->keyPool, 0, 5);
	a3clipInit(&demoMode->clipPool.clip[1], "Clip " + (2), &demoMode->clipPool, &demoMode->keyPool, 8, 14);
	a3clipInit(&demoMode->clipPool.clip[2], "Clip " + (3), &demoMode->clipPool, &demoMode->keyPool, 8, 14);
	a3clipInit(&demoMode->clipPool.clip[3], "Clip " + (4), &demoMode->clipPool, &demoMode->keyPool, 12, 18);
	a3clipInit(&demoMode->clipPool.clip[4], "Clip " + (5), &demoMode->clipPool, &demoMode->keyPool, 16, 20);
	// I didnt put this in a for loop because it was getting annoying
	// to think of a mathematical function to make the clips share keyframes
	// but also not go out of bounds, so i just did it manually
	// we can change this later

	//Test assingmnet of clip 0 transition indices pointing to the next clip in the pool
	demoMode->clipPool.clip[0].forwardTransition.index = 1;
	demoMode->clipPool.clip[0].forwardTransition.getNextKeyframe = a3getNextKeyframeSkipFromNextClip;
	demoMode->clipPool.clip[0].forwardTransition.transitionFunction = a3terminusForwardSkipPlayback;
	demoMode->clipPool.clip[0].backwardTransition.index = 1;
	demoMode->clipPool.clip[0].backwardTransition.getNextKeyframe = a3getNextKeyframeSkipFromNextClip;
	demoMode->clipPool.clip[0].backwardTransition.transitionFunction = a3terminusReverseSkipPlayback;
	
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

a3ui32 a3countClips(const a3byte filePath[1024]) {
	FILE* fptr = fopen(filePath, "r");
	if (fptr == NULL) {
		printf("no such file.\n");
		return 0;
	}

	// File Format:
	// @ clip_name duration_s first_frame last_frame reverse_transition forward_transition comments(ignored)

	a3ui32 lineCount = 0;
	char lineStarter[5];
	while (fscanf(fptr, "%4s", lineStarter) == 1) {
		if (lineStarter == "@") {
			lineCount++;
			printf(lineStarter);
		}
	};

	fclose(fptr);
	return lineCount;
}

a3ui32 a3readClipPoolFromFile(a3_ClipPool* clipPool, a3_KeyframePool* keyframePool, const a3byte filePath[1024]) {
	if (clipPool == NULL) {
		printf("no such clip pool.\n");
		return 0;
	}

	if (keyframePool == NULL) {
		printf("no such keyframe pool.\n");
		return 0;
	}
	
	FILE* fptr = fopen(filePath, "r");
	if (fptr == NULL) {
		printf("no such file.\n");
		return 0;
	}

	// File Format:
	// @ clip_name duration_s first_frame last_frame reverse_transition forward_transition comments(ignored)

	a3ui32 lineCount = 0;
	a3byte lineStarter[32], clip_name[32], duration_s[32],
		first_frame[32], last_frame[32],
		transition1[32], transition2[32], transition3[32], transition4[32];
	a3byte fileData[25][8][32];
	while (fscanf(fptr, "%32s %32s %32s %32s %32s %32s %32s %32s %32s", lineStarter, clip_name, duration_s, first_frame, last_frame, transition1, transition2, transition3, transition4) == 1) {
		if (lineStarter == "@") {
			printf("%s\n", clip_name);
			for (a3ui32 i = 0; i < 32; i++) {
				fileData[lineCount][0][i] = clip_name[i];
				fileData[lineCount][1][i] = duration_s[i];
				fileData[lineCount][2][i] = first_frame[i];
				fileData[lineCount][3][i] = last_frame[i];
				fileData[lineCount][4][i] = transition1[i];
				fileData[lineCount][5][i] = transition2[i];
				fileData[lineCount][6][i] = transition3[i];
				fileData[lineCount][7][i] = transition4[i];
			}
			lineCount++;
		}
	};

	for (a3ui32 i = 0; i < lineCount; i++) {
		a3clipInit(&clipPool->clip[i], fileData[i][0], clipPool, keyframePool, atoi(fileData[i][2]), atoi(fileData[i][3]));
		
		//setting clip duration to fileData[i][1]
		//clipPool->clip[lineCount].duration = atoi(fileData[i][1]);
		//what is a3real?

		a3byte prevOp[5];
		for (a3ui32 j = 0; j < 5; j++) {
			prevOp[i] = fileData[i][4][j];
		}
		a3byte nextOp[5];
		a3byte prevClip[32];
		a3byte nextClip[32];
		a3i32 prevIndex = -1;
		a3i32 nextIndex = -1;

		a3boolean specRev, specFor;
		if (fileData[lineCount][6] == "#") {	//if only 2 terms, then no clips are referenced
			specRev = false;
			specFor = false;
			for (a3ui32 j = 0; j < 5; j++) {
				nextOp[i] = fileData[i][5][j];
			}
		}
		else if (fileData[lineCount][7] == "#") {	//if 3 terms, then 1 clip is referenced
			if (fileData[i][5] == "<" || fileData[i][5] == ">" || fileData[i][5] == "|") {	//forward transition references the clip
				specRev = false;
				specFor = true;
				for (a3ui32 j = 0; j < 5; j++) {
					nextOp[i] = fileData[i][5][j];
				}
				for (a3ui32 j = 0; j < 32; j++) {
					nextClip[j] = fileData[i][6][j];
				}
			}
			else {	//reverse transition references the clip
				specRev = true;
				specFor = false;
				for (a3ui32 j = 0; j < 5; j++) {
					nextOp[i] = fileData[i][6][j];
				}
				for (a3ui32 j = 0; j < 32; j++) {
					prevClip[j] = fileData[i][5][j];
				}
			}
		}
		else {	//if 4 terms, then 2 clips are referenced
			specRev = true;
			specFor = true;
			for (a3ui32 j = 0; j < 5; j++) {
				nextOp[i] = fileData[i][6][j];
			}
			for (a3ui32 j = 0; j < 32; j++) {
				prevClip[j] = fileData[i][5][j];
				nextClip[j] = fileData[i][7][j];
			}
		}

		//find which index the previous clip transition has
		if (specRev) {
			for (a3ui32 j = 0; j < clipPool->count; j++) {
				if (clipPool->clip[j].name == prevClip) {
					prevIndex = j;
				}
			}
		}
		else {
			prevIndex = i;
		}

		//find which index the next clip transition has
		if (specFor) {
			for (a3ui32 j = 0; j < clipPool->count; j++) {
				if (clipPool->clip[j].name == nextClip) {
					nextIndex = j;
				}
			}
		}
		else {
			nextIndex = i;
		}
		
		if (prevIndex == -1) {
			printf("There is no clip referenced by the reverse transition\n");
			return 0;
		}
		if (nextIndex == -1) {
			printf("There is no clip referenced by the forward transition\n");
			return 0;
		}

		//a3clipTransitionInit(&clipPool->clip[i].backwardTransition, prevIndex, clipPool, DEFAULT_BACKWARD_TRANSITION, DEFAULT_NEXT_KEYFRAME);
		//a3clipTransitionInit(&clipPool->clip[i].forwardTransition, nextIndex, clipPool, DEFAULT_FORWARD_TRANSITION, DEFAULT_NEXT_KEYFRAME);
		
	}

	fclose(fptr);
	return 1;
}



//-----------------------------------------------------------------------------
