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

#include "../a3_KeyframeAnimation.h"

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

	keyframePool_out = (a3_KeyframePool*) malloc(sizeof(a3_KeyframePool));

	if (keyframePool_out == NULL) return -1;

	keyframePool_out->keyframe = (a3_Keyframe*) malloc(count * sizeof(a3_Keyframe));

	if(keyframePool_out->keyframe == NULL) return -1;

	keyframePool_out->count = count;

	for (a3ui16 i = 0; i < count; i++)
	{
		a3keyframeInit((keyframePool_out->keyframe + i), KEYFRAME_DEFAULT_DURATION, KEYFRAME_DEFAULT_DATA);
		(keyframePool_out->keyframe + i)->index = i;
	}

	return 1;
}

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool)
{
	if (keyframePool == NULL) return -1;
	
	int returnCode = 0;
	
	if (keyframePool->keyframe == NULL)
	{
		returnCode = 1;
	}
	else
	{
		free(keyframePool->keyframe);
	}

	free(keyframePool);

	return 0;
}

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3ui32 value_x)
{
	if(keyframe_out == NULL) return -1;

	keyframe_out->data = value_x;
	keyframe_out->setDuration = a3keyframeSetDuration;
	keyframe_out->setDuration(keyframe_out, duration);

	return 0;
}


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 count)
{
	clipPool_out = (a3_ClipPool*) malloc(sizeof(a3_ClipPool));

	if(clipPool_out == NULL) return -1;

	clipPool_out->count = count;

	clipPool_out->clip = (a3_Clip*) malloc(sizeof(a3_Clip) * count);

	if(clipPool_out->clip == NULL) return 1;

	return 0;
}

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool)
{
	if(clipPool == NULL) return -1;

	int returnCode = 0;

	if (clipPool->clip == NULL)
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
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex)
{
	if(clip_out == NULL) return -1;

	// clip_out->name = clipName;
	strcpy(clip_out->name, clipName);

	clip_out->keyframePool = keyframePool;
	clip_out->firstKeyframeIndex = firstKeyframeIndex;
	clip_out->lastKeyFrameIndex = finalKeyframeIndex;
	clip_out->keyframeCount = finalKeyframeIndex - firstKeyframeIndex + 1; // +1 since both final and first are included in pool

	a3clipCalculateDuration(clip_out);

	if(firstKeyframeIndex > finalKeyframeIndex) return 1;

	return 1;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	if(clipPool == NULL) return -1;

	for (a3ui32 i = 0; i < clipPool->count; i++)
	{
		if (strcmp(clipPool->clip[i].name, clipName) == 0)
		{
			return i;
		}
	}

	return -1;
}


//-----------------------------------------------------------------------------
