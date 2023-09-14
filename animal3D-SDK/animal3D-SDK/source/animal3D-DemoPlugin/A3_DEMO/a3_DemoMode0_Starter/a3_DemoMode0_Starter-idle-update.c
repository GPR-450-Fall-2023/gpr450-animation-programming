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
	
	a3_DemoMode0_Starter_idle-update.c
	Demo mode implementations: starter scene.

	********************************************
	*** UPDATE FOR STARTER SCENE MODE        ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode0_Starter.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update_objects(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox);
void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_DemoSceneObject* sceneObject, a3real4x4p s);

void a3starter_update(a3_DemoState* demoState, a3_DemoMode0_Starter* demoMode, a3f64 const dt)
{
	a3ui32 i;
	a3_DemoModelMatrixStack matrixStack[starterMaxCount_sceneObject];

	// active camera
	a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(demoState, dt,
		demoMode->object_scene, starterMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_camera, starterMaxCount_cameraObject, 1, 0);

	a3demo_updateProjectorViewProjectionMat(demoMode->proj_camera_main);

	a3demo_update_defaultAnimation(demoState, dt, demoMode->obj_box, 6, 2);

	// apply scales to starter objects
	for (i = 0; i < starterMaxCount_sceneObject; ++i)
		a3demo_applyScale_internal(demoMode->object_scene + i, scaleMat.m);

	// update skybox
	a3demo_update_bindSkybox(demoMode->obj_camera_main, demoMode->obj_skybox);

	// update matrix stack data
	for (i = 0; i < starterMaxCount_sceneObject; ++i)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m, activeCameraObject->modelMat.m, activeCameraObject->modelMatInv.m,
			demoMode->object_scene[i].modelMat.m, a3mat4_identity.m);
	}

	// select the active clip controller to make changes to the clip
	a3_ClipController* clipCtrl;
	switch (demoMode->currentController)
	{
	case 0:
		clipCtrl = &demoMode->clipCtrl1;
		break;
	case 1:
		clipCtrl = &demoMode->clipCtrl2;
		break;
	case 2:
		clipCtrl = &demoMode->clipCtrl3;
		break;
	default:
		clipCtrl = &demoMode->clipCtrl1;
		break;
	}

	// setting to start/end of clip
	if (demoMode->first == 1) {
		//call function to set to first keyframe of clip
		demoMode->first = 0;
	}
	if (demoMode->last == 1) {
		//call function to set to last keyframe of clip
		demoMode->last = 0;
	}

	// turning play/pause/slowmo/forward/backward into effect
	a3f32 playbackSpd = 1;
	if (demoMode->playPause == 1) {			//pause
		playbackSpd *= 0;
	}
	if (demoMode->forwardBackward == 1) {	//backward
		playbackSpd *= -1;
	}
	if (demoMode->slowmo == 1) {			//half speed
		playbackSpd *= 0.5;
	}
	else if (demoMode->slowmo == 2) {		//quarter speed
		playbackSpd *= 0.25;
	}
	clipCtrl->playbackDirection = playbackSpd;

	// put terminus action state into effect
	clipCtrl->terminusAction = demoMode->terminus;

	//UPDATE HERE - Dillon
	a3clipControllerUpdate(&demoMode->clipCtrl1, (a3real)dt);
}


//-----------------------------------------------------------------------------
