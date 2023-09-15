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
	
	a3_KeyframeAnimation.inl
	Inline definitions for keyframe animation.
*/

#ifdef __ANIMAL3D_KEYFRAMEANIMATION_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATION_INL
#define __ANIMAL3D_KEYFRAMEANIMATION_INL

// These are here to provide this file access to NULL for error checking
// https ://stackoverflow.com/questions/50379663/why-i-get-null-is-undefined-error
#include <stdlib.h>
#include <stdio.h>


//-----------------------------------------------------------------------------

// calculate clip duration as sum of keyframes' durations
inline a3i32 a3clipCalculateDuration(a3_Clip* clip)
{
	// return if anything needed is null
	if(clip == NULL) return -1;
	if(clip->keyframePool == NULL) return 1;

	a3real duration = 0;

	// add together all keyframe durations
	for (a3ui32 i = clip->firstKeyframeIndex; i <= clip->lastKeyframeIndex; i++)
	{
		duration += clip->keyframePool->keyframe[i].duration;
	}

	// make sure duration is greater than minimum
	if (duration < a3keyframeAnimation_minDuration)
	{
		clip->duration = a3keyframeAnimation_minDuration;
	}
	else
	{
		clip->duration = duration;
	}

	// set inverse
	clip->durationInverse = 1 / clip->duration;

	return 0;
}

// calculate keyframes' durations by distributing clip's duration
inline a3i32 a3clipDistributeDuration(a3_Clip* clip, const a3real newClipDuration)
{
	// return if anything needed is null
	if(clip == NULL) return -1;
	if(clip->keyframePool == NULL) return 1;
	if(clip->keyframeCount <= 0) return 1;

	a3_Keyframe* keyframe; // keyframe pointer we'll use for storage
	
	// calculate duration per keyframe
	a3real durationPerKeyframe = newClipDuration / clip->keyframeCount;

	// loop through each keyframe and set their duration
	for (a3ui32 i = clip->firstKeyframeIndex; i <= clip->lastKeyframeIndex; i++)
	{
		keyframe = (clip->keyframePool->keyframe + i);
		a3keyframeSetDuration(keyframe, durationPerKeyframe);
	}

	// do this in case durationPerKeyFrame ended up under min value
	// if it did, each keyframe would automatically have set duration to min value
	a3clipCalculateDuration(clip);

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATION_H