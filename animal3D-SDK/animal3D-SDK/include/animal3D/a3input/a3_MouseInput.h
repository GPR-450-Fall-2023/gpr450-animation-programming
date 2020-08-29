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
	
	a3_MouseInput.h
	Basic mouse input state tracking interface.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_MOUSEINPUT_H
#define __ANIMAL3D_MOUSEINPUT_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_InputState.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_MouseInput		a3_MouseInput;
	typedef enum a3_MouseButton			a3_MouseButton;
	typedef enum a3_MouseWheelState		a3_MouseWheelState;
#endif	// __cplusplus
	

//-----------------------------------------------------------------------------
	
	// A3: Mouse state tracker.
	//	members btn, btn0: current and previous button states, respectively
	//	members x, y: current cursor position (top-left is 0,0)
	//	members x0, y: previous cursor position
	struct a3_MouseInput
	{
		struct {
			a3byte btn[8];
		} btn, btn0;

		a3i32 x, y;
		a3i32 x0, y0;
	};


	// A3: Enumeration of mouse buttons to check states.
	enum a3_MouseButton
	{
		a3mouse_left,
		a3mouse_middle,
		a3mouse_wheel,
		a3mouse_right,
		a3mouse_extra1,
		a3mouse_extra2,
	};


	// A3: Enumeration of possible wheel delta states.
	enum a3_MouseWheelState
	{
		a3mws_negative_large = -2,
		a3mws_negative,
		a3mws_neutral,
		a3mws_positive,
		a3mws_positive_large,
	};


//-----------------------------------------------------------------------------

	// A3: Get the state of a mouse button.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param button: button to check the state of
	//	return:	1 if button is pressed, or [-2, 2] for mouse wheel
	//	return: 0 if button is not pressed or mouse wheel is neutral
	//	return: a3input_invalid if invalid params
	a3ret a3mouseGetState(const a3_MouseInput *mouse, const a3_MouseButton button);

	// A3: Get the mouse wheel counter.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param button: button to check the state of
	//	return:	mouse wheel counter
	//	return: a3input_invalid if invalid params
	a3ret a3mouseGetStateWheel(const a3_MouseInput *mouse);

	// A3: Check if button is held down from last update.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param button: button to check
	//	return:	1 if button is down and was also down last update
	//	return: 0 if button is not down or has changed
	//	return: a3input_invalid if invalid params
	a3ret a3mouseIsHeld(const a3_MouseInput *mouse, const a3_MouseButton button);

	// A3: Check if button was pressed or released since last update.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param button: button to check
	//	return:	+1 if button changed from released to pressed
	//	return:	-1 if button changed from pressed to released
	//	return: 0 if button did not change
	//	return: a3input_invalid if invalid params
	a3ret a3mouseIsChanged(const a3_MouseInput *mouse, const a3_MouseButton button);

	// A3: Check if button was pressed since last update.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param button: button to check
	//	return:	1 if button was pressed
	//	return: 0 if button was not pressed
	//	return: a3input_invalid if invalid params
	a3ret a3mouseIsPressed(const a3_MouseInput *mouse, const a3_MouseButton button);

	// A3: Check if button was released since last update.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param button: button to check
	//	return:	1 if button was released
	//	return: 0 if button was not released
	//	return: a3input_invalid if invalid params
	a3ret a3mouseIsReleased(const a3_MouseInput *mouse, const a3_MouseButton button);

	// A3: Check if wheel counter changed from last update.
	//	param mouse: non-null pointer to mouse state descriptor
	//	return: wheel delta from last update
	//	return: a3input_invalid if invalid params
	a3ret a3mouseIsChangedWheel(const a3_MouseInput *mouse);

	// A3: Get current X position of cursor.
	//	param mouse: non-null pointer to mouse state descriptor
	//	return: cursor position X
	//	return: a3input_invalid if invalid params
	a3ret a3mouseGetX(const a3_MouseInput *mouse);

	// A3: Get current Y position of cursor.
	//	param mouse: non-null pointer to mouse state descriptor
	//	return: cursor position Y
	//	return: a3input_invalid if invalid params
	a3ret a3mouseGetY(const a3_MouseInput *mouse);

	// A3: Calculate the change in X position from the last update.
	//	param mouse: non-null pointer to mouse state descriptor
	//	return: X delta from last update
	//	return: a3input_invalid if invalid params
	a3ret a3mouseGetDeltaX(const a3_MouseInput *mouse);

	// A3: Calculate the change in Y position from the last update.
	//	param mouse: non-null pointer to mouse state descriptor
	//	return: Y delta from last update
	//	return: a3input_invalid if invalid params
	a3ret a3mouseGetDeltaY(const a3_MouseInput *mouse);

	// A3: Set the state of a mouse button. If setting for the wheel, this 
	//		only updates the wheel's delta, not the actual counter.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param button: button to set
	//	param state: new state for button
	//	return:	new button state
	//	return: a3input_invalid if invalid params
	a3ret a3mouseSetState(a3_MouseInput *mouse, const a3_MouseButton button, const a3_InputState state);

	// A3: Set the mouse wheel's delta and increment counter.
	//	param mouse: non-null pointer to mouse state descriptor
	//	param state: mouse wheel delta to change state and update counter
	//	return:	new wheel state
	//	return: a3input_invalid if invalid params
	a3ret a3mouseSetStateWheel(a3_MouseInput *mouse, const a3_MouseWheelState delta);

	// A3: Set cursor position.
	//	param mouse: non-null pointer to mouse state descriptor
	//	params x, y: new cursor position
	//	return: 1 if success
	//	return: a3input_invalid if invalid params
	a3ret a3mouseSetPosition(a3_MouseInput *mouse, const a3i32 x, const a3i32 y);

	// A3: Update mouse state.
	//	param mouse: non-null pointer to mouse state descriptor
	//	return:	1 if success
	//	return: a3input_invalid if invalid param
	a3ret a3mouseUpdate(a3_MouseInput *mouse);

	// A3: Reset current mouse state.
	//	param mouse: non-null pointer to mouse state descriptor
	//	return:	1 if success
	//	return: a3input_invalid if invalid param
	a3ret a3mouseReset(a3_MouseInput *mouse);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_MouseInput.inl"


#endif	// !__ANIMAL3D_MOUSEINPUT_H