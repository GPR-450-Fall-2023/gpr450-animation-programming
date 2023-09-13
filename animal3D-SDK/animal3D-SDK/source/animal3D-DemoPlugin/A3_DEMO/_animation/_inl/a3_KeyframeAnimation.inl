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
	if(clip == NULL) return -1;
	if(clip->keyframePool == NULL) return 1;

	a3real duration = 0;

	for (a3ui32 i = clip->firstKeyframeIndex; i <= clip->lastKeyFrameIndex; i++)
	{
		duration += clip->keyframePool->keyframe[i].duration;
	}

	if (duration < a3keyframeAnimation_minDuration)
	{
		clip->duration = a3keyframeAnimation_minDuration;
	}
	else
	{
		clip->duration = duration;
	}

	clip->durationInverse = 1 / clip->duration;

	return 0;
}

// calculate keyframes' durations by distributing clip's duration
inline a3i32 a3clipDistributeDuration(a3_Clip* clip, const a3real newClipDuration)
{
	if(clip == NULL) return -1;
	if(clip->keyframePool == NULL) return 1;

	a3_Keyframe* keyframe;
	a3real durationPerKeyframe = newClipDuration / clip->keyframeCount;

	for (a3ui32 i = clip->firstKeyframeIndex; i <= clip->lastKeyFrameIndex; i++)
	{
		keyframe = (clip->keyframePool->keyframe + i);
		a3keyframeSetDuration(keyframe, durationPerKeyframe);
	}

	// Do this in case durationPerKeyFrame ended up under min value
	// If it did, each keyframe would automatically have set duration to min value
	a3clipCalculateDuration(clip);

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATION_H