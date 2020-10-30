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

#ifdef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL


//-----------------------------------------------------------------------------

inline a3i32 a3clipControllerSetPlayback(a3_ClipController* clipCtrl, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	if (clipCtrl && clipCtrl->clipPool && playback_stepPerSec > 0.0)
	{
		clipCtrl->playback_step = playback_step;
		clipCtrl->playback_stepPerSec = playback_stepPerSec;
		clipCtrl->playback_secPerStep = __a3recipsafeF64(playback_stepPerSec);
		clipCtrl->playback_sec = (a3f64)playback_step * clipCtrl->playback_secPerStep;
		return 1;
	}
	return -1;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	if (clipCtrl && clipPool && clipPool->clip && clipIndex_pool < clipPool->clipCount && playback_stepPerSec > 0.0)
	{
		clipCtrl->clipPool = clipPool;
		clipCtrl->clipIndex = clipIndex_pool;
		clipCtrl->clip = clipPool->clip + clipIndex_pool;

		// default testing behavior: set to first keyframe, discard time
		clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
		clipCtrl->keyframe = clipPool->keyframe + clipCtrl->keyframeIndex;
		clipCtrl->clipTime_step = clipCtrl->keyframeTime_step = 0;
		clipCtrl->clipTime_sec = clipCtrl->keyframeTime_sec = 0.0;
		clipCtrl->clipParam = clipCtrl->keyframeParam = 0.0;

		// set playback state
		a3clipControllerSetPlayback(clipCtrl, playback_step, playback_stepPerSec);

		// done
		return clipIndex_pool;
	}
	return -1;
}

// update clip controller
inline a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
		// variables
		a3f64 overstep;

		// time step
		dt *= clipCtrl->playback_sec;
		clipCtrl->clipTime_sec += dt;
		clipCtrl->keyframeTime_sec += dt;
		//dt *= clipCtrl->playback_stepPerSec;
		//clipCtrl->clipTime_step += (a3i32)dt;
		//clipCtrl->keyframeTime_step += (a3i32)dt;

		// resolve forward
		while ((overstep = clipCtrl->keyframeTime_sec - clipCtrl->keyframe->duration_sec) >= 0.0)
		{
			// are we passing the forward terminus of the clip
			if (clipCtrl->keyframeIndex == clipCtrl->clip->keyframeIndex_final)
			{
				// handle forward transition

				// default testing behavior: loop with overstep
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
				clipCtrl->keyframeTime_sec = overstep;
			}
			// are we simply moving to the next keyframe
			else
			{
				// set keyframe indices
				clipCtrl->keyframeIndex += clipCtrl->clip->keyframeDirection;

				// set keyframe pointers
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

				// new time is just the overstep
				clipCtrl->keyframeTime_sec = overstep;
			}
		}

		// resolve reverse
		while ((overstep = clipCtrl->keyframeTime_sec) < 0.0)
		{
			// are we passing the reverse terminus of the clip
			if (clipCtrl->keyframeIndex == clipCtrl->clip->keyframeIndex_first)
			{
				// handle reverse transition

				// default testing behavior: loop with overstep
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
				clipCtrl->keyframeTime_sec = overstep + clipCtrl->keyframe->duration_sec;
			}
			// are we simply moving to the previous keyframe
			else
			{
				clipCtrl->keyframeIndex -= clipCtrl->clip->keyframeDirection;

				// set keyframe pointers
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

				// new time is overstep (negative) from new duration
				clipCtrl->keyframeTime_sec = overstep + clipCtrl->keyframe->duration_sec;
			}
		}

		// normalize
		clipCtrl->keyframeParam = clipCtrl->keyframeTime_sec * clipCtrl->keyframe->durationInv;
		clipCtrl->clipParam = clipCtrl->clipTime_sec * clipCtrl->clip->durationInv;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H