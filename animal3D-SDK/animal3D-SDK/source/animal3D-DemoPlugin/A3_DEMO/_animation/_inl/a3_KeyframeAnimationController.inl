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
	inline definitions for keyframe animation controller.
*/

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_KeyframeAnimationController.inl
	Inline function definitions for clip controller
*/

#ifdef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL

#include <stdio.h> 
#include <math.h> 

#include "animal3D-A3DM/a3math/a3vector.h"


//-----------------------------------------------------------------------------

// update clip controller
inline a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt)
{
	//Comment this out if you want to see the whole history with each frame in the terminal
 	//system("cls"); //Clear terminal

	//clipCtrl null check
	if (!clipCtrl)
	{
		printf("\n--------- Clip controller invalid - a3clipController, Update failed --------\n\n");
		return -1;
	}

	////////////// Pre resolution - Step time forward //////////////////////
	clipCtrl->keyframeTime += clipCtrl->playbackDirection * dt;
	clipCtrl->clipTime += clipCtrl->playbackDirection * dt;

	///////////////// Resolution - Handle results of time step ///////////////////////

	//Local variables
	a3boolean resolved = false;
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	const a3real FLOATING_POINT_ERROR = .00001f;

	//Loop through to make sure we start at a valid keyframe time
	//(i.e. playhead may have skipped over a keyframe so we have to do case 3 twice)
	while (!resolved)
	{
		//Resolution cases
		if (clipCtrl->playbackDirection == 0) //Paused - Case 1
		{
			//Do nothing
			resolved = true;
			printf("Playhead Paused - ");
		}
		else if (clipCtrl->playbackDirection > 0 //Have to be moving forward
			&& clipCtrl->keyframeTime >= keyframe.duration) //Playhead is past keyframe end - Forward Case 3 and 4
		{
			//Playhead is at or past end of clip
			//Be mindful of floating point error to make sure we don't accidentally move to wrong keyframe
			//if keyframeTime = .5 (keyframe duration .5) and clipTime = 2.999998 (clip duration 3)
			if (clipCtrl->clipTime >= clip.duration - FLOATING_POINT_ERROR) //Forward Terminus - Case 4
			{
				//How far past the end the playhead is
				a3real clipDiff = clipCtrl->clipTime - clip.duration;

				//Call the function from clips forward transition
				clip.forwardTransition.transitionFunction(clipCtrl, &clip.forwardTransition);
			}
			else //Forward Skip - Case 3
			{
				//How far past the end of the keyframe the playhead is
				a3real keyframeDiff = clipCtrl->keyframeTime - keyframe.duration;

				//Move to next keyframe and set new keyframe time
				clipCtrl->keyframe++;
				clipCtrl->keyframeTime = keyframeDiff;

				printf("Playhead Forward Skip - ");
			}
		}
		else if (clipCtrl->keyframeTime < 0) //playhead passed keyframe end - Backward - Case 6 and 7
		{
			//Be mindful of floating point error to make sure we don't accidentally move to wrong keyframe
			//if keyframeTime = 0 and clipTime = .00001 or some other trivially small value
			if (clipCtrl->clipTime < 0 + FLOATING_POINT_ERROR) //Playhead passed clip end - Backward Terminus - Case 7
			{
				//Clip duration will be negative, want to add that negative to duration so we loop back to the end
				a3real clipDiff = clipCtrl->clipTime + clip.duration;

				//Call the function from clips backward transition
				clip.backwardTransition.transitionFunction(clipCtrl, &clip.backwardTransition);
			}
			else//Backward Skip - Case 6
			{
				//Update new keyframe index
				clipCtrl->keyframe--;

				//New keyframe time = old keyframe time (which should be negative) + new keyframe duration
				a3_Keyframe nextFrame = clip.keyframePool->keyframe[clipCtrl->keyframe];
				clipCtrl->keyframeTime = clipCtrl->keyframeTime + nextFrame.duration;

				//For debugging/testing only
				printf("Playhead Backward Skip - ");
			}
		}
		else //Case 2 and 5 - forward and backward move (do nothing else)
		{
			//Do nothing
			resolved = true;
			
			//For debugging/testing only
			if (clipCtrl->playbackDirection > 0)
			{
				printf("Playhead moved forward - ");
			}
			else
			{
				printf("Playhead moved backward - ");
			}
		}

		//Update new clip/keyframe info
		clip = clipCtrl->clipPool->clip[clipCtrl->clip];
		keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];
	}

	//////////////// Post Resolution ////////////////////
	clipCtrl->keyframeParameter = clipCtrl->keyframeTime * keyframe.durationInverse;
	clipCtrl->clipParameter = clipCtrl->clipTime * clip.durationInverse;

	//store last direction not including pause
	//Used to figure out which direction to go when unpausing
	if (clipCtrl->playbackDirection != 0)
	{
		clipCtrl->lastPlaybackDirection = clipCtrl->playbackDirection; 
	}

	a3real lerpData[3];
	a3lerpKeyframeData(clipCtrl, lerpData);

	//For debugging/testing only, prints out clipCtrl info
	printf("\nClip Time: %f\nClip Duration: %f\nClip Normalized: %f\n\nKeyframe Index: %i\nKeyframeTime: %f\nKeyframe Duration: %f\nKeyframe Normalized: %f\n", clipCtrl->clipTime, clip.duration, clipCtrl->clipParameter, clipCtrl->keyframe, clipCtrl->keyframeTime, keyframe.duration, clipCtrl->keyframeParameter);
	printf("\n------ Update Finished, Data = (%f, %f, %f)  ------\n\n", lerpData[0], lerpData[1], lerpData[2]);

	return 0;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	//clipCtrl null check
	if (!clipCtrl)
	{
		printf("\n----------- Clip controller invalid - a3clipControllerSetClip failed -------------\n\n");
		return -1;
	}
	 
	//Reset time values, set new clip and clip pool
	clipCtrl->clipPool = clipPool;
	clipCtrl->clip = clipIndex_pool;
	clipCtrl->clipTime = 0;
	clipCtrl->clipParameter = 0;

	//Set keyframe index to the first keyframe in the clip
	clipCtrl->keyframe = clipCtrl->clipPool->clip[clipCtrl->clip].firstKeyframeIndex;
	clipCtrl->keyframeTime = 0;
	clipCtrl->keyframeParameter = 0;

	return 0;
}


