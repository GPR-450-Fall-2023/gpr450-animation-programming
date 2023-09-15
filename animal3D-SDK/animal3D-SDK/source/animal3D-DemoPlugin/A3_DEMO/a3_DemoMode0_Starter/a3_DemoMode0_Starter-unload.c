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

	a3_DemoMode0_Starter_initialize.c
	Demo mode implementations: starter scene.

	********************************************
	*** UNLOADING FOR STARTER SCENE MODE     ***
	********************************************
*/

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_DemoMode0_Starter-unload.c
	Demo mode implementations: keyframe and clip controller

	UNLOADING FOR KEYFRAME AND CLIP CONTROLLER
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode0_Starter.h"

#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3starter_unloadValidate(a3_DemoState* demoState, a3_DemoMode0_Starter* demoMode)
{

}


void a3starter_unload(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode)
{
	//CLEANUP HERE - Dillon
	a3clipControllerPoolRelease(&demoMode->clipCtrlPool);
	a3clipPoolRelease(&demoMode->clipPool);
	a3keyframePoolRelease(&demoMode->keyPool);
}


//-----------------------------------------------------------------------------
