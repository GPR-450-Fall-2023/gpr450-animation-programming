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
#include <math.h>


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	//Copy name
	strcpy(clipCtrl_out->name, ctrlName);

	//Initialize clipCtrl values
	clipCtrl_out->playbackDirection = 1;
	//clipCtrl_out->forwardTerminusAction = a3terminusForwardLoop;
	//clipCtrl_out->backwardTerminusAction = a3terminusBackwardLoop;

	//Initialize clip pool and set current clip
	if (a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool) != 0)
	{
		return -1;
	}

	return 0;
}

a3i32 a3clipControllerPoolCreate(a3_ClipControllerPool* clipCtrlPool_out, a3_ClipPool* clipPool, const a3ui32 count)
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

a3i32 a3clipControllerPoolRelease(a3_ClipControllerPool* clipCtrlPool)
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


//a3i32 a3triggerClipTransition(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
//{
//	if (!transition
//		|| !transition->clipPool)
//	{
//		return -1;
//	}
//
//	//Set clip index and pool for clip control
//	clipCtrl->clip = transition->index;
//	clipCtrl->clipPool = transition->clipPool;
//
//	//Transition
//	transition->transitionFunction(clipCtrl, transition);
//
//	return 0;
//}


a3i32 a3terminusPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//DO NOT switch the current clip or clip pool, pausing on current clip

	//Current clip and keyframe
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	if (clipCtrl->playbackDirection > 0) //Moving forward
	{
		//Clamp times to their max values
		clipCtrl->clipTime = clip.duration;
		clipCtrl->keyframeTime = keyframe.duration;
		clipCtrl->keyframe = clip.lastKeyframeIndex;
	}
	else //Moving backward
	{
		//Zero out values and pause playhead so everything stops squarely at the beginning
		clipCtrl->clipTime = 0;
		clipCtrl->keyframeTime = 0;
		clipCtrl->keyframe = clip.firstKeyframeIndex;
	}

	//Pause clip controller
	clipCtrl->playbackDirection = 0;

	return 0;
}


a3i32 a3terminusForwardPlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//How far past the end the playhead is (direction ambiguous since current direction could be backwards)
	a3real clipDiff = 0;
	if (clipCtrl->clipTime > 0) //Playing forward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime - clipCtrl->clipPool->clip[clipCtrl->clip].duration);
	}
	else //Playing backward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime);
	}

	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Current clip
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	//Reset to beginning of clip while saving how far past the end
	//of the clip the playhead went
	clipCtrl->keyframe = clip.firstKeyframeIndex;
	clipCtrl->clipTime = clipDiff;
	clipCtrl->keyframeTime = clipDiff;

	//Move clip controller forward
	clipCtrl->playbackDirection = 1;
	clipCtrl->lastPlaybackDirection = 1;

	return 0;
}


a3i32 a3terminusForwardPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Zero out values and pause playhead so everything stops squarely at the beginning
	clipCtrl->clipTime = 0;
	clipCtrl->keyframeTime = 0;
	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;

	//Pause clip controller
	clipCtrl->playbackDirection = 0;
	clipCtrl->lastPlaybackDirection = 1; //Ensures we move forward when unpausing

	return 0;
}


a3i32 a3terminusReversePlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//How far past the end the playhead is (direction ambiguous since current direction could be backwards)
	a3real clipDiff = 0;
	if (clipCtrl->clipTime > 0) //Playing forward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime - clipCtrl->clipPool->clip[clipCtrl->clip].duration);
	}
	else //Playing backward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime);
	}
	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Current clip
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	//Loop keyframe to end of clip
	clipCtrl->keyframe = clip.lastKeyframeIndex;

	//Calculate keyframe time based on duration of the new frame
	a3_Keyframe keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].keyframePool->keyframe[clipCtrl->keyframe];
	clipCtrl->keyframeTime = keyframe.duration - clipDiff; //subtract overflow of clip time from keyframe duration for new keyframe time

	//Set new clipTime
	clipCtrl->clipTime = clip.duration - clipDiff;

	//Reverse clip controller
	clipCtrl->playbackDirection = -1;
	clipCtrl->lastPlaybackDirection = -1;

	return 0;
}


a3i32 a3terminusReversePause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Current clip and keyframe
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	//Clamp times to their max values
	clipCtrl->clipTime = clip.duration;
	clipCtrl->keyframeTime = keyframe.duration;
	clipCtrl->keyframe = clip.lastKeyframeIndex;

	//Pause clip controller
	clipCtrl->playbackDirection = 0;
	clipCtrl->lastPlaybackDirection = -1; //Ensures we move backward when unpausing

	return 0;
}


a3i32 a3terminusForwardSkipPlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//How far past the end the playhead is (direction ambiguous since current direction could be backwards)
	a3real clipDiff = 0;
	if (clipCtrl->clipTime > 0) //Playing forward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime - clipCtrl->clipPool->clip[clipCtrl->clip].duration);
	}
	else //Playing backward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime);
	}

	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Current clip
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	//Set keyframe to first in clip
	clipCtrl->keyframe = clip.firstKeyframeIndex;

	//Set time
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe]; //New keyframe
	clipCtrl->keyframeTime = keyframe.duration + clipDiff; //End of first keyframe
	clipCtrl->clipTime = keyframe.duration + clipDiff; //Equivalent keyframe duration / clip duration

	//Move clip controller forward
	clipCtrl->playbackDirection = 1;
	clipCtrl->lastPlaybackDirection = 1;

	return 0;
}