inline a3i32 a3lerpKeyframeData(a3_ClipController* clipCtrl, a3real3p out_data)
{
	if (!clipCtrl
		|| !clipCtrl->clipPool)
	{
		return -1;
	}

	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];

	if (!clip.keyframePool)
	{
		return -1;
	}

	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	//a3ui32 indexOffset = (clipCtrl->keyframe - clip.firstKeyframeIndex + 1) //Add 1 to get next
	//	% (clip.lastKeyframeIndex - clip.firstKeyframeIndex + 1);	//Mod so it does not go out of range
	//a3_Keyframe nextKeyframe = clip.keyframePool->keyframe
	//	[clip.firstKeyframeIndex + indexOffset];


	/*printf("\nIndex: %i   Data: (%f, %f, %f)\nIndex %i   Data: (%f, %f, %f)\n\n", 
		clipCtrl->keyframe, 
		keyframe.data[0], keyframe.data[1], keyframe.data[2],
		clip.firstKeyframeIndex + indexOffset, 
		nextKeyframe.data[0], nextKeyframe.data[1], nextKeyframe.data[2]);*/

	a3_Keyframe nextKeyframe;
	if (clipCtrl->playbackDirection >= 0)
	{
		clip.forwardTransition.getNextKeyframe(clipCtrl, &nextKeyframe, 1);
	}
	else
	{
		clip.backwardTransition.getNextKeyframe(clipCtrl, &nextKeyframe, 1);
	}
	
	
	//Lerp values using keyframe data
	a3real3Lerp(out_data, keyframe.data, nextKeyframe.data, clipCtrl->keyframeParameter);


	///////// CATMULL ROM ///////////

	//a3ui32 indexOffsetNext = (clipCtrl->keyframe - clip.firstKeyframeIndex + 2)
	//	% (clip.lastKeyframeIndex - clip.firstKeyframeIndex + 1);
	//a3_Keyframe nextSkipKeyframe = clip.keyframePool->keyframe
	//	[clip.firstKeyframeIndex + indexOffsetNext];

	//a3ui32 indexOffsetPrev = (clipCtrl->keyframe - clip.firstKeyframeIndex - 1 + clip.lastKeyframeIndex - clip.firstKeyframeIndex + 1) //Add the mod value to make this positive
	//	% (clip.lastKeyframeIndex - clip.firstKeyframeIndex + 1);
	//a3_Keyframe prevKeyframe = clip.keyframePool->keyframe
	//	[clip.firstKeyframeIndex + indexOffsetPrev];

	//printf("\nIndex: %i   Data: (%f, %f, %f)\nIndex %i   Data: (%f, %f, %f)\nIndex %i   Data: (%f, %f, %f)\nIndex %i   Data: (%f, %f, %f)\n\n",
	//	clip.firstKeyframeIndex + indexOffsetPrev,
	//	prevKeyframe.data[0], prevKeyframe.data[1], prevKeyframe.data[2],
	//	clipCtrl->keyframe,
	//	keyframe.data[0], keyframe.data[1], keyframe.data[2],
	//	clip.firstKeyframeIndex + indexOffset,
	//	nextKeyframe.data[0], nextKeyframe.data[1], nextKeyframe.data[2],
	//	clip.firstKeyframeIndex + indexOffsetNext,
	//	nextSkipKeyframe.data[0], nextSkipKeyframe.data[1], nextSkipKeyframe.data[2]
	//);

	//a3real3GenericCatmullRom(
	//	out_data, 
	//	prevKeyframe.data,
	//	keyframe.data, 
	//	nextKeyframe.data, 
	//	nextSkipKeyframe.data, 
	//	clipCtrl->keyframeParameter
	//);

	return 0;
}

