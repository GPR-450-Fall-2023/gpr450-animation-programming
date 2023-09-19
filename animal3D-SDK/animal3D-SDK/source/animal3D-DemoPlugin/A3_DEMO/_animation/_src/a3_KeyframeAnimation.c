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
	
	a3_KeyframeAnimation.c
	Implementation of keyframe animation interfaces.
*/

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_KeyframeAnimation.c
	Implementation of keyframe and clip controller interfaces
*/

#include "../a3_KeyframeAnimation.h"
#include "../a3_KeyframeAnimationController.h"

#include <stdlib.h>
#include <string.h>


// macros to help with names
#define A3_CLIP_DEFAULTNAME		("unnamed clip")
#define A3_CLIP_SEARCHNAME		((clipName && *clipName) ? clipName : A3_CLIP_DEFAULTNAME)


//-----------------------------------------------------------------------------


// sets keyframe duration while handling edge cases and setting duration inverse
void a3keyframeSetDuration(a3_Keyframe* keyframe, const a3real duration)
{
	// set keyframe duration to provided duration or minDuration, whichever is higher
	if (duration < a3keyframeAnimation_minDuration)
	{
		keyframe->duration = a3keyframeAnimation_minDuration;
	}
	else
	{
		keyframe->duration = duration;
	}

	// set durationInverse to reciprocal
	keyframe->durationInverse = 1 / keyframe->duration;
}


// allocate keyframe pool
a3i32 a3keyframePoolCreate(a3_KeyframePool* keyframePool_out, const a3ui32 count)
{
	const a3real KEYFRAME_DEFAULT_DURATION = 1; // init each keyframe with this duration
	const a3ui32 KEYFRAME_DEFAULT_DATA = 0; // init each keyframe with this value

	if (keyframePool_out == NULL) return -1; // return if keyframePool doesn't already exist

	// allocate keyframe array
	keyframePool_out->keyframe = (a3_Keyframe*) malloc(count * sizeof(a3_Keyframe));

	if(keyframePool_out->keyframe == NULL) return -1; // return if malloc failed

	keyframePool_out->count = count; // set count

	// init each keyframe with default values
	for (a3ui16 i = 0; i < count; i++)
	{
		a3keyframeInit((keyframePool_out->keyframe + i), KEYFRAME_DEFAULT_DURATION, KEYFRAME_DEFAULT_DATA);
		(keyframePool_out->keyframe + i)->index = i; //Log keyframes index in pool
	}

	return 1;
}

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool)
{
	if (keyframePool == NULL) return -1; // return if keyframePool doesn't exist
	
	int returnCode = 0; // code we will return
	
	// release keyframe array if it exists
	if (keyframePool->keyframe == NULL)
	{
		returnCode = 1;
	}
	else
	{
		free(keyframePool->keyframe);
	}

	free(keyframePool);

	return returnCode;
}

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3ui32 value_x)
{
	if(keyframe_out == NULL) return -1; // return if keyframe_out is null

	// set keyframe variables
	keyframe_out->data = value_x;
	keyframe_out->setDuration = a3keyframeSetDuration; // give keyframe appropriate setter function
	keyframe_out->setDuration(keyframe_out, duration);

	return 0;
}


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, a3_KeyframePool* keyframePool, const a3ui32 count)
{
	// default values for clips
	const a3byte DEFAULT_CLIP_NAME[] = "Clip";
	const a3ui32 DEFAULT_FIRST_INDEX = 0;

	if(clipPool_out == NULL) return -1; // return if clipPool_out doesn't exist
	if(keyframePool == NULL) return -1; // return if keyframePool doesn't exist

	a3ui32 lastIndex = DEFAULT_FIRST_INDEX + keyframePool->count - 1; //Default last index should be the index of the last keyframe in the pool

	clipPool_out->count = count; // set count

	clipPool_out->clip = (a3_Clip*) malloc(sizeof(a3_Clip) * count); // create clip array

	if(clipPool_out->clip == NULL) return -1;

	for (a3ui32 i = 0; i < count; i++)
	{
		// init each clip with default values
		a3clipInit((clipPool_out->clip + i), DEFAULT_CLIP_NAME, clipPool_out, keyframePool, DEFAULT_FIRST_INDEX, lastIndex);
		
		if ((clipPool_out->clip + i) == NULL) return -1; // return if clip is null
		
		(clipPool_out->clip + i)->index = i; //Log clips index in pool
	}

	return 0;
}

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool)
{
	if(clipPool == NULL) return -1; // return if clipPool doesn't exist

	int returnCode = 0;

	if (clipPool->clip == NULL) // only free clip array if it exists
	{
		returnCode = 1;
	}
	else
	{
		free(clipPool->clip);
	}

	free(clipPool);

	return returnCode;
}

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex)
{
	const a3ui32 DEFAULT_TRANSITION_INDEX = 0;
	void(*DEFAULT_FORWARD_TRANSITION) = a3terminusForwardPlayback;
	void(*DEFAULT_BACKWARD_TRANSITION) = a3terminusBackwardPlayback;

	if(clip_out == NULL) return -1; // return if clip_out doesn't exist

	// copy passed in name to clip name
	strcpy(clip_out->name, clipName);

	// set variables
	clip_out->keyframePool = keyframePool;
	clip_out->firstKeyframeIndex = firstKeyframeIndex;
	clip_out->lastKeyframeIndex = finalKeyframeIndex;
	clip_out->keyframeCount = finalKeyframeIndex - firstKeyframeIndex + 1; // +1 since both final and first are included in pool

	a3clipTransitionInit(&clip_out->forwardTransition, DEFAULT_TRANSITION_INDEX, clipPool, DEFAULT_FORWARD_TRANSITION);
	a3clipTransitionInit(&clip_out->backwardTransition, DEFAULT_TRANSITION_INDEX, clipPool, DEFAULT_BACKWARD_TRANSITION);

	a3clipCalculateDuration(clip_out);

	if(firstKeyframeIndex > finalKeyframeIndex) return 1; // return warning if first index > final index

	return 0;
}

a3i32 a3clipTransitionInit(a3_ClipTransition* clipTransition, const a3ui32 index, const a3_ClipPool* clipPool, void(*transitionFunction))
{
	if (clipTransition == NULL || clipPool == NULL) return -1;

	clipTransition->index = index;
	clipTransition->clipPool = clipPool;
	clipTransition->transitionFunction = transitionFunction;

	return 0;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	if(clipPool == NULL) return -1; // return if clipPool is null

	// loop through to find index of clip in pool
	for (a3ui32 i = 0; i < clipPool->count; i++)
	{
		if (strcmp(clipPool->clip[i].name, clipName) == 0) // true if we've found the name we're looking for
		{
			return i;
		}
	}

	return -1;
}


//-----------------------------------------------------------------------------
