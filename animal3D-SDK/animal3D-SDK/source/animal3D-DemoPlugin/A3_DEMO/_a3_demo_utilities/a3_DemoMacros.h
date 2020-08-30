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

	a3_DemoMacros.h
	Declarations of helpful macros.

	********************************************
	*** Declarations of utility macros.      ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOMACROS_H
#define __ANIMAL3D_DEMOMACROS_H


//-----------------------------------------------------------------------------
// helpers for toggling and looping controls

#define a3demoCtrlToggle(value)											(value = !value)
#define a3demoCtrlIncLoop(value, maximum)								(value = (value + 1) % maximum)
#define a3demoCtrlDecLoop(value, maximum)								(value = (value + maximum - 1) % maximum)
#define a3demoCtrlIncCap(value, maximum)								(value = value < maximum ? value + 1 : value)
#define a3demoCtrlDecCap(value, minimum)								(value = value > minimum ? value - 1 : value)
#define a3demoCtrlIncClamp(value, maximum, minimum)						(value = value < maximum && value > minimum ? maximum : value)
#define a3demoCtrlDecClamp(value, maximum, minimum)						(value = value > minimum && value < maximum ? minimum : value)

#define a3demoCtrlCaseToggle(value, ctrl)								case ctrl: a3demoCtrlToggle(value); break
#define a3demoCtrlCaseIncLoop(value, maximum, ctrl)						case ctrl: a3demoCtrlIncLoop(value, maximum); break
#define a3demoCtrlCaseDecLoop(value, maximum, ctrl)						case ctrl: a3demoCtrlDecLoop(value, maximum); break
#define a3demoCtrlCaseIncCap(value, maximum, ctrl)						case ctrl: a3demoCtrlIncCap(value, maximum); break
#define a3demoCtrlCaseDecCap(value, minimum, ctrl)						case ctrl: a3demoCtrlDecCap(value, minimum); break

#define a3demoCtrlCasesLoop(value, maximum, ctrlInc, ctrlDec)			a3demoCtrlCaseIncLoop(value, maximum, ctrlInc);	a3demoCtrlCaseDecLoop(value, maximum, ctrlDec)
#define a3demoCtrlCasesCap(value, maximum, minimum, ctrlInc, ctrlDec)	a3demoCtrlCaseIncCap(value, maximum, ctrlInc);	a3demoCtrlCaseDecCap(value, minimum, ctrlDec)


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_DEMOMACROS_H