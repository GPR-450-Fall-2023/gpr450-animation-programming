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
typedef enum a3_SpatialPoseEulerOrder	a3_SpatialPoseEulerOrder;
typedef enum a3_SpatialPoseChannel		a3_SpatialPoseChannel;
typedef struct a3_SpatialPose			a3_SpatialPose;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// list of Euler angle product orders
enum a3_SpatialPoseEulerOrder
{
	a3poseEulerOrder_xyz,
	a3poseEulerOrder_yzx,
	a3poseEulerOrder_zxy,
	a3poseEulerOrder_yxz,
	a3poseEulerOrder_xzy,
	a3poseEulerOrder_zyx,
};


//-----------------------------------------------------------------------------

// flags to describe transformation components in use
// useful for constraining motion and kinematics
enum a3_SpatialPoseChannel
{
	// identity (no channels)
	a3poseChannel_none,

	// rotation
	a3poseChannel_rotate_x = 0x0001,
	a3poseChannel_rotate_y = 0x0002,
	a3poseChannel_rotate_z = 0x0004,
	a3poseChannel_rotate_w = 0x0008,
	a3poseChannel_rotate_xy = a3poseChannel_rotate_x | a3poseChannel_rotate_y,
	a3poseChannel_rotate_yz = a3poseChannel_rotate_y | a3poseChannel_rotate_z,
	a3poseChannel_rotate_zx = a3poseChannel_rotate_z | a3poseChannel_rotate_x,
	a3poseChannel_rotate_xyz = a3poseChannel_rotate_xy | a3poseChannel_rotate_z,

	// scale
	a3poseChannel_scale_x = 0x0010,
	a3poseChannel_scale_y = 0x0020,
	a3poseChannel_scale_z = 0x0040,
	a3poseChannel_scale_w = 0x0080,
	a3poseChannel_scale_xy = a3poseChannel_scale_x | a3poseChannel_scale_y,
	a3poseChannel_scale_yz = a3poseChannel_scale_y | a3poseChannel_scale_z,
	a3poseChannel_scale_zx = a3poseChannel_scale_z | a3poseChannel_scale_x,
	a3poseChannel_scale_xyz = a3poseChannel_scale_xy | a3poseChannel_scale_z,

	// translation
	a3poseChannel_translate_x = 0x0100,
	a3poseChannel_translate_y = 0x0200,
	a3poseChannel_translate_z = 0x0400,
	a3poseChannel_translate_w = 0x0800,
	a3poseChannel_translate_xy = a3poseChannel_translate_x | a3poseChannel_translate_y,
	a3poseChannel_translate_yz = a3poseChannel_translate_y | a3poseChannel_translate_z,
	a3poseChannel_translate_zx = a3poseChannel_translate_z | a3poseChannel_translate_x,
	a3poseChannel_translate_xyz = a3poseChannel_translate_xy | a3poseChannel_translate_z,

	// user channels
	a3poseChannel_user_x = 0x1000,
	a3poseChannel_user_y = 0x2000,
	a3poseChannel_user_z = 0x4000,
	a3poseChannel_user_w = 0x8000,
	a3poseChannel_user_xy = a3poseChannel_user_x | a3poseChannel_user_y,
	a3poseChannel_user_yz = a3poseChannel_user_y | a3poseChannel_user_z,
	a3poseChannel_user_zx = a3poseChannel_user_z | a3poseChannel_user_x,
	a3poseChannel_user_xyz = a3poseChannel_user_xy | a3poseChannel_user_z,
};

	
//-----------------------------------------------------------------------------

// single pose for a single node
struct a3_SpatialPose
{
	a3mat4 transformMat;
	a3dualquat transformDQ;
	a3vec4 rotate;
	a3vec4 scale;
	a3vec4 translate;
	a3vec4 user;
};


//-----------------------------------------------------------------------------

// set rotation values for a single node pose
a3i32 a3spatialPoseSetRotation(a3_SpatialPose* spatialPose, const a3f32 rx_degrees, const a3f32 ry_degrees, const a3f32 rz_degrees);

// scale
a3i32 a3spatialPoseSetScale(a3_SpatialPose* spatialPose, const a3f32 sx, const a3f32 sy, const a3f32 sz);

// translation
a3i32 a3spatialPoseSetTranslation(a3_SpatialPose* spatialPose, const a3f32 tx, const a3f32 ty, const a3f32 tz);


//-----------------------------------------------------------------------------

// reset single node pose
a3i32 a3spatialPoseReset(a3_SpatialPose* spatialPose);

// convert single node pose to matrix
a3i32 a3spatialPoseConvert(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order);

// restore single node pose from matrix
a3i32 a3spatialPoseRestore(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order);

// copy operation for single node pose
a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_in);

// concat
a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs);

// deconcat
a3i32 a3spatialPoseDeconcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs);

// lerp
a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_0, const a3_SpatialPose* spatialPose_1, const a3real u);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_SpatialPose.inl"


#endif	// !__ANIMAL3D_SPATIALPOSE_H