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
	
	a3_MouseInput.inl
	Basic mouse input state tracking implementation.

	**DO NOT MODIFY THIS FILE**
*/


#ifdef __ANIMAL3D_MOUSEINPUT_H
#ifndef __ANIMAL3D_MOUSEINPUT_INL
#define __ANIMAL3D_MOUSEINPUT_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3mouseGetState(const a3_MouseInput *mouse, const a3_MouseButton button)
{
	if (mouse)
		return (mouse->btn.btn[button]);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseGetStateWheel(const a3_MouseInput *mouse)
{
	if (mouse)
		return (*(a3i16 *)(mouse->btn.btn + 6));
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseIsHeld(const a3_MouseInput *mouse, const a3_MouseButton button)
{
	if (mouse)
		return (mouse->btn.btn[button] && mouse->btn0.btn[button]);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseIsChanged(const a3_MouseInput *mouse, const a3_MouseButton button)
{
	if (mouse)
		return (mouse->btn.btn[button] - mouse->btn0.btn[button]);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseIsPressed(const a3_MouseInput *mouse, const a3_MouseButton button)
{
	if (mouse)
		return (mouse->btn.btn[button] && !mouse->btn0.btn[button]);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseIsReleased(const a3_MouseInput *mouse, const a3_MouseButton button)
{
	if (mouse)
		return (!mouse->btn.btn[button] && mouse->btn0.btn[button]);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseIsChangedWheel(const a3_MouseInput *mouse)
{
	if (mouse)
		return (*(a3i16 *)(mouse->btn.btn + 6) - *(a3i16 *)(mouse->btn0.btn + 6));
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseGetX(const a3_MouseInput *mouse)
{
	if (mouse)
		return (mouse->x);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseGetY(const a3_MouseInput *mouse)
{
	if (mouse)
		return (mouse->y);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseGetDeltaX(const a3_MouseInput *mouse)
{
	if (mouse)
		return (mouse->x - mouse->x0);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseGetDeltaY(const a3_MouseInput *mouse)
{
	if (mouse)
		return (mouse->y - mouse->y0);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseSetState(a3_MouseInput *mouse, const a3_MouseButton button, const a3_InputState state)
{
	if (mouse)
		return (mouse->btn.btn[button] = state);
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseSetStateWheel(a3_MouseInput *mouse, const a3_MouseWheelState delta)
{
	if (mouse)
		return (*(a3i16 *)(mouse->btn.btn + 6) += (mouse->btn.btn[a3mouse_wheel] = delta));
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseSetPosition(a3_MouseInput *mouse, const a3i32 x, const a3i32 y)
{
	if (mouse)
	{
		mouse->x = x;
		mouse->y = y;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseUpdate(a3_MouseInput *mouse)
{
	if (mouse)
	{
		mouse->btn0 = mouse->btn;
		mouse->x0 = mouse->x;
		mouse->y0 = mouse->y;
		return 1;
	}
	return a3input_invalid;
}

A3_INLINE a3ret a3mouseReset(a3_MouseInput *mouse)
{
	static const a3_MouseInput reset = { 0 };
		
	if (mouse)
	{
		mouse->btn = reset.btn;
		return 1;
	}
	return a3input_invalid;
}


//-----------------------------------------------------------------------------

#endif	// !__ANIMAL3D_MOUSEINPUT_INL
#endif	// __ANIMAL3D_MOUSEINPUT_H