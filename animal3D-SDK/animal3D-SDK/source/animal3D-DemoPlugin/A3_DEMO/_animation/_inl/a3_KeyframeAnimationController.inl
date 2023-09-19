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

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_KeyframeAnimationController.inl
	Inline function definitions for clip controller
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

				//Call the function from clips forward transition
				a3triggerClipTransition(clipCtrl, &clip.forwardTransition);
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

				//Call the function from clips backward transition
				a3triggerClipTransition(clipCtrl, &clip.backwardTransition);
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

	//Set keyframe index to the first keyframe in the clip
	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;
	clipCtrl->keyframeTime = 0;
	clipCtrl->keyframeParameter = 0;

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H