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

	a3types_real.h
	Shorthand real (floating point) type definitions and basic vector types.
*/

#ifndef __ANIMAL3D_TYPES_REAL_H
#define __ANIMAL3D_TYPES_REAL_H


#include "a3config.h"


//-----------------------------------------------------------------------------
// intrinsics include

#ifdef A3_USING_INTRIN
//-----------------------------------------------------------------------------
// unsupported feature message
#pragma message ("\
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n\
/// ****A3 WARNING**** User specified A3_USING_INTRIN not supported in current version; undefining INTRIN macro.            /// \n\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   ")
#undef A3_USING_INTRIN
//-----------------------------------------------------------------------------
#endif	// A3_USING_INTRIN


#ifdef A3_USING_INTRIN
#ifndef A3_INTRIN_INCLUDED
#define A3_INTRIN_INCLUDED
#include <xmmintrin.h>
#endif	// !A3_INTRIN_INCLUDED
#endif	// A3_USING_INTRIN


//-----------------------------------------------------------------------------


A3_BEGIN


//-----------------------------------------------------------------------------

// decimal types
typedef float				a3f32;
typedef double				a3f64;
typedef long double			a3f128;


// aliases for decimal (real) number types
#ifdef A3_USING_INTRIN
// f32 is real (float)
typedef a3f32				a3real;

// define vector base types and their pointers as intrinsic types
typedef __m64				a3real2[1];	// 2 floats
typedef __m128				a3real3[1];	// 3 floats + 1 pad
typedef __m128				a3real4[1]; // 4 floats

// define parameter (p), return (r) and const return (rk) types as pointers
typedef __m64				a3real2p[], *a3real2r;
typedef __m128				a3real3p[], *a3real3r;
typedef __m128				a3real4p[], *a3real4r;
typedef __m64	const		*a3real2rk;
typedef __m128	const		*a3real3rk;
typedef __m128	const		*a3real4rk;

// padding argument for 3D vector base
// used in places where the size of a 3D vector matters
// e.g. param in creating a 3D matrix
#define A3PAD(a,b)	,__pad##a##b 
#define A3PAD0		,a3real_zero 

#else	// !A3_USING_INTRIN
#ifdef A3_REAL_F64
// f64 is real (double)
typedef a3f64				a3real;
#else	// !A3_REAL_F64
#ifdef A3_REAL_F128
// f128 is real (long double)
typedef a3f128				a3real;
#else	// !A3_REAL_F128
// f32 is real (float)
typedef a3f32				a3real;
#endif	// A3_REAL_F128
#endif	// A3_REAL_F64

// define vector base types and their pointers as arrays of real
typedef a3real				a3real2[2], a3real3[3], a3real4[4];

// define parameter (p), return (r) and const return (rk) types as pointers
typedef a3real				a3real2p[], a3real3p[], a3real4p[], *a3real2r, *a3real3r, *a3real4r;
typedef a3real	const		*a3real2rk, *a3real3rk, *a3real4rk;

// padding argument is not required
#define A3PAD(a,b)	
#define A3PAD0		

#endif	// A3_USING_INTRIN

// multi-dimensional arrays for a variety of matrix base types and pointers
typedef a3real2				a3real2x2[2], a3real2x3[3], a3real2x4[4];
typedef a3real3				a3real3x2[2], a3real3x3[3], a3real3x4[4];
typedef a3real4				a3real4x2[2], a3real4x3[3], a3real4x4[4];

// param and return types
typedef a3real2				a3real2x2p[], a3real2x3p[], a3real2x4p[], *a3real2x2r, *a3real2x3r, *a3real2x4r;
typedef a3real3				a3real3x2p[], a3real3x3p[], a3real3x4p[], *a3real3x2r, *a3real3x3r, *a3real3x4r;
typedef a3real4				a3real4x2p[], a3real4x3p[], a3real4x4p[], *a3real4x2r, *a3real4x3r, *a3real4x4r;
typedef a3real2	const		*a3real2x2rk, *a3real2x3rk, *a3real2x4rk;
typedef a3real3	const		*a3real3x2rk, *a3real3x3rk, *a3real3x4rk;
typedef a3real4	const		*a3real4x2rk, *a3real4x3rk, *a3real4x4rk;


// standard definition of some function that takes in and returns a real number
typedef a3real(*a3realfunc)(a3real);


//-----------------------------------------------------------------------------


A3_END


#endif	// !__ANIMAL3D_TYPES_REAL_H