inline a3real3r a3real3GenericLerp(a3real3p out, a3real3p x0, a3real3p x1, a3real u)
{
	a3real3r sub = a3real3Sub(x1, x0); //x1 - x0
	a3real3r mul = a3real3MulS(sub, u); // (x1 - x0) * u
	a3real3r add = a3real3Add(x0, mul); // x0 + ((x1 - x0) * u)

	out[0] = add[0];
	out[1] = add[1];
	out[2] = add[2];

	return out;
}

//Adapted from https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline#Definition
inline a3real3r a3real3GenericCatmullRom(a3real3p out, a3real3p xP, a3real3p x0, a3real3p x1, a3real3p xN, a3real u)
{
	//a3real tP = 0.0f;
	//a3real3r d = a3real3Sub(x0, xP);
	//a3real t0 = (a3real)pow(a3real3Dot(d, d), .25) + u;
	//d = a3real3Sub(x1, x0);
	//a3real t1 = (a3real)pow(a3real3Dot(d, d), .25) + u;
	//d = a3real3Sub(xN, x1);
	//a3real tN = (a3real)pow(a3real3Dot(d, d), .25) + u;

	//a3real t = t0 + ((t1 - t0) * u); //Calculate new u by lerping it between current and next frame

	//a3real3r A1 = a3real3Add(
	//	a3real3MulS(xP, ((t0 - t) / (t0 - tP))), 
	//	a3real3MulS(x0, ((t - tP) / (t0 - tP)))
	//);
	//a3real3r A2 = a3real3Add(
	//	a3real3MulS(x0, ((t1 - t) / (t1 - t0))),
	//	a3real3MulS(x1, ((t - t0) / (t1 - t0)))
	//);
	//a3real3r A3val = a3real3Add( //A3 is reserved
	//	a3real3MulS(x1, ((tN - t) / (tN - t1))),
	//	a3real3MulS(xN, ((t - t1) / (tN - t1)))
	//);

	//a3real3r B1 = a3real3Add(
	//	a3real3MulS(A1, ((t1 - t) / (t1 - tP))),
	//	a3real3MulS(A2, ((t - tP) / (t1 - tP)))
	//);
	//a3real3r B2 = a3real3Add(
	//	a3real3MulS(A2, ((tN - t) / (tN - t0))),
	//	a3real3MulS(A3val, ((t - t0) / (tN - t0)))
	//);

	//a3real3r C = a3real3Add(
	//	a3real3MulS(B1, ((t1 - t) / (t1 - t0))),
	//	a3real3MulS(B2, ((t - t0) / (t1 - t0)))
	//);

	a3real tP = -u + (2 * u * u) - (u * u * u);
	a3real t0 = 2 - (5 * u * u) - (3 * u * u * u);
	a3real t1 = u + (4 * u * u) - (3 * u * u * u);
	a3real tN = (-3 * u * u) + (u * u * u);

	a3real3r catRom = a3real3Add(a3real3Add(a3real3MulS(xP, tP), a3real3MulS(x0, t0)), a3real3Add(a3real3MulS(x1, t1), a3real3MulS(xN, tN)));

	out[0] = catRom[0];
	out[1] = catRom[1];
	out[2] = catRom[2];

	return out;
}


