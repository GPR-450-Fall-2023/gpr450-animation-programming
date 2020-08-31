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
}


//-----------------------------------------------------------------------------
