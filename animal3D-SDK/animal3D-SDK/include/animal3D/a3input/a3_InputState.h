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
	
	a3_InputState.h
	Common input button states.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_INPUTSTATE_H
#define __ANIMAL3D_INPUTSTATE_H


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef enum a3_InputState			a3_InputState;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Possible input button states.
	enum a3_InputState
	{
		a3input_up,
		a3input_down,
	};


	// A3: Possible input button state changes.
	enum a3_InputStateChange
	{
		a3input_invalid = -128,
		a3input_released = -1,
		a3input_unchanged,
		a3input_pressed,
	};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_INPUTSTATE_H