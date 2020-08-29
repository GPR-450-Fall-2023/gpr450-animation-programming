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

	a3macros.h
	Common macros and frequently-used values that are made type-agnostic.
*/

#ifndef __ANIMAL3D_MACROS_H
#define __ANIMAL3D_MACROS_H


#include "a3config.h"


//-----------------------------------------------------------------------------
// CONSTANTS

// f64: double
#define __a3f64zero				0.0
#define __a3f64sixth			0.16666666666666666666666666666667
#define __a3f64fifth			0.2
#define __a3f64quarter			0.25
#define __a3f64third			0.33333333333333333333333333333333
#define __a3f64half				0.5
#define __a3f64twothirds		0.66666666666666666666666666666667
#define __a3f64threequarters	0.75
#define	__a3f64one				1.0
#define __a3f64fourthirds		1.3333333333333333333333333333333
#define __a3f64onehalf			1.5
#define __a3f64two				2.0
#define __a3f64three			3.0
#define __a3f64four				4.0
#define __a3f64five				5.0
#define __a3f64six				6.0
#define __a3f64onehundred		100.0
#define __a3f64sqrthalf			0.70710678118654752440084436210485
#define __a3f64sqrttwo			1.4142135623730950488016887242097
#define __a3f64sqrtthree		1.7320508075688772935274463415059
#define __a3f64twopi			6.283185307179586476925286766559
#define __a3f64pi				3.1415926535897932384626433832795
#define __a3f64halfpi			1.5707963267948966192313216916398
#define __a3f64thirdpi			1.0471975511965977461542144610932
#define __a3f64quarterpi		0.78539816339744830961566084581988
#define __a3f64threesixty		360.0
#define __a3f64oneeighty		180.0
#define __a3f64ninety			90.0
#define __a3f64sixty			60.0
#define __a3f64fortyfive		45.0
#define __a3f64epsilon			2.22e-16
#define __a3f64halfeps			1.11e-16
#define __a3f64deg2rad			0.01745329251994329576923690768489
#define __a3f64rad2deg			57.295779513082320876798154814105

// f128: long double
#define __a3f128zero			0.0##l
#define __a3f128sixth			0.16666666666666666666666666666667##l
#define __a3f128fifth			0.2##l
#define __a3f128quarter			0.25##l
#define __a3f128third			0.33333333333333333333333333333333##l
#define __a3f128half			0.5##l
#define __a3f128twothirds		0.66666666666666666666666666666667##l
#define __a3f128threequarters	0.75##l
#define	__a3f128one				1.0##l
#define __a3f128fourthirds		1.3333333333333333333333333333333##l
#define __a3f128onehalf			1.5##l
#define __a3f128two				2.0##l
#define __a3f128three			3.0##l
#define __a3f128four			4.0##l
#define __a3f128five			5.0##l
#define __a3f128six				6.0##l
#define __a3f128onehundred		100.0##l
#define __a3f128sqrthalf		0.70710678118654752440084436210485##l
#define __a3f128sqrttwo			1.4142135623730950488016887242097##l
#define __a3f128sqrtthree		1.7320508075688772935274463415059##l
#define __a3f128twopi			6.283185307179586476925286766559##l
#define __a3f128pi				3.1415926535897932384626433832795##l
#define __a3f128halfpi			1.5707963267948966192313216916398##l
#define __a3f128thirdpi			1.0471975511965977461542144610932##l
#define __a3f128quarterpi		0.78539816339744830961566084581988##l
#define __a3f128threesixty		360.0##l
#define __a3f128oneeighty		180.0##l
#define __a3f128ninety			90.0##l
#define __a3f128sixty			60.0##l
#define __a3f128fortyfive		45.0##l
#define __a3f128epsilon			1.93e-34##l
#define __a3f128halfeps			9.63e-35##l
#define __a3f128deg2rad			0.01745329251994329576923690768489##l
#define __a3f128rad2deg			57.295779513082320876798154814105##l

