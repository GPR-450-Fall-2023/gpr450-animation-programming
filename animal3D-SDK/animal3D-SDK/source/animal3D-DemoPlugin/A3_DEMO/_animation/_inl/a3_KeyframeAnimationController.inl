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
	a3boolean resolved = false;

	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	//Loop through to make sure we start at a valid keyframe time
	//(i.e. playhead may have skipped over a keyframe so we have to do case 3 twice)
	while (!resolved)
	{
		//Resolution cases
		if (clipCtrl->playbackDirection == 0) //Paused - Case 1
		{
			resolved = true;
			printf("Playhead Paused");
		}
		else if (clipCtrl->keyframeTime >= keyframe.duration) //Forward Case 3 and 4
		{
			if (clipCtrl->clipTime >= clip.duration) //Forward Terminus - Case 4
			{
				a3real clipDiff = clipCtrl->clipTime - clip.duration;

				switch (clipCtrl->terminusAction)
				{
					case LOOP:
						clipCtrl->keyframe = 0;
						clipCtrl->clipTime = clipDiff;
						clipCtrl->keyframeTime = clipDiff;

						printf("Playhead Forward Loop Terminus");
						break;
				}
			}
			else //Forward Skip - Case 3
			{
				a3real keyframeDiff = clipCtrl->keyframeTime - keyframe.duration;

				clipCtrl->keyframe++;
				clipCtrl->keyframeTime = keyframeDiff;

				printf("Playhead Forward Skip");
			}
		}
		else if (clipCtrl->keyframeTime < 0) //Backward Skip - Case 6
		{


			//Backward Terminus - Case 7 (which imples case 6
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

	printf("Update Finished, Data = %i", keyframe.data);

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