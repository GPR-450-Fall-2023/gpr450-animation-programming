/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either ex
	or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_KeyframeAnimation.h
	Data structures for fundamental keyframe animation.
*/

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_KeyframeAnimation.h
	Declaration of keyframe, clip, keyframe pool, and clip pool
*/



#ifndef __ANIMAL3D_KEYFRAMEANIMATION_H
#define __ANIMAL3D_KEYFRAMEANIMATION_H


#include "animal3D-A3DM/a3math/a3vector.h"
#include "animal3D-A3DM/a3math/a3interpolation.h"

#include "A3_DEMO/_animation/a3_HierarchyState.h"

//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_Keyframe					a3_Keyframe;
typedef struct a3_KeyframePool				a3_KeyframePool;
typedef struct a3_Clip						a3_Clip;
typedef struct a3_ClipPool					a3_ClipPool;
typedef struct a3_ClipTransition			a3_ClipTransition;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

//Forward declarations
struct a3_ClipController;

// constant values
enum
{
	a3keyframeAnimation_nameLenMax = 32,
};


static const a3real a3keyframeAnimation_minDuration = (a3real) .01;


// description of single keyframe
// metaphor: moment
struct a3_Keyframe
{
	// index in keyframe pool
	a3ui32 index;

	// amount of time keyframe is active for, cannot be 0
	// NOTE: instead of setting this directly, you should use the setDuration function
	a3real duration;

	// reciprocal of duration
	// NOTE: instead of setting this directly, you should use the setDuration function
	a3real durationInverse;

	// data held by keyframe
	a3_HierarchyPose* data;

	// function for setting duration, will ensure duration > 0 and durationInverse will be set
	void (*setDuration)(a3_Keyframe* keyframe, const a3real newDuration);
};

// pool of keyframe descriptors
struct a3_KeyframePool
{
	// array of keyframes
	a3_Keyframe *keyframe;

	// number of keyframes
	a3ui32 count;
};


void a3keyframeSetDuration(a3_Keyframe* keyframe, const a3real duration);


// allocate keyframe pool
a3i32 a3keyframePoolCreate(a3_KeyframePool* keyframePool_out, const a3ui32 count, const a3_HierarchyPose* default_data);

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool);

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3_HierarchyPose* value);


//-----------------------------------------------------------------------------

struct a3_ClipTransition
{
	//Clip pool passed into the clip controller
	const a3_ClipPool* clipPool;

	//Index of clip to go to in pool
	a3ui32 index;

	//Next keyframe
	void (*getNextKeyframe)(struct a3_ClipController* clipCtrl, a3_Keyframe* out_data, const a3ui32 offset);

	//Terminus Action
	void (*transitionFunction)(struct a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
};

// description of single clip
// metaphor: timeline
struct a3_Clip
{
	// clip name
	a3byte name[a3keyframeAnimation_nameLenMax];

	// index in clip pool
	a3ui32 index;

	// total length of all keyframes combined
	a3real duration;

	// reciprocal of duration
	a3real durationInverse;

	// number of keyframes that clip is using
	a3ui32 keyframeCount;

	// index of first keyframe in pool
	a3ui32 firstKeyframeIndex;

	// index of last keyframe in pool
	a3ui32 lastKeyframeIndex;

	// pool with keyframes clip uses
	const a3_KeyframePool* keyframePool;

	//Clip transitions on either side of the clip
	a3_ClipTransition forwardTransition;
	a3_ClipTransition backwardTransition;
};

// group of clips
struct a3_ClipPool
{
	// array of clips
	a3_Clip* clip;

	// number of clips
	a3ui32 count;
};


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, a3_KeyframePool* keyframePool, const a3ui32 count);

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool);

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex);

// initialize clip transition with next clip, transition function, and clip pool
a3i32 a3clipTransitionInit(a3_ClipTransition* clipTransition, const a3ui32 index, const a3_ClipPool* clipPool, void(*transitionFunction), void(*getKeyframe));

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax]);

// calculate clip duration as sum of keyframes' durations
a3i32 a3clipCalculateDuration(a3_Clip* clip);

// calculate keyframes' durations by distributing clip's duration
a3i32 a3clipDistributeDuration(a3_Clip* clip, const a3real newClipDuration);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimation.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_H