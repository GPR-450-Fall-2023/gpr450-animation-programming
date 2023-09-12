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
	
	a3_KeyframeAnimationController.h
	inline definitions for keyframe animation controller.
*/

#ifdef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL

#include <stdio.h> 
#include <stdbool.h>


//-----------------------------------------------------------------------------

// update clip controller
inline a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt)
{
	if (!clipCtrl)
	{
		printf("Clip controller invalid - a3clipControllerUpdate failed");
		return -1;
	}

	//Pre resolution
	clipCtrl->keyframeTime += dt;
	clipCtrl->clipTime += dt;

	//Resolution
	bool resolved = false;

	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	while (!resolved)
	{
		//Paused - Case 1
		if (clipCtrl->playbackDirection == 0)
		{
			resolved = true;
			printf("Playhead Paused");
		}
		else if (clipCtrl->keyframeTime >= keyframe.duration) //Forward Skip - Case 3
		{
			//Forward Terminus - Case 4

			if (clipCtrl->clipTime >= clip.duration)
			{
				printf("Playhead Forward Terminus");
			}
			else
			{
				printf("Playhead Forward Skip");
			}
		}
		else if (clipCtrl->keyframeTime < 0) //Backward Skip - Case 6
		{
			//Backward Terminus - Case 7
			if (clipCtrl->clipTime < 0)
			{
				printf("Playhead Backward Terminus");
			}
			else
			{
				printf("Playhead Backward Skip");
			}
		}
		else //Case 2 and 5 - forward and backward move (do nothing else)
		{
			resolved = true;
			
			if (clipCtrl->playbackDirection > 0)
			{
				printf("Playhead moved forward");
			}
			else
			{
				printf("Playhead moved backward");
			}
		}

		//Update new clip/keyframe info
		clip = clipCtrl->clipPool->clip[clipCtrl->clip];
		keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];
	}

	//Resolution
	clipCtrl->keyframeParameter = clipCtrl->keyframeTime * keyframe.durationInverse;
	clipCtrl->clipTime = clipCtrl->clipTime * clip.durationInverse;

	printf("Update Finished");

	return 0;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	if (!clipCtrl)
	{
		printf("Clip controller invalid - a3clipControllerSetClip failed");
		return -1;
	}

	clipCtrl->clipPool = clipPool;

	clipCtrl->clip = clipIndex_pool;
	clipCtrl->clipTime = 0;
	clipCtrl->clipParameter = 0;

	clipCtrl->keyframe = 0;
	clipCtrl->keyframeTime = 0;
	clipCtrl->keyframeParameter = 0;

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H