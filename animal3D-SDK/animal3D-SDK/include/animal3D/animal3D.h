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
	
	animal3D.h
	Include all animal3D SDK header files.

	**DO NOT MODIFY THIS FILE**
*/


#ifndef __ANIMAL3D_H
#define __ANIMAL3D_H


//-----------------------------------------------------------------------------
// A3: Utility includes.

#include "animal3D/a3utility/a3_Stream.h"
#include "animal3D/a3utility/a3_Timer.h"
#include "animal3D/a3utility/a3_Thread.h"


//-----------------------------------------------------------------------------
// A3: Input includes.

#include "animal3D/a3input/a3_InputState.h"
#include "animal3D/a3input/a3_MouseInput.h"
#include "animal3D/a3input/a3_KeyboardInput.h"
#include "animal3D/a3input/a3_XboxControllerInput.h"


//-----------------------------------------------------------------------------
// A3: Pre-defined geometry includes.

#include "animal3D/a3geometry/a3_GeometryData.h"
#include "animal3D/a3geometry/a3_ProceduralGeometry.h"
#include "animal3D/a3geometry/a3_ModelLoader_WavefrontOBJ.h"


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_H