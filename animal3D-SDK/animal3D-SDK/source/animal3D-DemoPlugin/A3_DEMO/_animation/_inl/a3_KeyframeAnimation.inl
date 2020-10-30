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


//-----------------------------------------------------------------------------

// calculate clip duration as sum of keyframes' durations
inline a3i32 a3clipCalculateDuration(a3_ClipPool const* clipPool, const a3ui32 clipIndex, const a3f64 playback_stepPerSec)
{
	if (clipPool && clipPool->clip && clipIndex < clipPool->clipCount && playback_stepPerSec > 0.0)
	{
		a3_Clip* clip = clipPool->clip + clipIndex;
		a3i32 i, k;
		clip->duration_step = 0;
		clip->duration_sec = clip->durationInv = 0.0;
		for (i = 0, k = clip->keyframeIndex_first; i < clip->keyframeCount; ++i, k += clip->keyframeDirection)
			clip->duration_step += clipPool->keyframe[k].duration_step;
		clip->duration_sec = (a3f64)clip->duration_step / playback_stepPerSec;
		clip->durationInv = __a3recipsafeF64(clip->duration_sec);
		return clip->index;
	}
	return -1;
}

// calculate keyframes' durations by distributing clip's duration
inline a3i32 a3clipDistributeDuration(a3_ClipPool const* clipPool, const a3ui32 clipIndex, const a3f64 playback_stepPerSec)
{
	if (clipPool && clipPool->clip && clipIndex < clipPool->clipCount && playback_stepPerSec > 0.0)
	{
		a3_Clip* clip = clipPool->clip + clipIndex;
		a3_Keyframe* keyframe;
		a3_Sample* sample0, * sample1;
		a3i32 i, k;
		for (i = 0, k = clip->keyframeIndex_first; i < clip->keyframeCount; ++i, k += clip->keyframeDirection)
		{
			keyframe = clipPool->keyframe + k;
			sample0 = clipPool->sample + keyframe->sampleIndex0;
			sample1 = clipPool->sample + keyframe->sampleIndex1;
			a3sampleInit(sample0, ((sample0->index * clip->duration_step) / clip->keyframeCount), playback_stepPerSec);
			a3sampleInit(sample1, ((sample1->index * clip->duration_step) / clip->keyframeCount), playback_stepPerSec);
			a3keyframeInit(clipPool->keyframe + k, sample0, sample1, playback_stepPerSec);
		}
		return clip->index;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATION_H