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
	
	a3_SpatialPose.inl
	Implementation of inline spatial pose operations.
*/


#ifdef __ANIMAL3D_SPATIALPOSE_H
#ifndef __ANIMAL3D_SPATIALPOSE_INL
#define __ANIMAL3D_SPATIALPOSE_INL


//-----------------------------------------------------------------------------

// set rotation values for a single node pose
inline a3i32 a3spatialPoseSetRotation(a3_SpatialPose* spatialPose, const a3f32 rx_degrees, const a3f32 ry_degrees, const a3f32 rz_degrees)
{
	if (spatialPose)
	{
		//Set rotation
		spatialPose->rotation[0] = rx_degrees;
		spatialPose->rotation[1] = ry_degrees;
		spatialPose->rotation[2] = rz_degrees;

		//Recalculate transform
		a3spatialPoseConvert(&spatialPose->transform, spatialPose, a3poseChannel_translate_xyz | a3poseChannel_orient_xyz | a3poseChannel_scale_xyz, a3poseEulerOrder_xyz);

		return 1;
	}
	return -1;
}

// scale
inline a3i32 a3spatialPoseSetScale(a3_SpatialPose* spatialPose, const a3f32 sx, const a3f32 sy, const a3f32 sz)
{
	if (spatialPose)
	{
		//Set scale
		spatialPose->scale[0] = sx;
		spatialPose->scale[1] = sy;
		spatialPose->scale[2] = sz;

		//Recalculate transform
		a3spatialPoseConvert(&spatialPose->transform, spatialPose, a3poseChannel_translate_xyz | a3poseChannel_orient_xyz | a3poseChannel_scale_xyz, a3poseEulerOrder_xyz);

		return 1;
	}
	return -1;
}

// translation
inline a3i32 a3spatialPoseSetTranslation(a3_SpatialPose* spatialPose, const a3f32 tx, const a3f32 ty, const a3f32 tz)
{
	if (spatialPose)
	{
		//Set translation
		spatialPose->translation[0] = tx;
		spatialPose->translation[1] = ty;
		spatialPose->translation[2] = tz;

		//Recalculate transform
		a3spatialPoseConvert(&spatialPose->transform, spatialPose, a3poseChannel_translate_xyz | a3poseChannel_orient_xyz | a3poseChannel_scale_xyz, a3poseEulerOrder_xyz);

		return 1;
	}
	return -1;
}

//// Transform
//a3i32 a3setSpatialPoseTransform(a3_SpatialPose* spatialPose, a3real4x4p trans)
//{
//	if (spatialPose)
//	{
//		//Set translation
//		a3translation
//
//		//Recalculate transform
//		a3spatialPoseConvert(&spatialPose->transform, spatialPose, a3poseChannel_translate_xyz | a3poseChannel_orient_xyz | a3poseChannel_scale_xyz, a3poseEulerOrder_xyz);
//
//		return 1;
//	}
//	return -1;
//}


//-----------------------------------------------------------------------------

// reset single node pose
inline a3i32 a3spatialPoseReset(a3_SpatialPose* spatialPose)
{
	if (spatialPose)
	{
		//Reset pose to default values
		spatialPose->rotation[0] = 0;
		spatialPose->rotation[1] = 0;
		spatialPose->rotation[2] = 0;

		spatialPose->translation[0] = 0;
		spatialPose->translation[1] = 0;
		spatialPose->translation[2] = 0;

		spatialPose->scale[0] = 1;
		spatialPose->scale[1] = 1;
		spatialPose->scale[2] = 1;

		spatialPose->transform = a3mat4_identity;

		return 1;
	}
	return -1;
}

// convert single node pose to matrix
inline a3i32 a3spatialPoseConvert(a3mat4* mat_out, const a3_SpatialPose* spatialPose_in, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (mat_out && spatialPose_in)
	{
		///////////////////////////////////////////////
		// TODO - IMPLEMENT
		///////////////////////////////////////////////

		//Create translation matrix
		a3real4x4 translation;
		a3real4x4SetIdentity(translation);
		a3real4x4Set(translation,
			1, 0, 0, spatialPose_in->translation[0],
			0, 1, 0, spatialPose_in->translation[1],
			0, 0, 1, spatialPose_in->translation[2],
			0, 0, 0, 1);

		//Create scale matrix
		a3real4x4 scale;
		a3real4x4SetIdentity(scale);
		a3real4x4Set(scale,
			spatialPose_in->scale[0], 0, 0, 0,
			0, spatialPose_in->scale[1], 0, 0,
			0, 0, spatialPose_in->scale[2], 0,
			0, 0, 0, 1);

		//Create rotation matrix
		a3real4x4 rotation;
		a3real4x4SetIdentity(rotation);
		a3real4x4SetRotateXYZ(rotation, spatialPose_in->rotation[0], spatialPose_in->rotation[1], spatialPose_in->rotation[2]);
		
		//Reset transform
		a3real4x4SetIdentity((a3real4*)mat_out);

		//Set mat_out to T*R*S*original
		a3real4x4MulTransform(scale, (a3real4*)mat_out);
		a3real4x4MulTransform(rotation, scale);
		a3real4x4MulTransform(translation, rotation);
		mat_out = (a3mat4*)translation;

		return 1;
	}
	return -1;
}

// copy operation for single node pose
inline a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		//Copy values not pointers
		*(spatialPose_out->rotation) = *spatialPose_in->rotation;
		*(spatialPose_out->translation) = *spatialPose_in->translation;
		*(spatialPose_out->scale) = *spatialPose_in->scale;
		spatialPose_out->transform = spatialPose_in->transform;

		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SPATIALPOSE_INL
#endif	// __ANIMAL3D_SPATIALPOSE_H