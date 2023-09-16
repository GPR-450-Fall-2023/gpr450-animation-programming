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
	clipCtrl_out->playbackDirection = 1;
	clipCtrl_out->forwardTerminusAction = a3terminusForwardLoop;
	clipCtrl_out->backwardTerminusAction = a3terminusBackwardLoop;

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


a3ui32 a3terminusForwardLoop(a3_ClipController* clipCtrl)
{
	//Current clip
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	//How far past the end the playhead is
	a3real clipDiff = clipCtrl->clipTime - clip.duration;

	//Reset to beginning of clip while saving how far past the end
	//of the clip the playhead went
	clipCtrl->keyframe = clip.firstKeyframeIndex;
	clipCtrl->clipTime = clipDiff;
	clipCtrl->keyframeTime = clipDiff;

	//For debugging/testing only
	printf("Playhead Forward Loop Terminus - ");

	return 0;
}


a3ui32 a3terminusForwardStop(a3_ClipController* clipCtrl)
{
	//Current clip and keyframe
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	//Pause clipController and clamp times to their max values
	clipCtrl->clipTime = clip.duration;
	clipCtrl->keyframeTime = keyframe.duration;
	clipCtrl->playbackDirection = 0;
	clipCtrl->keyframe = clip.lastKeyframeIndex;

	//For debugging/testing only
	printf("Playhead Forward Stop Terminus - ");

	return 0;
}


a3ui32 a3terminusForwardPingPong(a3_ClipController* clipCtrl)
{
	//Current clip and keyframe
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	//How far past the end the playhead is
	a3real clipDiff = clipCtrl->clipTime - clip.duration;

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

	return 0;
}


a3ui32 a3terminusBackwardLoop(a3_ClipController* clipCtrl)
{
	//Current clip
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	//Clip duration will be negative, want to add that negative to duration so we loop back to the end
	a3real clipDiff = clipCtrl->clipTime + clip.duration;

	//Loop keyframe to end of clip
	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].lastKeyframeIndex;

	//Calculate keyframe time based on duration of the new frame
	a3_Keyframe nextFrame = clipCtrl->clipPool->clip[clipCtrl->clip].keyframePool->keyframe[clipCtrl->keyframe];
	clipCtrl->keyframeTime = clipCtrl->keyframeTime + nextFrame.duration; //Add this because keyframe time is negative

	//Set new clipTime
	clipCtrl->clipTime = clipDiff;

	//For debugging/testing only
	printf("Playhead Backward Loop Terminus - ");

	return 0;
}


a3ui32 a3terminusBackwardStop(a3_ClipController* clipCtrl)
{
	//Zero out values and pause playhead so everything stops squarely at the beginning
	clipCtrl->clipTime = 0;
	clipCtrl->keyframeTime = 0;
	clipCtrl->playbackDirection = 0;
	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;

	//For debugging/testing only
	printf("Playhead Backward Stop Terminus - ");

	return 0;
}


a3ui32 a3terminusBackwardPingPong(a3_ClipController* clipCtrl)
{
	//Reverse the playhead direction
	clipCtrl->playbackDirection *= -1;

	//Reverse direction of overflowed time
	//clipTime should be negative if it has gone past 0 so the 
	//overflow is just the positive clipTime
	clipCtrl->clipTime = -clipCtrl->clipTime; //Flip sign
	clipCtrl->keyframeTime = clipCtrl->clipTime; //MAKE SURE you do not flip this sign (it has already been flipped)
	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;

	//For debugging/testing only
	printf("Playhead Backward Ping Pong Terminus - ");

	return 0;
}


//-----------------------------------------------------------------------------