// f32: float
#define __a3f32zero				0.0##f
#define __a3f32sixth			0.16666666666666666666666666666667##f
#define __a3f32fifth			0.2##f
#define __a3f32quarter			0.25##f
#define __a3f32third			0.33333333333333333333333333333333##f
#define __a3f32half				0.5##f
#define __a3f32twothirds		0.66666666666666666666666666666667##f
#define __a3f32threequarters	0.75##f
#define	__a3f32one				1.0##f
#define __a3f32fourthirds		1.3333333333333333333333333333333##f
#define __a3f32onehalf			1.5##f
#define __a3f32two				2.0##f
#define __a3f32three			3.0##f
#define __a3f32four				4.0##f
#define __a3f32five				5.0##f
#define __a3f32six				6.0##f
#define __a3f32onehundred		100.0##f
#define __a3f32sqrthalf			0.70710678118654752440084436210485##f
#define __a3f32sqrttwo			1.4142135623730950488016887242097##f
#define __a3f32sqrtthree		1.7320508075688772935274463415059##f
#define __a3f32twopi			6.283185307179586476925286766559##f
#define __a3f32pi				3.1415926535897932384626433832795##f
#define __a3f32halfpi			1.5707963267948966192313216916398##f
#define __a3f32thirdpi			1.0471975511965977461542144610932##f
#define __a3f32quarterpi		0.78539816339744830961566084581988##f
#define __a3f32threesixty		360.0##f
#define __a3f32oneeighty		180.0##f
#define __a3f32ninety			90.0##f
#define __a3f32sixty			60.0##f
#define __a3f32fortyfive		45.0##f
#define __a3f32epsilon			1.19e-07##f
#define __a3f32halfeps			5.96e-08##f
#define __a3f32deg2rad			0.01745329251994329576923690768489##f
#define __a3f32rad2deg			57.295779513082320876798154814105##f



//-----------------------------------------------------------------------------
// OPERATIONS

// shorthand operations/quick functions
#define __a3isNearZeroF32(n)		( (n) >= -__a3f32epsilon  && (n) <= __a3f32epsilon  )
#define __a3isNearZeroF64(n)		( (n) >= -__a3f64epsilon  && (n) <= __a3f64epsilon  )
#define __a3isNearZeroF128(n)		( (n) >= -__a3f128epsilon && (n) <= __a3f128epsilon )

#define __a3isNotNearZeroF32(n)		( (n) < -__a3f32epsilon  || (n) > __a3f32epsilon  )
#define __a3isNotNearZeroF64(n)		( (n) < -__a3f64epsilon  || (n) > __a3f64epsilon  )
#define __a3isNotNearZeroF128(n)	( (n) < -__a3f128epsilon || (n) > __a3f128epsilon )

#define __a3isBetweenOnesF32(n)		( (n) > -__a3f32one  && (n) < __a3f32one  )
#define __a3isBetweenOnesF64(n)		( (n) > -__a3f64one  && (n) < __a3f64one  )
#define __a3isBetweenOnesF128(n)	( (n) > -__a3f128one && (n) < __a3f128one )

#define __a3absoluteF32(n)			( (n) >= __a3f32zero  ? (n) : -(n) )
#define __a3absoluteF64(n)			( (n) >= __a3f64zero  ? (n) : -(n) )
#define __a3absoluteF128(n)			( (n) >= __a3f128zero ? (n) : -(n) )

#define __a3divideF32(n, d)			( (d) != __a3f32zero  ? (n)/(d) : __a3f32zero  )
#define __a3divideF64(n, d)			( (d) != __a3f64zero  ? (n)/(d) : __a3f64zero  )
#define __a3divideF128(n, d)		( (d) != __a3f128zero ? (n)/(d) : __a3f128zero )

#define __a3recipF32(n)				( __a3f32one  / (n) )
#define __a3recipF64(n)				( __a3f64one  / (n) )
#define __a3recipF128(n)			( __a3f128one / (n) )

#define __a3recipsafeF32(n)			( (n) != __a3f32zero  ? __a3f32one  / (n) : __a3f32zero  )
#define __a3recipsafeF64(n)			( (n) != __a3f64zero  ? __a3f64one  / (n) : __a3f64zero  )
#define __a3recipsafeF128(n)		( (n) != __a3f128zero ? __a3f128one / (n) : __a3f128zero )

#define __a3serializeF32(n)			( ( (n) + __a3f32one  )*__a3f32half  )
#define __a3serializeF64(n)			( ( (n) + __a3f64one  )*__a3f64half  )
#define __a3serializeF128(n)		( ( (n) + __a3f128one )*__a3f128half )
#define __a3deserializeF32(n)		( (n)+(n) - __a3f32one  )
#define __a3deserializeF64(n)		( (n)+(n) - __a3f64one  )
#define __a3deserializeF128(n)		( (n)+(n) - __a3f128one )

