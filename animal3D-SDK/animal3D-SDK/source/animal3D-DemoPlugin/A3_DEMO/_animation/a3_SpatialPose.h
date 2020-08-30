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
	
a3_SpatialPose.h
Description of a spatial pose with rotation, translation and scale.
*/

#ifndef __ANIMAL3D_SPATIALPOSE_H
#define __ANIMAL3D_SPATIALPOSE_H


// A3 math library
#include "animal3D-A3DM/animal3D-A3DM.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef enum a3_SpatialPoseChannel		a3_SpatialPoseChannel;
typedef struct a3_SpatialPose			a3_SpatialPose;
#endif	// __cplusplus
	

//-----------------------------------------------------------------------------

// flags to describe transformation components in use
// useful for constraining motion and kinematics
enum a3_SpatialPoseChannel
{
	// identity
	a3poseChannel_none,					// no channels
};

	
//-----------------------------------------------------------------------------

// single pose for a single node
struct a3_SpatialPose
{
	a3mat4 transform;
};


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_SpatialPose.inl"


#endif	// !__ANIMAL3D_SPATIALPOSE_H