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

	a3_DemoMode1_Animation-unload.c
	Demo mode implementations: animation scene.

	********************************************
	*** UNLOADING FOR ANIMATION SCENE MODE   ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"

#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3animation_unload(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode)
{
	// release skeleton and related assets
	a3hierarchyRelease(demoMode->sceneGraph);
	a3hierarchyStateRelease(demoMode->sceneGraphState);
	a3hierarchyRelease(demoMode->hierarchy_skel);
	a3hierarchyStateRelease(demoMode->hierarchyState_skel);
	a3hierarchyPoseGroupRelease(demoMode->hierarchyPoseGroup_skel);
}


void a3animation_unloadValidate(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode)
{

}


//-----------------------------------------------------------------------------
