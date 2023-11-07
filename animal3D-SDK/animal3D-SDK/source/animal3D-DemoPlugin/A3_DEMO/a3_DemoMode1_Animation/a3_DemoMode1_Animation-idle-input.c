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
		//a3demoCtrlCasesLoop(demoMode->render, animation_render_max, 'k', 'j');

		// toggle display program
		//a3demoCtrlCasesLoop(demoMode->display, animation_display_max, 'K', 'J');

		// toggle active camera
		//a3demoCtrlCasesLoop(demoMode->activeCamera, animation_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3demoCtrlCasesLoop(demoMode->pipeline, animation_pipeline_max, ']', '[');

		// toggle target
		a3demoCtrlCasesLoop(demoMode->targetIndex[demoMode->pass], demoMode->targetCount[demoMode->pass], '}', '{');

		// toggle pass to display
		a3demoCtrlCasesLoop(demoMode->pass, animation_pass_max, ')', '(');

		// toggle control target
		a3demoCtrlCasesLoop(demoMode->ctrl_target, animation_ctrlmode_max, '\'', ';');

		// toggle position input mode
		a3demoCtrlCasesLoop(demoMode->ctrl_position, animation_inputmode_max, '=', '-');

		// toggle rotation input mode
		a3demoCtrlCasesLoop(demoMode->ctrl_rotation, animation_inputmode_max, '+', '_');
	}
}

void a3animation_input_keyCharHold(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode, a3i32 const asciiKey, a3i32 const state)
{
//	switch (asciiKey)
//	{
//
//	}
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

	
	// Get inputs and prep them for use
	a3vec2 posInput = { (a3real) (a3keyboardGetState(demoState->keyboard, a3key_D) - a3keyboardGetState(demoState->keyboard, a3key_A)) * MOVE_MAGNITUDE,
						(a3real) (a3keyboardGetState(demoState->keyboard, a3key_W) - a3keyboardGetState(demoState->keyboard, a3key_S)) * MOVE_MAGNITUDE };

	a3real rotInput = (a3real) (a3keyboardGetState(demoState->keyboard, a3key_J) - a3keyboardGetState(demoState->keyboard, a3key_L));


	a3vec2 posResult = { demoMode->ctrlNode->translate.x, demoMode->ctrlNode->translate.y };
	a3real rotResult = { demoMode->ctrlNode->rotate.z };


	a3vec2 posIntegrateResult;
	a3real rotIntegrateResult;


	switch(demoMode->ctrl_position)
	{
	case animation_input_direct:
		posResult = (a3vec2) { posInput.x * MOVE_MAGNITUDE, posInput.y * MOVE_MAGNITUDE };
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




//-----------------------------------------------------------------------------

void a3demo_input_controlProjector(
	a3_DemoState* demoState, a3_DemoProjector* projector,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed, a3real ctrlZoomSpeed);

void a3animation_input(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	// TEST - Manipulating ctrlNode
	/*a3ret horizontal = a3keyboardGetState(demoState->keyboard, a3key_rightArrow) - a3keyboardGetState(demoState->keyboard, a3key_leftArrow);
	a3ret vertical = a3keyboardGetState(demoState->keyboard, a3key_upArrow) - a3keyboardGetState(demoState->keyboard, a3key_downArrow);

	a3spatialPoseSetTranslation(demoMode->ctrlNode, demoMode->ctrlNode->translate.x + horizontal, demoMode->ctrlNode->translate.y + vertical, demoMode->ctrlNode->translate.z);*/


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
	
	// choose control target
	switch (demoMode->ctrl_target)
	{
	case animation_ctrl_camera:
		// move camera
		a3demo_input_controlProjector(demoState, projector,
			dt, projector->ctrlMoveSpeed, projector->ctrlRotateSpeed, projector->ctrlZoomSpeed);
		break;
	case animation_ctrl_character:
		// capture axes
		if (a3XboxControlIsConnected(demoState->xcontrol))
		{
			// get directly from joysticks
			a3XboxControlGetJoysticks(demoState->xcontrol, demoMode->axis_l, demoMode->axis_r);
		}
		else
		{
			//Movement
			// calculate normalized vectors given keyboard state
			demoMode->axis_l[0] = (a3real)a3keyboardGetDifference(demoState->keyboard, a3key_D, a3key_A);
			demoMode->axis_l[1] = (a3real)a3keyboardGetDifference(demoState->keyboard, a3key_W, a3key_S);

			//Normalize if magnitude is not 0
			if (demoMode->axis_l[0] != 0 && demoMode->axis_l[1] != 0)
			{
				a3real shortAxis[2];
				shortAxis[0] = (a3real)demoMode->axis_l[0];
				shortAxis[1] = (a3real)demoMode->axis_l[1];
				a3real2Normalize(shortAxis);
				demoMode->axis_l[0] = (a3f64)shortAxis[0];
				demoMode->axis_l[1] = (a3f64)shortAxis[1];
			}

			//Rotation
			a3real pitch = -(a3real)a3mouseGetDeltaY(demoState->mouse);
			a3real yaw = -(a3real)a3mouseGetDeltaX(demoState->mouse);

			demoMode->axis_r[0] = yaw;
			demoMode->axis_r[1] = pitch;

			a3handleLocomotionInput(demoState, demoMode, dt);
		}
		break;
	}

	// allow the controller, if connected, to change control targets
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_right))
			a3demoCtrlIncLoop(demoMode->ctrl_target, animation_ctrlmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_left))
			a3demoCtrlDecLoop(demoMode->ctrl_target, animation_ctrlmode_max);

		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_B))
			a3demoCtrlIncLoop(demoMode->ctrl_position, animation_inputmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_X))
			a3demoCtrlDecLoop(demoMode->ctrl_position, animation_inputmode_max);

		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_Y))
			a3demoCtrlIncLoop(demoMode->ctrl_rotation, animation_inputmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_A))
			a3demoCtrlDecLoop(demoMode->ctrl_rotation, animation_inputmode_max);
	}
}