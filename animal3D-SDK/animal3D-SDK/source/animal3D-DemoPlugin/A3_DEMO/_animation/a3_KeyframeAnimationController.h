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

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_KeyframeAnimationController.h
	Declaration of clip controller and clip controller pool
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
typedef struct a3_ClipControllerPool		a3_ClipControllerPool;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// clip controller
// metaphor: playhead
struct a3_ClipController
{
	//Name of clip controller
	a3byte name[a3keyframeAnimation_nameLenMax];

	// index in clip controller pool
	a3ui32 index;

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
	a3real lastPlaybackDirection;

	//How the clip controller ends clips
	//enum TerminusAction terminusAction; //////////////////////////// DELETE
	//a3i32 (*forwardTerminusAction)(void* clipCtrl);
	//a3i32 (*backwardTerminusAction)(void* clipCtrl);

	//Pointer to all clips controller controls
	const a3_ClipPool* clipPool;
};

struct a3_ClipControllerPool 
{
	//Array of clipControllers
	a3_ClipController* clipControllers;

	//Amount of controllers in clipControllers
	a3ui32 count;
};


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool);

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt);

// set clip to play
a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool);

//Create clip controller pool
a3i32 a3clipControllerPoolCreate(a3_ClipControllerPool* clipCtrlPool, a3_ClipPool* clipPool, const a3ui32 count);

//Release clip controller pool
a3i32 a3clipControllerPoolRelease(a3_ClipControllerPool* clipCtrlPool);

// lerp data within keyframe
a3i32 a3lerpKeyframeData(struct a3_ClipController* clipCtrl, a3real3p out_data);

//Generic lerp for a3real3 values
a3real3r a3real3GenericLerp(a3real3p out, a3real3p x0, a3real3p x1, a3real u);

//Generic catmull rom function from a3real3 values
a3real3r a3real3GenericCatmullRom(a3real3p out, a3real3p xP, a3real3p x0, a3real3p x1, a3real3p xN, a3real u);

/*

	Functions for handling setting indices and times for their respective 
	terminus actions in their respective directions

*/

//Pause playback at beginning or end of clip dependent on playback direction
a3i32 a3terminusPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Forward playback starting at first frame of next clip
a3i32 a3terminusForwardPlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Pause at beginning of clip pointed to by transition
a3i32 a3terminusForwardPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Reverse playback starting at end of last frame of next clip
a3i32 a3terminusReversePlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Reverse pause at end of clip
a3i32 a3terminusReversePause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Forward playback from end of first frame of next clip
a3i32 a3terminusForwardSkipPlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Forward pause at end of first frame of next clip
a3i32 a3terminusForwardSkipPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Reverse playback from beginning of last frame of next clip
a3i32 a3terminusReverseSkipPlayback(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//Reverse pause at beginning of first frame of next clip
a3i32 a3terminusReverseSkipPause(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);


/*

	Functions for handling how we determine what the next keyframe is

*/

//Gets keyframes from the next clip if the offset is out of bounds of the current clips keyframes
a3i32 a3getNextKeyframeFromNextClip(a3_ClipController* clipCtrl, a3_Keyframe* out_data, const a3ui32 offset);

//Gets keyframes from the beginning of the current clip
a3i32 a3getNextKeyframeLoop(a3_ClipController* clipCtrl, a3_Keyframe* out_data, const a3ui32 offset);

//a3i32 a3terminusForwardStop(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//a3i32 a3terminusForwardPingPong(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//a3i32 a3terminusBackwardStop(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);
//a3i32 a3terminusBackwardPingPong(a3_ClipController* clipCtrl, const a3_ClipTransition* transition);

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimationController.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H