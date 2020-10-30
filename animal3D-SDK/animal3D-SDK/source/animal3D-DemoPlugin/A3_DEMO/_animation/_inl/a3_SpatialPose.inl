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
		spatialPose->rotate.x = rx_degrees;
		spatialPose->rotate.y = ry_degrees;
		spatialPose->rotate.z = rz_degrees;
		return 1;
	}
	return -1;
}

// scale
inline a3i32 a3spatialPoseSetScale(a3_SpatialPose* spatialPose, const a3f32 sx, const a3f32 sy, const a3f32 sz)
{
	if (spatialPose)
	{
		spatialPose->scale.x = sx;
		spatialPose->scale.y = sy;
		spatialPose->scale.z = sz;
		return 1;
	}
	return -1;
}

// translation
inline a3i32 a3spatialPoseSetTranslation(a3_SpatialPose* spatialPose, const a3f32 tx, const a3f32 ty, const a3f32 tz)
{
	if (spatialPose)
	{
		spatialPose->translate.x = tx;
		spatialPose->translate.y = ty;
		spatialPose->translate.z = tz;
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// reset single node pose
inline a3i32 a3spatialPoseReset(a3_SpatialPose* spatialPose)
{
	if (spatialPose)
	{
		spatialPose->transformMat = a3mat4_identity;
		spatialPose->transformDQ = a3dualquat_identity;
		spatialPose->rotate = a3vec4_zero;
		spatialPose->scale = a3vec4_one;
		spatialPose->translate = a3vec4_w;
		spatialPose->user = a3vec4_zero;
		return 1;
	}
	return -1;
}

// convert single node pose to matrix
inline a3i32 a3spatialPoseConvert(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (spatialPose)
	{
		a3mat4 rx, ry, rz, r;
		a3real4x4SetRotateX(rx.m, a3trigValid_sind(spatialPose->rotate.x));
		a3real4x4SetRotateY(ry.m, a3trigValid_sind(spatialPose->rotate.y));
		a3real4x4SetRotateZ(rz.m, a3trigValid_sind(spatialPose->rotate.z));
		switch (order)
		{
		case a3poseEulerOrder_xyz:
			a3real4x4Product(r.m, rx.m, ry.m);
			a3real4x4Product(spatialPose->transformMat.m, r.m, rz.m);
			break;
		case a3poseEulerOrder_yzx:
			a3real4x4Product(r.m, ry.m, rz.m);
			a3real4x4Product(spatialPose->transformMat.m, r.m, rx.m);
			break;
		case a3poseEulerOrder_zxy:
			a3real4x4Product(r.m, rz.m, rx.m);
			a3real4x4Product(spatialPose->transformMat.m, r.m, ry.m);
			break;
		case a3poseEulerOrder_yxz:
			a3real4x4Product(r.m, ry.m, rx.m);
			a3real4x4Product(spatialPose->transformMat.m, r.m, rz.m);
			break;
		case a3poseEulerOrder_xzy:
			a3real4x4Product(r.m, rx.m, rz.m);
			a3real4x4Product(spatialPose->transformMat.m, r.m, ry.m);
			break;
		case a3poseEulerOrder_zyx:
			a3real4x4Product(r.m, rz.m, ry.m);
			a3real4x4Product(spatialPose->transformMat.m, r.m, rx.m);
			break;
		}
		a3real3MulS(spatialPose->transformMat.v0.v, spatialPose->scale.x);
		a3real3MulS(spatialPose->transformMat.v1.v, spatialPose->scale.y);
		a3real3MulS(spatialPose->transformMat.v2.v, spatialPose->scale.z);
		spatialPose->transformMat.v3 = spatialPose->translate;
		return 1;
	}
	return -1;
}

// restore single node pose from matrix
inline a3i32 a3spatialPoseRestore(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (spatialPose)
	{

	}
	return -1;
}

// copy operation for single node pose
inline a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		*spatialPose_out = *spatialPose_in;
		return 1;
	}
	return -1;
}

// concat
inline a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs)
{
	if (spatialPose_out && spatialPose_lhs && spatialPose_rhs)
	{
		spatialPose_out->rotate.x = spatialPose_lhs->rotate.x + spatialPose_rhs->rotate.x;
		spatialPose_out->rotate.y = spatialPose_lhs->rotate.y + spatialPose_rhs->rotate.y;
		spatialPose_out->rotate.z = spatialPose_lhs->rotate.z + spatialPose_rhs->rotate.z;

		spatialPose_out->scale.x = spatialPose_lhs->scale.x * spatialPose_rhs->scale.x;
		spatialPose_out->scale.y = spatialPose_lhs->scale.y * spatialPose_rhs->scale.y;
		spatialPose_out->scale.z = spatialPose_lhs->scale.z * spatialPose_rhs->scale.z;

		spatialPose_out->translate.x = spatialPose_lhs->translate.x + spatialPose_rhs->translate.x;
		spatialPose_out->translate.y = spatialPose_lhs->translate.y + spatialPose_rhs->translate.y;
		spatialPose_out->translate.z = spatialPose_lhs->translate.z + spatialPose_rhs->translate.z;

		return 1;
	}
	return -1;
}

// deconcat
inline a3i32 a3spatialPoseDeconcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs)
{
	if (spatialPose_out && spatialPose_lhs && spatialPose_rhs)
	{
		spatialPose_out->rotate.x = spatialPose_lhs->rotate.x - spatialPose_rhs->rotate.x;
		spatialPose_out->rotate.y = spatialPose_lhs->rotate.y - spatialPose_rhs->rotate.y;
		spatialPose_out->rotate.z = spatialPose_lhs->rotate.z - spatialPose_rhs->rotate.z;

		spatialPose_out->scale.x = spatialPose_lhs->scale.x / spatialPose_rhs->scale.x;
		spatialPose_out->scale.y = spatialPose_lhs->scale.y / spatialPose_rhs->scale.y;
		spatialPose_out->scale.z = spatialPose_lhs->scale.z / spatialPose_rhs->scale.z;

		spatialPose_out->translate.x = spatialPose_lhs->translate.x - spatialPose_rhs->translate.x;
		spatialPose_out->translate.y = spatialPose_lhs->translate.y - spatialPose_rhs->translate.y;
		spatialPose_out->translate.z = spatialPose_lhs->translate.z - spatialPose_rhs->translate.z;

		return 1;
	}
	return -1;
}

// lerp
inline a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_0, const a3_SpatialPose* spatialPose_1, const a3real u)
{
	if (spatialPose_out && spatialPose_0 && spatialPose_1)
	{
		a3real3Lerp(spatialPose_out->rotate.v, spatialPose_0->rotate.v, spatialPose_1->rotate.v, u);

		a3real3Lerp(spatialPose_out->scale.v, spatialPose_0->scale.v, spatialPose_1->scale.v, u);

		a3real3Lerp(spatialPose_out->translate.v, spatialPose_0->translate.v, spatialPose_1->translate.v, u);

		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SPATIALPOSE_INL
#endif	// __ANIMAL3D_SPATIALPOSE_H