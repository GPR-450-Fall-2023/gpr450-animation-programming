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
	
	a3_XboxControllerInput.inl
	Basic Xbox controller input state tracking implementation.

	**DO NOT MODIFY THIS FILE**
*/


#if (defined _WINDOWS || defined _WIN32)
#ifdef __ANIMAL3D_XBOXCONTROLLERINPUT_H
#ifndef __ANIMAL3D_XBOXCONTROLLERINPUT_INL
#define __ANIMAL3D_XBOXCONTROLLERINPUT_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3XboxControlSetID(a3_XboxControllerInput *control, const a3_XboxControllerID id)
{
	if (control)
	{
		control->id = id;
		return a3XboxControlUpdate(control);
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlGetState(const a3_XboxControllerInput *control, const a3_XboxControllerButton button)
{
	if (control)
		return (control->ctrl.buttons & button);
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlIsHeld(const a3_XboxControllerInput *control, const a3_XboxControllerButton button)
{
	if (control)
		return ((control->ctrl.buttons & button) && (control->ctrl0.buttons & button));
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlIsChanged(const a3_XboxControllerInput *control, const a3_XboxControllerButton button)
{
	if (control)
		return ((control->ctrl.buttons & button) - (control->ctrl0.buttons & button));
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlIsPressed(const a3_XboxControllerInput *control, const a3_XboxControllerButton button)
{
	if (control)
		return ((control->ctrl.buttons & button) && !(control->ctrl0.buttons & button));
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlIsReleased(const a3_XboxControllerInput *control, const a3_XboxControllerButton button)
{
	if (control)
		return (!(control->ctrl.buttons & button) && (control->ctrl0.buttons & button));
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlGetLeftTrigger(const a3_XboxControllerInput *control, a3f64 *trigger_out)
{
	if (control && trigger_out)
	{
		*trigger_out = control->ctrl.lTrigger_unit;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlGetRightTrigger(const a3_XboxControllerInput *control, a3f64 *trigger_out)
{
	if (control && trigger_out)
	{
		*trigger_out = control->ctrl.rTrigger_unit;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlGetTriggers(const a3_XboxControllerInput *control, a3f64 *leftTrigger_out, a3f64 *rightTrigger_out)
{
	if (control && leftTrigger_out && rightTrigger_out)
	{
		*leftTrigger_out = control->ctrl.lTrigger_unit;
		*rightTrigger_out = control->ctrl.rTrigger_unit;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlGetLeftJoystick(const a3_XboxControllerInput *control, a3f64 *joystick_out)
{
	if (control && joystick_out)
	{
		joystick_out[0] = control->ctrl.lThumbX_unit;
		joystick_out[1] = control->ctrl.lThumbY_unit;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlGetRightJoystick(const a3_XboxControllerInput *control, a3f64 *joystick_out)
{
	if (control && joystick_out)
	{
		joystick_out[0] = control->ctrl.rThumbX_unit;
		joystick_out[1] = control->ctrl.rThumbY_unit;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlGetJoysticks(const a3_XboxControllerInput *control, a3f64 *leftJoystick_out, a3f64 *rightJoystick_out)
{
	if (control && leftJoystick_out && rightJoystick_out)
	{
		leftJoystick_out[0] = control->ctrl.lThumbX_unit;
		leftJoystick_out[1] = control->ctrl.lThumbY_unit;
		rightJoystick_out[0] = control->ctrl.rThumbX_unit;
		rightJoystick_out[1] = control->ctrl.rThumbY_unit;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlIsConnected(const a3_XboxControllerInput *control)
{
	if (control)
		return (control->connected);
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlConnectionChanged(const a3_XboxControllerInput *control)
{
	if (control)
		return (control->connected - control->connected0);
	return a3input_invalid;
}

A3_INLINE a3ret a3XboxControlReset(a3_XboxControllerInput *control)
{
	static const a3_XboxControllerInput reset = { 0 };

	if (control)
	{
		control->ctrl = reset.ctrl;
		control->connected = 0;
		*(a3i32 *)control->rumble = 0;
		return 1;
	}
	return a3input_invalid;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_XBOXCONTROLLERINPUT_INL
#endif	// __ANIMAL3D_XBOXCONTROLLERINPUT_H
#endif	// (defined _WINDOWS || defined _WIN32)