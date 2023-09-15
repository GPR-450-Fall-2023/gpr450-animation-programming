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

	a3_DemoMode0_Starter_idle-input.c
	Demo mode implementations: starter scene.

	********************************************
	*** INPUT FOR STARTER SCENE MODE         ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode0_Starter.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// CALLBACKS

// main demo mode callback
void a3starter_input_keyCharPress(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode, a3i32 const asciiKey, a3i32 const state)
{
	switch (asciiKey)
	{
		// toggle render program
		a3demoCtrlCasesLoop(demoMode->render, starter_render_max, 'k', 'j');

		// toggle display program
		a3demoCtrlCasesLoop(demoMode->display, starter_display_max, 'K', 'J');

		// toggle active camera
		a3demoCtrlCasesLoop(demoMode->activeCamera, starter_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3demoCtrlCasesLoop(demoMode->pipeline, starter_pipeline_max, ']', '[');

		// toggle target
		a3demoCtrlCasesLoop(demoMode->targetIndex[demoMode->pass], demoMode->targetCount[demoMode->pass], '}', '{');

		// toggle pass to display
		a3demoCtrlCasesLoop(demoMode->pass, starter_pass_max, ')', '(');

		// toggle clip controllers
		a3demoCtrlCasesLoop(demoMode->currentController, demoMode->numOfControllers, 'w', 'q');
		demoMode->terminus = demoMode->clipCtrlPool.clipControllers[demoMode->currentController].terminusAction;
		//demoMode->currentClip = demoMode->clipCtrlPool.clipControllers[demoMode->currentController].clip;

		// toggle pause
		a3demoCtrlCaseToggle(demoMode->togglePause, 'p');
		//a3demoCtrlCasesLoop(demoMode->playPause, starter_play_pause_max, 'p', 'o');

		// set to first/last frame in current clip
		a3demoCtrlCasesLoop(demoMode->first, starter_first_keyframe_max, 'e', 'd');
		a3demoCtrlCasesLoop(demoMode->last, starter_last_keyframe_max, 'r', 'f');

		// toggle clip
		a3demoCtrlCasesLoop(demoMode->clipCtrlPool.clipControllers[demoMode->currentController].clip, demoMode->numOfClips, 's', 'a');

		// toggle backwards
		a3demoCtrlCaseToggle(demoMode->shouldRewind, 'b');
		//a3demoCtrlCasesLoop(demoMode->forwardBackward, starter_playing_max, 'b', 'g');

		// toggle slowmo
		a3demoCtrlCaseToggle(demoMode->shouldSpeedUp, 'm');
		a3demoCtrlCaseToggle(demoMode->shouldSlowDown, 'n');
		//a3demoCtrlCasesLoop(demoMode->slowmo, starter_slowmo_speed_max, 'm', 'n');

		// toggle terminus actions
		a3demoCtrlCasesLoop(demoMode->terminus, starter_terminus_max, ',', '.');
	}
}

void a3starter_input_keyCharHold(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode, a3i32 const asciiKey, a3i32 const state)
{
//	switch (asciiKey)
//	{
//
//	}
}


//-----------------------------------------------------------------------------

void a3demo_input_controlProjector(
	a3_DemoState* demoState, a3_DemoProjector* projector,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed, a3real ctrlZoomSpeed);

void a3starter_input(a3_DemoState* demoState, a3_DemoMode0_Starter* demoMode, a3f64 const dt)
{
	a3_DemoProjector* projector = demoMode->projector + demoMode->activeCamera;

	// right click to ray pick
	if (a3mouseGetState(demoState->mouse, a3mouse_right) == a3input_down)
	{
		// get window coordinates
		a3i32 const x = a3mouseGetX(demoState->mouse) + demoState->frameBorder;
		a3i32 const y = a3mouseGetY(demoState->mouse) + demoState->frameBorder;

		// transform to NDC
		a3vec4 coord = a3vec4_one;
		coord.x = +((a3real)x * demoState->frameWidthInv * a3real_two - a3real_one);
		coord.y = -((a3real)y * demoState->frameHeightInv * a3real_two - a3real_one);
		coord.z = -a3real_one;

		// transform to view space
		a3real4Real4x4Mul(projector->projectionMatInv.m, coord.v);
		a3real4DivS(coord.v, coord.w);

		// transform to world space
		a3real4Real4x4Mul(projector->sceneObject->modelMat.m, coord.v);
	}
	
	// move camera
	a3demo_input_controlProjector(demoState, projector,
		dt, projector->ctrlMoveSpeed, projector->ctrlRotateSpeed, projector->ctrlZoomSpeed);
}


//-----------------------------------------------------------------------------
