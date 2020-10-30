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

#define a3sgn(x)	((x) != 0 ? ((x) > 0 ? +1 : -1) : 0)


//-----------------------------------------------------------------------------

// initialize sample
a3i32 a3sampleInit(a3_Sample* sample_out, a3i32 const time_step, a3f64 const playback_stepPerSec)
{
	if (sample_out && sample_out->index >= 0 && playback_stepPerSec > 0.0)
	{
		sample_out->time_step = time_step;
		sample_out->time_sec = (a3f64)time_step / playback_stepPerSec;
		return sample_out->index;
	}
	return -1;
}

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, a3_Sample const* sample0, a3_Sample const* sample1, a3f64 const playback_stepPerSec)
{
	if (keyframe_out && keyframe_out->index >= 0 && sample0 && sample0->index >= 0 && sample1 && sample1->index >= 0 && playback_stepPerSec > 0.0)
	{
		keyframe_out->sampleIndex0 = sample0->index;
		keyframe_out->sampleIndex1 = sample1->index;
		keyframe_out->duration_step = sample1->time_step - sample0->time_step;
		keyframe_out->duration_sec = (a3f64)keyframe_out->duration_step / playback_stepPerSec;
		keyframe_out->durationInv = __a3recipsafeF64(keyframe_out->duration_sec);
		return keyframe_out->index;
	}
	return -1;
}


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 clipCount, const a3ui32 keyframeCount, const a3ui32 sampleCount)
{
	if (clipPool_out && !clipPool_out->clip && clipCount && keyframeCount && sampleCount)
	{
		a3ui32 i;
		a3size const sz = sizeof(a3_Clip) * clipCount + sizeof(a3_Keyframe) * keyframeCount + sizeof(a3_Sample) * sampleCount;
		clipPool_out->clip = (a3_Clip*)malloc(sz);
		clipPool_out->keyframe = (a3_Keyframe*)(clipPool_out->clip + clipCount);
		clipPool_out->sample = (a3_Sample*)(clipPool_out->keyframe + keyframeCount);
		memset(clipPool_out->clip, 0, sz);
		for (i = 0, clipPool_out->clipCount = clipCount; i < clipCount; ++i)
			clipPool_out->clip[i].index = i;
		for (i = 0, clipPool_out->keyframeCount = keyframeCount; i < keyframeCount; ++i)
			clipPool_out->keyframe[i].index = i;
		for (i = 0, clipPool_out->sampleCount = sampleCount; i < sampleCount; ++i)
			clipPool_out->sample[i].index = i;
		return clipCount;
	}
	return -1;
}

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool)
{
	if (clipPool && clipPool->clip)
	{
		a3ui32 const clipCount = clipPool->clipCount;
		free(clipPool->clip);
		clipPool->clip = 0;
		clipPool->keyframe = 0;
		clipPool->sample = 0;
		clipPool->clipCount = 0;
		clipPool->keyframeCount = 0;
		clipPool->sampleCount = 0;
		return clipCount;
	}
	return -1;
}

// initialize clip transition
a3i32 a3clipTransitionInit(a3_ClipTransition* transition, a3_ClipTransitionFlag const transitionFlag, const a3i32 offset, a3_Clip const* clip)
{
	if (transition && clip)
	{
		transition->flag = transitionFlag;
		transition->offset = offset;
		transition->clipIndex = clip->index;
		return transitionFlag;
	}
	return -1;
}

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], a3_Keyframe const* keyframe_first, a3_Keyframe const* keyframe_final)
{
	if (clip_out && clip_out->index >= 0 && keyframe_first && keyframe_first->index >= 0 && keyframe_final && keyframe_final->index >= 0)
	{
		strncpy(clip_out->name, A3_CLIP_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		clip_out->keyframeIndex_first = keyframe_first->index;
		clip_out->keyframeIndex_final = keyframe_final->index;
		clip_out->keyframeCount = clip_out->keyframeIndex_final - clip_out->keyframeIndex_first;
		clip_out->keyframeDirection = a3sgn(clip_out->keyframeCount);
		clip_out->keyframeCount = 1 + clip_out->keyframeCount * clip_out->keyframeDirection;
		a3clipTransitionInit(clip_out->transitionForward, a3clip_stopFlag, 0, clip_out);
		a3clipTransitionInit(clip_out->transitionReverse, a3clip_stopFlag, 0, clip_out);
		return clip_out->index;
	}
	return -1;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	if (clipPool && clipPool->clip)
	{
		a3ui32 i;
		for (i = 0; i < clipPool->clipCount; ++i)
			if (strncmp(clipPool->clip[i].name, A3_CLIP_SEARCHNAME, a3keyframeAnimation_nameLenMax) == 0)
				return i;
	}
	return -1;
}


//-----------------------------------------------------------------------------
