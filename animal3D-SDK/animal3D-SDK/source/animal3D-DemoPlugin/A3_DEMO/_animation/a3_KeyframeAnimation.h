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
	
	a3_KeyframeAnimation.h
	Data structures for fundamental keyframe animation.
*/

#ifndef __ANIMAL3D_KEYFRAMEANIMATION_H
#define __ANIMAL3D_KEYFRAMEANIMATION_H


#include "animal3D-A3DM/a3math/a3vector.h"
#include "animal3D-A3DM/a3math/a3interpolation.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_Sample					a3_Sample;
typedef struct a3_Keyframe					a3_Keyframe;
typedef enum a3_ClipTransitionFlag			a3_ClipTransitionFlag;
typedef struct a3_ClipTransition			a3_ClipTransition;
typedef struct a3_Clip						a3_Clip;
typedef struct a3_ClipPool					a3_ClipPool;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// constant values
enum
{
	a3keyframeAnimation_nameLenMax = 32,
};


// sample
// metaphor: moment
struct a3_Sample
{
	// index in sample pool
	a3i32 index;

	// time in steps
	a3i32 time_step;

	// time in seconds
	a3f64 time_sec;
};

// description of single keyframe
// metaphor: span of moments
struct a3_Keyframe
{
	// index in keyframe pool
	a3i32 index;

	// sample indices in pool
	a3ui32 sampleIndex0, sampleIndex1;

	// duration in steps
	a3ui32 duration_step;

	// duration in seconds and reciprocal
	a3f64 duration_sec, durationInv;
};


// initialize sample
a3i32 a3sampleInit(a3_Sample* sample_out, a3i32 const time_step, a3f64 const playback_stepPerSec);

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, a3_Sample const* sample0, a3_Sample const* sample1, a3f64 const playback_stepPerSec);


//-----------------------------------------------------------------------------

// clip transition flags
enum a3_ClipTransitionFlag
{
	a3clip_stopFlag = 0x00,		// stop flag (no change)
	a3clip_playFlag = 0x01,		// play after transition
	a3clip_reverseFlag = 0x02,	// direction is reverse after transition
	a3clip_skipFlag = 0x04,		// skip terminus frame after transition
	a3clip_overstepFlag = 0x08,	// keep overstep time after transition
	a3clip_terminusFlag = 0x10,	// target terminus is end of clip
	a3clip_offsetFlag = 0x20,	// there is an offset from the terminus
	a3clip_clipFlag = 0x40,		// there is a clip change
	a3clip_branchFlag = 0x80,	// there is a branch/condition
};

// clip transition
struct a3_ClipTransition
{
	a3_ClipTransitionFlag flag;
	a3i32 offset;
	a3i32 clipIndex;
};

// description of single clip
// metaphor: timeline
struct a3_Clip
{
	// clip name
	a3byte name[a3keyframeAnimation_nameLenMax];

	// index in clip pool
	a3i32 index;

	// keyframe indices, count and step direction
	a3i32 keyframeIndex_first, keyframeIndex_final, keyframeCount, keyframeDirection;

	// duration in steps
	a3i32 duration_step;

	// duration in seconds and reciprocal
	a3f64 duration_sec, durationInv;

	// transitions
	a3_ClipTransition transitionForward[1], transitionReverse[1];
};

// group of clips
struct a3_ClipPool
{
	// array of clips
	a3_Clip* clip;

	// array of keyframes
	a3_Keyframe* keyframe;

	// array of samples
	a3_Sample* sample;

	// counts
	a3ui32 clipCount, keyframeCount, sampleCount;
};


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 clipCount, const a3ui32 keyframeCount, const a3ui32 sampleCount);

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool);

// initialize clip transition
a3i32 a3clipTransitionInit(a3_ClipTransition* transition, a3_ClipTransitionFlag const transitionFlag, const a3i32 offset, a3_Clip const* clip);

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], a3_Keyframe const* keyframe_first, a3_Keyframe const* keyframe_final);

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax]);

// calculate clip duration as sum of keyframes' durations
a3i32 a3clipCalculateDuration(a3_ClipPool const* clipPool, const a3ui32 clipIndex, const a3f64 playback_stepPerSec);

// calculate keyframes' durations by distributing clip's duration
a3i32 a3clipDistributeDuration(a3_ClipPool const* clipPool, const a3ui32 clipIndex, const a3f64 playback_stepPerSec);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimation.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_H