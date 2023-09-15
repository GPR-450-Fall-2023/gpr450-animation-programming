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
	//Comment this out if you want to see the whole history with each frame in the terminal
 	//system("cls"); //Clear terminal

	//clipCtrl null check
	if (!clipCtrl)
	{
		printf("\n--------- Clip controller invalid - a3clipController, Update failed --------\n\n");
		return -1;
	}

	////////////// Pre resolution - Step time forward //////////////////////
	clipCtrl->keyframeTime += clipCtrl->playbackDirection * dt;
	clipCtrl->clipTime += clipCtrl->playbackDirection * dt;

	///////////////// Resolution - Handle results of time step ///////////////////////

	//Local variables
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
			//Do nothing
			resolved = true;
			printf("Playhead Paused - ");
		}
		else if (clipCtrl->playbackDirection > 0 //Have to be moving forward
			&& clipCtrl->keyframeTime >= keyframe.duration) //Playhead is past keyframe end - Forward Case 3 and 4
		{
			//Playhead is at or past end of clip
			if (clipCtrl->clipTime >= clip.duration) //Forward Terminus - Case 4
			{
				//How far past the end the playhead is
				a3real clipDiff = clipCtrl->clipTime - clip.duration;

				//Handle specific terminus actions
				switch (clipCtrl->terminusAction)
				{
					case LOOP:
						//Reset to beginning of clip while saving how far past the end
						//of the clip the playhead went
						clipCtrl->keyframe = clip.firstKeyframeIndex;
						clipCtrl->clipTime = clipDiff;
						clipCtrl->keyframeTime = clipDiff;

						//For debugging/testing only
						printf("Playhead Forward Loop Terminus - ");
						break;

					case STOP:
						//Pause clipController and clamp times to their max values
						clipCtrl->clipTime = clip.duration;
						clipCtrl->keyframeTime = keyframe.duration;
						clipCtrl->playbackDirection = 0;
						clipCtrl->keyframe = clip.lastKeyframeIndex;

						//For debugging/testing only
						printf("Playhead Forward Stop Terminus - ");
						break;
					case PING_PONG:
						//Reverse playback direction and calculate new playhead location by
						//sending it backwards as far is it went past the end of the clip
						clipCtrl->playbackDirection *= -1;
						a3real clipOverflow = (clipCtrl->clipTime - clip.duration);
						clipCtrl->clipTime = clip.duration - clipOverflow; //Reverse direction of overflowed time
						
						//Reset keyframe to make sure we're at the last one (could have skipped it)
						clipCtrl->keyframe = clip.lastKeyframeIndex;
						keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

						//Calculate keyframe time in the same way as the clipTime, use clipDiff 
						//since we are at the last keyframe
						clipCtrl->keyframeTime = keyframe.duration - (clipDiff);	
						

						printf("Playhead Forward Ping Pong Terminus - ");
						break;
				}
			}
			else //Forward Skip - Case 3
			{
				//How far past the end of the keyframe the playhead is
				a3real keyframeDiff = clipCtrl->keyframeTime - keyframe.duration;

				//Move to next keyframe and set new keyframe time
				clipCtrl->keyframe++;
				clipCtrl->keyframeTime = keyframeDiff;

				printf("Playhead Forward Skip - ");
			}
		}
		else if (clipCtrl->keyframeTime < 0) //playhead passed keyframe end - Backward - Case 6 and 7
		{
			if (clipCtrl->clipTime < 0) //Playhead passed clip end - Backward Terminus - Case 7
			{
				//Clip duration will be negative, want to add that negative to duration so we loop back to the end
				a3real clipDiff = clipCtrl->clipTime + clip.duration;

				//Handle clip terminus actions
				switch (clipCtrl->terminusAction)
				{
				case LOOP:
					//Loop keyframe to end of clip
					clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].lastKeyframeIndex;
					
					//Calculate keyframe time based on duration of the new frame
					a3_Keyframe nextFrame = clipCtrl->clipPool->clip[clipCtrl->clip].keyframePool->keyframe[clipCtrl->keyframe];
					clipCtrl->keyframeTime = clipCtrl->keyframeTime + nextFrame.duration; //Add this because keyframe time is negative
					
					//Set new clipTime
					clipCtrl->clipTime = clipDiff;

					//For debugging/testing only
					printf("Playhead Backward Loop Terminus - ");
					break;
				case STOP:

					//Zero out values and pause playhead so everything stops squarely at the beginning
					clipCtrl->clipTime = 0;
					clipCtrl->keyframeTime = 0;
					clipCtrl->playbackDirection = 0;
					clipCtrl->keyframe = clip.firstKeyframeIndex;

					//For debugging/testing only
					printf("Playhead Backward Stop Terminus - ");
					break;
				case PING_PONG:
					//Reverse the playhead direction
					clipCtrl->playbackDirection *= -1;

					//Reverse direction of overflowed time
					//clipTime should be negative if it has gone past 0 so the 
					//overflow is just the positive clipTime
					clipCtrl->clipTime = -clipCtrl->clipTime; //Flip sign
					clipCtrl->keyframeTime = clipCtrl->clipTime; //MAKE SURE you do not flip this sign (it has already been flipped)
					clipCtrl->keyframe = clip.firstKeyframeIndex;

					//For debugging/testing only
					printf("Playhead Backward Ping Pong Terminus - ");
					break;
				}
			}
			else//Backward Skip - Case 6
			{
				//Update new keyframe index
				clipCtrl->keyframe--;

				//New keyframe time = old keyframe time (which should be negative) + new keyframe duration
				a3_Keyframe nextFrame = clip.keyframePool->keyframe[clipCtrl->keyframe];
				clipCtrl->keyframeTime = clipCtrl->keyframeTime + nextFrame.duration;

				//For debugging/testing only
				printf("Playhead Backward Skip - ");
			}
		}
		else //Case 2 and 5 - forward and backward move (do nothing else)
		{
			//Do nothing
			resolved = true;
			
			//For debugging/testing only
			if (clipCtrl->playbackDirection > 0)
			{
				printf("Playhead moved forward - ");
			}
			else
			{
				printf("Playhead moved backward - ");
			}
		}

		//Update new clip/keyframe info
		clip = clipCtrl->clipPool->clip[clipCtrl->clip];
		keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];
	}

	//////////////// Post Resolution ////////////////////
	clipCtrl->keyframeParameter = clipCtrl->keyframeTime * keyframe.durationInverse;
	clipCtrl->clipParameter = clipCtrl->clipTime * clip.durationInverse;

	//For debugging/testing only, prints out clipCtrl info
	printf("\nClip Time: %f\nClip Duration: %f\nClip Normalized: %f\n\nKeyframe Index: %i\nKeyframeTime: %f\nKeyframe Duration: %f\nKeyframe Normalized: %f\n", clipCtrl->clipTime, clip.duration, clipCtrl->clipParameter, clipCtrl->keyframe, clipCtrl->keyframeTime, keyframe.duration, clipCtrl->keyframeParameter);
	printf("\n------ Update Finished, Data = %i,  ------\n\n", keyframe.data);

	return 0;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	//clipCtrl null check
	if (!clipCtrl)
	{
		printf("\n----------- Clip controller invalid - a3clipControllerSetClip failed -------------\n\n");
		return -1;
	}
	 
	//Reset time values, set new clip and clip pool
	clipCtrl->clipPool = clipPool;

	clipCtrl->clip = clipIndex_pool;
	clipCtrl->clipTime = 0;
	clipCtrl->clipParameter = 0;

	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;
	clipCtrl->keyframeTime = 0;
	clipCtrl->keyframeParameter = 0;

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H