#define __a3deg2radF32(n)			( (n)*__a3f32deg2rad  )
#define __a3deg2radF64(n)			( (n)*__a3f64deg2rad  )
#define __a3deg2radF128(n)			( (n)*__a3f128deg2rad )
#define __a3rad2degF32(n)			( (n)*__a3f32rad2deg  )
#define __a3rad2degF64(n)			( (n)*__a3f64rad2deg  )
#define __a3rad2degF128(n)			( (n)*__a3f128rad2deg )



//-----------------------------------------------------------------------------

// shorthand for selected default decimal type is being used as 'real'
#ifdef A3_REAL_F64
#define a3real_zero				__a3f64zero
#define a3real_sixth			__a3f64sixth
#define a3real_fifth			__a3f64fifth
#define a3real_quarter			__a3f64quarter
#define a3real_third			__a3f64third
#define a3real_half				__a3f64half
#define a3real_twothirds		__a3f64twothirds
#define a3real_threequarters	__a3f64threequarters
#define a3real_one				__a3f64one
#define a3real_fourthirds		__a3f64fourthirds
#define a3real_onehalf			__a3f64onehalf
#define a3real_two				__a3f64two
#define a3real_three			__a3f64three
#define a3real_four				__a3f64four
#define a3real_five				__a3f64five
#define a3real_six				__a3f64six
#define a3real_onehundred		__a3f64onehundred
#define a3real_sqrthalf			__a3f64sqrthalf
#define a3real_sqrttwo			__a3f64sqrttwo
#define a3real_sqrtthree		__a3f64sqrtthree
#define a3real_twopi			__a3f64twopi
#define a3real_pi				__a3f64pi
#define a3real_halfpi			__a3f64halfpi
#define a3real_thirdpi			__a3f64thirdpi
#define a3real_quarterpi		__a3f64quarterpi
#define a3real_threesixty		__a3f64threesixty
#define a3real_oneeighty		__a3f64oneeighty
#define a3real_ninety			__a3f64ninety
#define a3real_sixty			__a3f64sixty
#define a3real_fortyfive		__a3f64fortyfive
#define a3real_epsilon			__a3f64epsilon
#define a3real_halfepsilon		__a3f64halfeps
#define a3real_deg2rad			__a3f64deg2rad
#define a3real_rad2deg			__a3f64rad2deg
#else	// !A3_REAL_F64
#ifdef A3_REAL_F128
#define a3real_zero				__a3f128zero
#define a3real_sixth			__a3f128sixth
#define a3real_fifth			__a3f128fifth
#define a3real_quarter			__a3f128quarter
#define a3real_third			__a3f128third
#define a3real_half				__a3f128half
#define a3real_twothirds		__a3f128twothirds
#define a3real_threequarters	__a3f128threequarters
#define a3real_one				__a3f128one
#define a3real_fourthirds		__a3f128fourthirds
#define a3real_onehalf			__a3f128onehalf
#define a3real_two				__a3f128two
#define a3real_three			__a3f128three
#define a3real_four				__a3f128four
#define a3real_five				__a3f128five
#define a3real_six				__a3f128six
#define a3real_onehundred		__a3f128onehundred
#define a3real_sqrthalf			__a3f128sqrthalf
#define a3real_sqrttwo			__a3f128sqrttwo
#define a3real_sqrtthree		__a3f128sqrtthree
#define a3real_twopi			__a3f128twopi
#define a3real_pi				__a3f128pi
#define a3real_halfpi			__a3f128halfpi
#define a3real_thirdpi			__a3f128thirdpi
#define a3real_quarterpi		__a3f128quarterpi
#define a3real_threesixty		__a3f128threesixty
#define a3real_oneeighty		__a3f128oneeighty
#define a3real_ninety			__a3f128ninety
#define a3real_sixty			__a3f128sixty
#define a3real_fortyfive		__a3f128fortyfive
#define a3real_epsilon			__a3f128epsilon
#define a3real_halfepsilon		__a3f128halfeps
#define a3real_deg2rad			__a3f128deg2rad
#define a3real_rad2deg			__a3f128rad2deg
#else	// !A3_REAL_F128
#define a3real_zero				__a3f32zero
#define a3real_sixth			__a3f32sixth
#define a3real_fifth			__a3f32fifth
#define a3real_quarter			__a3f32quarter
#define a3real_third			__a3f32third
#define a3real_half				__a3f32half
#define a3real_twothirds		__a3f32twothirds
#define a3real_threequarters	__a3f32threequarters
#define a3real_one				__a3f32one
#define a3real_fourthirds		__a3f32fourthirds
#define a3real_onehalf			__a3f32onehalf
#define a3real_two				__a3f32two
#define a3real_three			__a3f32three
#define a3real_four				__a3f32four
#define a3real_five				__a3f32five
#define a3real_six				__a3f32six
#define a3real_onehundred		__a3f32onehundred
#define a3real_sqrthalf			__a3f32sqrthalf
#define a3real_sqrttwo			__a3f32sqrttwo
#define a3real_sqrtthree		__a3f32sqrtthree
#define a3real_twopi			__a3f32twopi
#define a3real_pi				__a3f32pi
#define a3real_halfpi			__a3f32halfpi
#define a3real_thirdpi			__a3f32thirdpi
#define a3real_quarterpi		__a3f32quarterpi
#define a3real_threesixty		__a3f32threesixty
#define a3real_oneeighty		__a3f32oneeighty
#define a3real_ninety			__a3f32ninety
#define a3real_sixty			__a3f32sixty
#define a3real_fortyfive		__a3f32fortyfive
#define a3real_epsilon			__a3f32epsilon
#define a3real_halfepsilon		__a3f32halfeps
#define a3real_deg2rad			__a3f32deg2rad
#define a3real_rad2deg			__a3f32rad2deg
#endif	// A3_REAL_F128
#endif	// A3_REAL_F64


