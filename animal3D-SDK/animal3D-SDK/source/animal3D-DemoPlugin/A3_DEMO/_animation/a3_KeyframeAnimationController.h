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
	
	a3_KeyframeAnimationController.h
	Keyframe animation clip controller. Basically a frame index manager. Very 
	limited in what one can do with this; could potentially be so much more.
*/

#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H


#include "a3_KeyframeAnimation.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_ClipController			a3_ClipController;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

//terminus actions - what the clip does at the end
enum TerminusActions {
	STOP = 0,
	LOOP,
	PING_PONG
};

// clip controller
// metaphor: playhead
struct a3_ClipController
{
	a3byte name[a3keyframeAnimation_nameLenMax];

	//Index of current referenced clip in pool
	a3ui32 clip;
	//Current time from start of clip [0, clipDuration)
	a3real clipTime;
	//Normalized clip time [0, 1) (only 1 if clip ends)
	a3real clipParameter;

	//Index of current frame in keyframe pool
	a3ui32 keyframe;
	//Current time from start of keyframe
	a3real keyframeTime;
	//Normalized keyframe time [0, 1) (only 1 if the last keyframe ends with a stop terminus action)
	a3real keyframeParameter;

	//Indicates play direction and speed (+1 means forward at normal pace, 0 is paused, -1 is backwards)
	a3real playbackDirection;

	//Pointer to all clips controller controls
	const a3_ClipPool* clipPool;
};


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool);

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt);

// set clip to play
a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimationController.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H