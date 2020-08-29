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
	animal3D Math (A3DM) SDK
	By Daniel S. Buckstein

	a3sqrt.h
	Declarations for fast square root functions.
*/

#ifndef __ANIMAL3D_A3DM_SQRT_H
#define __ANIMAL3D_A3DM_SQRT_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3macros.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------

// A3: Compute fast square root using Quake's method ("0x5f3759df") using 
//		Newton's approximation method. Note: only works with float precision.
//	param x: number to square root
//	return: approximate square root of x
A3_INLINE a3f32 a3sqrtf0x(const a3f32 x);

// A3: Compute fast inverse square root using Quake's method and Newton's 
//		approximation method.
//	param x: number to square root and invert
//	return: approximate inverse (reciprocal) square root of x
A3_INLINE a3f32 a3sqrtf0xInverse(const a3f32 x);


// A3: Fast and precise square root for floats.
//	param x: number to square root
//	return: square root of x
extern a3f32 a3sqrtf(const a3f32 x);

// A3: Fast and precise square root for doubles.
//	param x: number to square root
//	return: square root of x
extern a3f64 a3sqrtd(const a3f64 x);

// A3: Fast and precise inverse square root for floats.
//	param x: number to square root
//	return: square root of x
extern a3f32 a3sqrtfInverse(const a3f32 x);

// A3: Fast and precise inverse square root for doubles.
//	param x: number to square root
//	return: square root of x
extern a3f64 a3sqrtdInverse(const a3f64 x);


// A3: Macros for generalizing square root based on active real type.
#if (defined A3_REAL_F64 || defined A3_REAL_F128)
#define a3sqrt					a3sqrtd
#define a3sqrtInverse			a3sqrtdInverse
#else	// !(defined A3_REAL_F64 || defined A3_REAL_F128)
#define a3sqrt					a3sqrtf
#define a3sqrtInverse			a3sqrtfInverse
#endif	// (defined A3_REAL_F64 || defined A3_REAL_F128)

// A3: Safe inverse square root.
#define a3sqrtSafeInverse(x)	( (x) != a3real_zero ? a3sqrtInverse(x) : a3real_zero )


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3sqrt_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_SQRT_H