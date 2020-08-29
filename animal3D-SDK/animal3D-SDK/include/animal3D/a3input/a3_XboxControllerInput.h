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
	
	a3_XboxControllerInput.h
	Basic Xbox controller input state tracking interface.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_XBOXCONTROLLERINPUT_H
#define __ANIMAL3D_XBOXCONTROLLERINPUT_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"
#include "a3_InputState.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_XboxControllerInput	a3_XboxControllerInput;
	typedef enum a3_XboxControllerButton	a3_XboxControllerButton;
	typedef enum a3_XboxControllerID		a3_XboxControllerID;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Xbox controller state tracker; a wrapper for XInput's structure.
	//	members ctrl, ctrl0: wrapper for Xinput state with button, trigger and 
	//		joystick state, along with normalized versions of the triggers and 
	//		joysticks (for external flexibility)
	//	member rumble: vibration/rumble motor states
	//	member id: controller/player number (0-3)
	//	members connected, connected0: current and previous connection state
	struct a3_XboxControllerInput
	{
		struct {
			// internally represented as Xinput state
			a3ui32 packet;
			a3ui16 buttons;
			a3ubyte lTrigger;
			a3ubyte rTrigger;
			a3i16 lThumbX;
			a3i16 lThumbY;
			a3i16 rThumbX;
			a3i16 rThumbY;

			// normalized axis controls
			a3f64 lTrigger_unit;
			a3f64 rTrigger_unit;
			a3f64 lThumbX_unit;
			a3f64 lThumbY_unit;
			a3f64 rThumbX_unit;
			a3f64 rThumbY_unit;
		} ctrl, ctrl0;

		a3ui16 rumble[2];
		a3i16 id;
		a3byte connected, connected0;
	};


	// A3: Enumeration of controller buttons.
	enum a3_XboxControllerButton
	{
		a3xbox_null,
		a3xbox_DPAD_up = 0x0001,
		a3xbox_DPAD_down = 0x0002,
		a3xbox_DPAD_left = 0x0004,
		a3xbox_DPAD_right = 0x0008,
		a3xbox_start = 0x0010,
		a3xbox_back = 0x0020,
		a3xbox_leftThumb = 0x0040,
		a3xbox_rightThumb = 0x0080,
		a3xbox_leftBumper = 0x0100,
		a3xbox_rightBumper = 0x0200,
		a3xbox_A = 0x1000,
		a3xbox_B = 0x2000,
		a3xbox_X = 0x4000,
		a3xbox_Y = 0x8000,
	};


	// A3: Enumeration of controller IDs by player number.
	enum a3_XboxControllerID
	{
		a3xbox_player1,
		a3xbox_player2,
		a3xbox_player3,
		a3xbox_player4,
	};


