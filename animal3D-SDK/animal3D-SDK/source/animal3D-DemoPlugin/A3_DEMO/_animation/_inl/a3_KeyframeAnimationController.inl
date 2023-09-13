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
	//system("cls"); //Clear terminal

	if (!clipCtrl)
	{
		printf("\n--------- Clip controller invalid - a3clipController, Update failed --------\n\n");
		return -1;
	}

	//Pre resolution
	clipCtrl->keyframeTime += clipCtrl->playbackDirection * dt;
	clipCtrl->clipTime += clipCtrl->playbackDirection * dt;

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
			printf("Playhead Paused\n");
		}
		else if (clipCtrl->playbackDirection > 0 //Have to be moving forward
			&& clipCtrl->keyframeTime >= keyframe.duration) //Forward Case 3 and 4
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

						printf("Playhead Forward Loop Terminus\n");
						break;

					case STOP:
						clipCtrl->clipTime = clip.duration;
						clipCtrl->keyframeTime = keyframe.duration;
						clipCtrl->playbackDirection = 0;
						printf("Playhead Forward Stop Terminus\n");
						break;
				}
			}
			else //Forward Skip - Case 3
			{
				a3real keyframeDiff = clipCtrl->keyframeTime - keyframe.duration;

				clipCtrl->keyframe++;
				clipCtrl->keyframeTime = keyframeDiff;

				printf("Playhead Forward Skip\n");
			}
		}
		else if (clipCtrl->keyframeTime < 0) //Backward - Case 6 and 7
		{
			if (clipCtrl->clipTime < 0) //Backward Terminus - Case 7
			{
				//Clip duration will be negative, want to add that negative to duration so we loop back to the end
				a3real clipDiff = clipCtrl->clipTime + clip.duration;

				switch (clipCtrl->terminusAction)
				{
				case LOOP:
					//Loop keyframe to end of clip
					clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].keyframePool->count - 1;
					
					a3_Keyframe nextFrame = clipCtrl->clipPool->clip[clipCtrl->clip].keyframePool->keyframe[clipCtrl->keyframe];
					clipCtrl->keyframeTime = clipCtrl->keyframeTime + nextFrame.duration;
					
					//Set new clipTime
					clipCtrl->clipTime = clipDiff;

					printf("Playhead Backward Loop Terminus\n");
					break;
				case STOP:
					clipCtrl->clipTime = 0;
					clipCtrl->keyframeTime = 0;
					clipCtrl->playbackDirection = 0;

					printf("Playhead Backward Stop Terminus\n");
					break;
				}
			}
			else//Backward Skip - Case 6
			{
				clipCtrl->keyframe--;

				//New keyframe time = old keyframe time (which should be negative) + new keyframe duration
				a3_Keyframe nextFrame = clipCtrl->clipPool->clip[clipCtrl->clip].keyframePool->keyframe[clipCtrl->keyframe];
				clipCtrl->keyframeTime = clipCtrl->keyframeTime + nextFrame.duration;

				printf("Playhead Backward Skip\n");
			}
		}
		else //Case 2 and 5 - forward and backward move (do nothing else)
		{
			resolved = true;
			
			if (clipCtrl->playbackDirection > 0)
			{
				printf("Playhead moved forward\n");
			}
			else
			{
				printf("Playhead moved backward\n");
			}
		}

		//Update new clip/keyframe info
		clip = clipCtrl->clipPool->clip[clipCtrl->clip];
		keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];
	}

	//Resolution
	clipCtrl->keyframeParameter = clipCtrl->keyframeTime * keyframe.durationInverse;
	clipCtrl->clipParameter = clipCtrl->clipTime * clip.durationInverse;

	printf("Clip Time: %f\nClip Duration: %f\nKeyframeTime: %f\nKeyframe Duration: %f\n", clipCtrl->clipTime, clip.duration, clipCtrl->keyframeTime, keyframe.duration);
	printf("\n------ Update Finished, Data = %i ------\n\n", keyframe.data);

	return 0;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	if (!clipCtrl)
	{
		printf("\n----------- Clip controller invalid - a3clipControllerSetClip failed -------------\n\n");
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