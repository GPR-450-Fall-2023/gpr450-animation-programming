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

	a3_DemoMode1_Animation-idle-input.c
	Demo mode implementations: animation scene.

	********************************************
	*** INPUT FOR ANIMATION SCENE MODE       ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// CALLBACKS

// main demo mode callback
void a3animation_input_keyCharPress(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode, a3i32 const asciiKey, a3i32 const state)
{
	switch (asciiKey)
	{
		// toggle render program
		a3demoCtrlCasesLoop(demoMode->render, animation_render_max, 'k', 'j');

		// toggle display program
		a3demoCtrlCasesLoop(demoMode->display, animation_display_max, 'K', 'J');

		// toggle active camera
		a3demoCtrlCasesLoop(demoMode->activeCamera, animation_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3demoCtrlCasesLoop(demoMode->pipeline, animation_pipeline_max, ']', '[');

		// toggle target
		a3demoCtrlCasesLoop(demoMode->targetIndex[demoMode->pass], demoMode->targetCount[demoMode->pass], '}', '{');

		// toggle pass to display
		a3demoCtrlCasesLoop(demoMode->pass, animation_pass_max, ')', '(');
	}
}

void a3animation_input_keyCharHold(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode, a3i32 const asciiKey, a3i32 const state)
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

void a3animation_input(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
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