// other shorthands not dependent on real type
// same functions as above
#define a3isNearZero(n)			( (n) >= -a3real_epsilon && (n) <= a3real_epsilon )
#define a3isNotNearZero(n)		( (n) <  -a3real_epsilon || (n) >  a3real_epsilon )
#define a3isBetweenOnes(n)		( (n) > -a3real_one && (n) < a3real_one )
#define a3absoluteInt(n)		( (n) >= 0 ? (n) : -(n) )
#define a3divideInt(n, d)		( (d) != 0 ? (n)/(d) : 0 )
#define a3absolute(n)			( (n) >= a3real_zero ? (n) : -(n) )
#define a3divide(n, d)			( (d) != a3real_zero ? (n)/(d) : a3real_zero )
#define a3recip(n)				( a3real_one / (n) )
#define a3recipsafe(n)			( (n) != a3real_zero ? a3real_one / (n) : a3real_zero )
#define a3serialize(n)			( ( (n) + a3real_one )*a3real_half  )
#define a3deserialize(n)		( (n)+(n) - a3real_one )
#define a3deg2rad(n)			( (n)*a3real_deg2rad  )
#define a3rad2deg(n)			( (n)*a3real_rad2deg )

// independent macros
#define a3maximum(n0, n1)		( (n0) > (n1) ? (n0) : (n1) )
#define a3minimum(n0, n1)		( (n0) < (n1) ? (n0) : (n1) )
#define a3clamp(n0, n1, v)		( (v) < (n1) ? (v) > (n0) ? (v) : (n0) : (n1) )
#define a3lerp(n0, n1, t)		( (n0) + ( (n1) - (n0) )*t )
#define a3unlerp(n0, n1, n)		( ( (n) - (n0) ) / ( (n1) - (n0) ) )
#define a3remap(m0, m1, n0, n1, n)	a3lerp(m0, m1, a3unlerp(n0, n1, n))
#define a3swap2(tmp,n0,n1)			tmp=n0;n0=n1;n1=tmp
#define a3swap3(tmp,n0,n1,n2)		tmp=n0;n0=n1;n1=n2;n2=tmp
#define a3swap4(tmp,n0,n1,n2,n3)	tmp=n0;n0=n1;n1=n2;n2=n3;n3=tmp

// tau: same as 2pi radians (360 degrees)
#define a3real_tau				a3real_twopi
#define a3real_tau_deg			a3real_threesixty

// compress/decompress: same as serialize/deserialize in this context
// compress maps range [-1,+1] to [0,1], uncompress does opposite
#define a3compress				a3serialize
#define a3uncompress			a3deserialize

// booleans
#define a3true					1
#define a3false					0
#ifndef __cplusplus
#define true					a3true
#define false					a3false
#endif	// !__cplusplus


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_MACROS_H