a3i32 a3terminusForwardSkipPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Current clip
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	//Set keyframe to first in clip
	clipCtrl->keyframe = clip.firstKeyframeIndex;

	//Set time
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe]; //New keyframe
	clipCtrl->keyframeTime = keyframe.duration; //End of first keyframe
	clipCtrl->clipTime = keyframe.duration; //Equivalent keyframe duration / clip duration

	//Pause clip controller
	clipCtrl->playbackDirection = 0;
	clipCtrl->lastPlaybackDirection = 1; //Ensures we move forward when unpausing

	return 0;
}


a3i32 a3terminusReverseSkipPlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//How far past the end the playhead is (direction ambiguous since current direction could be backwards)
	a3real clipDiff = 0;
	if (clipCtrl->clipTime > 0) //Playing forward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime - clipCtrl->clipPool->clip[clipCtrl->clip].duration);
	}
	else //Playing backward
	{
		clipDiff = (a3real)fabs(clipCtrl->clipTime);
	}
	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Current clip
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	//Loop keyframe to end of clip
	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].lastKeyframeIndex;

	//Calculate keyframe time based on duration of the new frame
	a3_Keyframe keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].keyframePool->keyframe[clipCtrl->keyframe];
	clipCtrl->keyframeTime = -clipDiff; //clip diff is negative so we update the keyframe on the next loop through resolution

	//Set new clipTime
	clipCtrl->clipTime = clip.duration - keyframe.duration - clipDiff;

	//Reverse clip controller
	clipCtrl->playbackDirection = -1;
	clipCtrl->lastPlaybackDirection = -1;

	return 0;
}


a3i32 a3terminusReverseSkipPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
{
	if (!clipCtrl
		|| !transition
		|| !transition->clipPool)
	{
		return -1;
	}

	//Set next clip index and pool for clip control
	clipCtrl->clip = transition->index;
	clipCtrl->clipPool = transition->clipPool;

	//Current clip and keyframe
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	//Clamp times to their max values
	clipCtrl->clipTime = clip.duration - keyframe.duration;
	clipCtrl->keyframeTime = 0;
	clipCtrl->keyframe = clip.lastKeyframeIndex;

	//Pause clip controller
	clipCtrl->playbackDirection = 0;
	clipCtrl->lastPlaybackDirection = -1; //Ensures we move backward when unpausing

	return 0;
}

//a3i32 a3terminusForwardPingPong(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
//{
//	if (!clipCtrl
//		|| !transition
//		|| !transition->clipPool)
//	{
//		return -1;
//	}
//
//	//Set next clip index and pool for clip control
//	clipCtrl->clip = transition->index;
//	clipCtrl->clipPool = transition->clipPool;
//
//	//Current clip and keyframe
//	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
//	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];
//
//	//How far past the end the playhead is
//	a3real clipDiff = clipCtrl->clipTime - clip.duration;
//
//	//Reverse playback direction and calculate new playhead location by
//	//sending it backwards as far is it went past the end of the clip
//	clipCtrl->playbackDirection *= -1;
//	a3real clipOverflow = (clipCtrl->clipTime - clip.duration);
//	clipCtrl->clipTime = clip.duration - clipOverflow; //Reverse direction of overflowed time
//
//	//Reset keyframe to make sure we're at the last one (could have skipped it)
//	clipCtrl->keyframe = clip.lastKeyframeIndex;
//	keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];
//
//	//Calculate keyframe time in the same way as the clipTime, use clipDiff 
//	//since we are at the last keyframe
//	clipCtrl->keyframeTime = keyframe.duration - (clipDiff);
//
//
//	printf("Playhead Forward Ping Pong Terminus - ");
//
//	return 0;
//}





//a3i32 a3terminusBackwardStop(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
//{
//	if (!clipCtrl
//		|| !transition
//		|| !transition->clipPool)
//	{
//		return -1;
//	}
//
//	//Set next clip index and pool for clip control
//	clipCtrl->clip = transition->index;
//	clipCtrl->clipPool = transition->clipPool;
//
//	//Zero out values and pause playhead so everything stops squarely at the beginning
//	clipCtrl->clipTime = 0;
//	clipCtrl->keyframeTime = 0;
//	clipCtrl->playbackDirection = 0;
//	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;
//
//	//For debugging/testing only
//	printf("Playhead Backward Stop Terminus - ");
//
//	return 0;
//}


//a3i32 a3terminusBackwardPingPong(a3_ClipController* clipCtrl, const a3_ClipTransition* transition)
//{
//	if (!clipCtrl
//		|| !transition
//		|| !transition->clipPool)
//	{
//		return -1;
//	}
//
//	//Set next clip index and pool for clip control
//	clipCtrl->clip = transition->index;
//	clipCtrl->clipPool = transition->clipPool;
//	//Reverse the playhead direction
//	clipCtrl->playbackDirection *= -1;
//
//	//Reverse direction of overflowed time
//	//clipTime should be negative if it has gone past 0 so the 
//	//overflow is just the positive clipTime
//	clipCtrl->clipTime = -clipCtrl->clipTime; //Flip sign
//	clipCtrl->keyframeTime = clipCtrl->clipTime; //MAKE SURE you do not flip this sign (it has already been flipped)
//	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;
//
//	//For debugging/testing only
//	printf("Playhead Backward Ping Pong Terminus - ");
//
//	return 0;
//}


//-----------------------------------------------------------------------------