//Gets keyframes from the next clip if the offset is out of bounds of the current clips keyframes
inline a3i32 a3getNextKeyframeFromNextClip(a3_ClipController* clipCtrl, a3_Keyframe* out_data, const a3ui32 offset)
{
	//a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	//a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	//a3ui32 indexOffset = (clipCtrl->keyframe - clip.firstKeyframeIndex + offset) //Add 1 to get next, 2 to get the one after
	//	% (clip.lastKeyframeIndex - clip.firstKeyframeIndex + 1);	//Mod so it does not go out of range

	//if (clipCtrl->playbackDirection >= 0)
	//{
	//	a3_Clip nextClip = clipCtrl->clipPool->clip[clip.forwardTransition.index];

	//	//Moving forward
	//	a3_Keyframe nextKeyframe;
	//	if (clipCtrl->keyframe + indexOffset > clip.lastKeyframeIndex)
	//	{
	//		a3i32 diff = (clipCtrl->keyframe + indexOffset) - clip.lastKeyframeIndex;

	//		nextKeyframe = nextClip.keyframePool->keyframe[nextClip.firstKeyframeIndex + diff];
	//	}
	//	else
	//	{
	//		nextKeyframe = clip.keyframePool->keyframe[clip.firstKeyframeIndex + indexOffset];
	//	}
	//}
	//else
	//{
	//	a3_Clip nextClip = clipCtrl->clipPool->clip[clip.forwardTransition.index];

	//	//Moving backward
	//	a3_Keyframe nextKeyframe;
	//	if (clipCtrl->keyframe + indexOffset < clip.lastKeyframeIndex)
	//	{
	//		a3i32 diff = (clipCtrl->keyframe + indexOffset) - clip.lastKeyframeIndex;

	//		nextKeyframe = nextClip.keyframePool->keyframe[nextClip.firstKeyframeIndex + diff];
	//	}
	//	else
	//	{
	//		nextKeyframe = clip.keyframePool->keyframe[clip.firstKeyframeIndex + indexOffset];
	//	}
	//}

	return 0;
}

//Gets keyframes from the beginning of the current clip
inline a3i32 a3getNextKeyframeLoop(a3_ClipController* clipCtrl, a3_Keyframe* out_data, const a3ui32 offset)
{
	a3_Clip clip = clipCtrl->clipPool->clip[clipCtrl->clip];
	a3_Keyframe keyframe = clip.keyframePool->keyframe[clipCtrl->keyframe];

	a3ui32 indexOffset = (clipCtrl->keyframe - clip.firstKeyframeIndex + 1) //Add 1 to get next
		% (clip.lastKeyframeIndex - clip.firstKeyframeIndex + 1);	//Mod so it does not go out of range
	a3_Keyframe nextKeyframe = clip.keyframePool->keyframe[clip.firstKeyframeIndex + indexOffset];

	printf("\nIndex: %i   Data: (%f, %f, %f)\nIndex %i   Data: (%f, %f, %f)\n\n",
		clipCtrl->keyframe,
		keyframe.data[0], keyframe.data[1], keyframe.data[2],
		clip.firstKeyframeIndex + indexOffset,
		nextKeyframe.data[0], nextKeyframe.data[1], nextKeyframe.data[2]);

	*out_data = nextKeyframe;

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H