//-----------------------------------------------------------------------------

	// A3: Initialize controller given ID and update that controller.
	//	param control: non-null pointer to Xbox controller state
	//	param id: player number index, determines which controller this is
	//	return: 1 if success
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlSetID(a3_XboxControllerInput *control, const a3_XboxControllerID id);

	// A3: Get the state of a button.
	//	param control: non-null pointer to Xbox controller state
	//	param button: controller button to check the state of
	//	return:	1 if button is pressed
	//	return: 0 if button is not pressed
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlGetState(const a3_XboxControllerInput *control, const a3_XboxControllerButton button);

	// A3: Check if button is held down from last update.
	//	param control: non-null pointer to Xbox controller state
	//	param button: controller button to check
	//	return:	1 if button is down and was also down last update
	//	return: 0 if button is not down or has changed
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlIsHeld(const a3_XboxControllerInput *control, const a3_XboxControllerButton button);

	// A3: Check if button was pressed or released since last update.
	//	param control: non-null pointer to Xbox controller state
	//	param button: controller button to check
	//	return:	+1 if button changed from released to pressed
	//	return:	-1 if button changed from pressed to released
	//	return: 0 if button did not change
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlIsChanged(const a3_XboxControllerInput *control, const a3_XboxControllerButton button);

	// A3: Check if button was pressed since last update.
	//	param control: non-null pointer to Xbox controller state
	//	param button: controller button to check
	//	return:	1 if button was pressed
	//	return: 0 if button was not pressed
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlIsPressed(const a3_XboxControllerInput *control, const a3_XboxControllerButton button);

	// A3: Check if button was released since last update.
	//	param control: non-null pointer to Xbox controller state
	//	param button: controller button to check
	//	return:	1 if button was released
	//	return: 0 if button was not released
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlIsReleased(const a3_XboxControllerInput *control, const a3_XboxControllerButton button);

	// A3: Get left trigger state as normalized value.
	//	param control: non-null pointer to Xbox controller state
	//	param trigger_out: non-null pointer to double to receive trigger state
	//	return: 1 if success, trigger_out holds value of trigger
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlGetLeftTrigger(const a3_XboxControllerInput *control, a3f64 *trigger_out);

	// A3: Get right trigger state as normalized value.
	//	param control: non-null pointer to Xbox controller state
	//	param trigger_out: non-null pointer to double to receive trigger state
	//	return: 1 if success, trigger_out holds value of trigger
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlGetRightTrigger(const a3_XboxControllerInput *control, a3f64 *trigger_out);

	// A3: Get both trigger states as normalized values.
	//	param control: non-null pointer to Xbox controller state
	//	param leftTrigger_out: non-null pointer to receive left trigger state
	//	param rightTrigger_out: non-null pointer to receive right trigger state
	//	return: 1 if success, pointers hold the respective trigger values
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlGetTriggers(const a3_XboxControllerInput *control, a3f64 *leftTrigger_out, a3f64 *rightTrigger_out);

	// A3: Get left joystick state as normalized values.
	//	param control: non-null pointer to Xbox controller state
	//	param joystick_out: non-null array of 2 values to store joystick state
	//	return: 1 if success, trigger_out holds joystick X in position [0] 
	//		and holds joystick Y in position [1]
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlGetLeftJoystick(const a3_XboxControllerInput *control, a3f64 *joystick_out);

	// A3: Get right joystick state as normalized values.
	//	param control: non-null pointer to Xbox controller state
	//	param joystick_out: non-null array of 2 values to store joystick state
	//	return: 1 if success, trigger_out holds joystick X in position [0] 
	//		and holds joystick Y in position [1]
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlGetRightJoystick(const a3_XboxControllerInput *control, a3f64 *joystick_out);

	// A3: Get both joystick states as normalized values.
	//	param control: non-null pointer to Xbox controller state
	//	param leftJoystick_out: non-null array to store left joystick state
	//	param rightJoystick_out: non-null array to store right joystick state
	//	return: 1 if success, pointers hold the respective joystick values
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlGetJoysticks(const a3_XboxControllerInput *control, a3f64 *leftJoystick_out, a3f64 *rightJoystick_out);

	// A3: Get connection status.
	//	param control: non-null pointer to Xbox controller state
	//	return: 1 if connected
	//	return: 0 if not connected
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlIsConnected(const a3_XboxControllerInput *control);

	// A3: Check if connection was dropped or reconnected from last update.
	//	param control: non-null pointer to Xbox controller state
	//	return: +1 if reconnected
	//	return: -1 if disconnected
	//	return: 0 if no change
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlConnectionChanged(const a3_XboxControllerInput *control);

	// A3: Set controller vibration motor speeds. Note: left motor is for 
	//		low-frequency rumble, right motor is for high-frequency rumble.
	//	param control: non-null pointer to Xbox controller state
	//	param leftMotor: the left motor speed
	//	param rightMotor: the right motor speed
	//	return: 1 if success
	//	return: 0 if failed (not connected)
	//	return: a3input_invalid if invalid params
	a3ret a3XboxControlSetRumble(a3_XboxControllerInput *control, const a3ui16 leftMotor, const a3ui16 rightMotor);

	// A3: Update controller; checks connection state and sets button state.
	//	param control: non-null pointer to Xbox controller state
	//	return: 1 if connected and status updated
	//	return: 0 if not connected
	//	return: a3input_invalid if invalid param
	a3ret a3XboxControlUpdate(a3_XboxControllerInput *control);

	// A3: Reset state; sets all control states to zero.
	//	param control: non-null pointer to Xbox controller state
	//	return: 1 if success
	//	return: a3input_invalid if invalid param
	a3ret a3XboxControlReset(a3_XboxControllerInput *control);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_XboxControllerInput.inl"


#endif	// !__ANIMAL3D_XBOXCONTROLLERINPUT_H