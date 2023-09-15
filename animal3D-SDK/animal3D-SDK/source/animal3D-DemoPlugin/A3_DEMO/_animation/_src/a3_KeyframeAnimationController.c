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
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	//Copy name
	strcpy(clipCtrl_out->name, ctrlName);

	//Initialize clipCtrl values
	clipCtrl_out->terminusAction = LOOP;
	clipCtrl_out->playbackDirection = 1;

	//Initialize clip pool and set current clip
	if (a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool) != 0)
	{
		return -1;
	}

	return 0;
}

a3ui32 a3clipControllerPoolCreate(a3_ClipControllerPool* clipCtrlPool_out, a3_ClipPool* clipPool, const a3ui32 count)
{
	//clipCtrlPool = (a3_ClipControllerPool*) malloc(sizeof(a3_ClipControllerPool) + (sizeof(a3_ClipController) * count));
	//clipCtrlPool->count = count;

	// default values for clips
	const a3byte DEFAULT_CLIP_CONTROLLER_NAME[] = "Clip Controller";
	const a3ui32 DEFAULT_FIRST_INDEX = 0;

	if (clipCtrlPool_out == NULL) return -1; // return if clipCtrlPool_out doesn't exist
	if (clipPool == NULL) return -1; // return if clip pool doesn't exist

	clipCtrlPool_out->count = count; // set count

	clipCtrlPool_out->clipControllers = (a3_ClipController*)malloc(sizeof(a3_ClipController) * count); // create controller array

	if (clipCtrlPool_out->clipControllers == NULL) return -1;

	for (a3ui32 i = 0; i < count; i++)
	{
		// init each clip controller with default values
		a3clipControllerInit(clipCtrlPool_out->clipControllers + i, DEFAULT_CLIP_CONTROLLER_NAME, clipPool, DEFAULT_FIRST_INDEX);

		if ((clipCtrlPool_out->clipControllers + i) == NULL) return -1; // return if controller is null

		(clipCtrlPool_out->clipControllers + i)->index = i; //Log controllers index in pool
	}

	return 0;
}

a3ui32 a3clipControllerPoolRelease(a3_ClipControllerPool* clipCtrlPool)
{

	if (clipCtrlPool == NULL) return -1; // return if clipPool doesn't exist

	int returnCode = 0;

	if (clipCtrlPool->clipControllers == NULL) // only free clip array if it exists
	{
		returnCode = 1;
	}
	else
	{
		free(clipCtrlPool->clipControllers);
	}

	free(clipCtrlPool);

	return returnCode;

	return 0;
}


//-----------------------------------------------------------------------------
