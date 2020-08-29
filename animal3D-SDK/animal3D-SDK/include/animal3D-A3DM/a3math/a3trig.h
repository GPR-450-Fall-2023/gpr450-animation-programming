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

	a3trig.h
	Declarations for fast trigonometry functions.
*/

#ifndef __ANIMAL3D_A3DM_TRIG_H
#define __ANIMAL3D_A3DM_TRIG_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3macros.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------

// A3: Initialize data tables for fast trigonometry.
//	param samplesPerDegree: number of divisions in a single degree (1 or more)
//	param table_out: externally-allocated, sufficiently-sized array to 
//		hold the function samples and params; total number of samples in each 
//		function will be samplesPerDegree x 720 + 1 samples for sin and cos, 
//		and the parameters will be retained, so the total number of elements in 
//		the table should be (samplesPerDegree x 720 + 1) x 3
//	return: number of values if success
//	return: 0 if fail
A3_INLINE a3index a3trigInit(const a3index samplesPerDegree, a3real table_out[]);

// A3: Set pointer to existing table and samples per degree.
//	param samplesPerDegree: number of divisions in a single degree (1 or more)
//	param table: externally-allocated array of pre-stored trig values 
//		(initialized using a3trigInit)
//	return: number of values if success
//	return: 0 if fail
A3_INLINE a3index a3trigInitSetTables(const a3index samplesPerDegree, const a3real table[]);

// A3: Check the number of samples that would be required for the trig table.
//	param samplesPerDegree: desired number of divisions per degree (1 or more)
//	return: number of values required (for pre-allocation)
A3_INLINE a3index a3trigInitSamplesRequired(const a3index samplesPerDegree);

// A3: Delete trig data tables.
//	return: 1 if success
//	return: 0 if fail
A3_INLINE a3index a3trigFree();


//-----------------------------------------------------------------------------
// validation functions: call before trig functions so inputs stay in range
// validate inputs to the functions below
// sin, cos, tan in degrees: input is [-360, +360]
// sin, cos, tan in radians: input is [-2pi, +2pi]
// asin, acos input is [-1, +1]
// atan input is (-pi/2, +pi/2)

// A3: Validate degree parameter for sine, cosine and tangent functions.
//	param x: input to function in degrees
//	return: param in valid range [-360, +360]
A3_INLINE a3real a3trigValid_sind(a3real x);

// A3: Validate radian parameter for sine, cosine and tangent functions.
//	param x: input to function in radians
//	return: param in valid range [-2pi, +2pi]
A3_INLINE a3real a3trigValid_sinr(a3real x);

// A3: Validate parameter for inverse sine and inverse cosine functions.
//	param x: input to function
//	return: param in valid range [-1, +1]
A3_INLINE a3real a3trigValid_asin(a3real x);

// A3: Validate parameter for inverse tangent function.
//	param x: input to function
//	return: param in valid range for maximum precision (-pi/2, +pi/2)
A3_INLINE a3real a3trigValid_atan(a3real x);


//-----------------------------------------------------------------------------
// trig and inverse-trig functions
// compute more precise trig functions by using interpolation
// sin, cos, tan assume input is [-360, +360] degrees or [-2pi, +2pi] radians, 
//	return value in [-1, +1] for sin, cos and (-inf, +inf) for tan
// arcsin assumes input is [-1, +1], 
//	returns [-90, +90] degrees or [-pi/2, +pi/2] radians
// arccos assumes input is [-1, +1], 
//	returns [0, +180] degrees or [0, +pi] radians
// arctan assumes input is (-inf, +inf), 
//	returns (-90, +90) degrees or (-pi/4, +pi/4) radians

// A3: Sine with degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	return: sin(x), range [-1, +1]
A3_INLINE a3real a3sind(const a3real x);

// A3: Cosine with degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	return: cos(x), range [-1, +1]
A3_INLINE a3real a3cosd(const a3real x);

// A3: Tangent with degree input (tan = sin/cos).
//	param x: input to function in degrees, domain [-360, +360]
//	return: tan(x), range (-inf, +inf)
A3_INLINE a3real a3tand(const a3real x);

// A3: Sine with radian input.
//	param x: input to function in radians, domain [-2pi, +2pi]
//	return: sin(x), range [-1, +1]
A3_INLINE a3real a3sinr(const a3real x);

// A3: Cosine with radian input.
//	param x: input to function in radians, domain [-2pi, +2pi]
//	return: cos(x), range [-1, +1]
A3_INLINE a3real a3cosr(const a3real x);

// A3: Tangent with radian input (tan = sin/cos).
//	param x: input to function in radians, domain [-2pi, +2pi]
//	return: tan(x), range (-inf, +inf)
A3_INLINE a3real a3tanr(const a3real x);


// A3: Inverse sine with degree output.
//	param x: input to function, domain [-1, +1]
//	return: asin(x) in degrees, range [-90, +90]
A3_INLINE a3real a3asind(const a3real x);

// A3: Inverse cosine with degree output.
//	param x: input to function, domain [-1, +1]
//	return: acos(x) in degrees, range [0, +180]
A3_INLINE a3real a3acosd(const a3real x);

// A3: Inverse tangent with degree output.
//	param x: input to function, domain (-inf, +inf)
//	return: atan(x) in degrees, range (-90, +90)
A3_INLINE a3real a3atand(const a3real x);

// A3: Inverse sine with radian output.
//	param x: input to function, domain [-1, +1]
//	return: asin(x) in radians, range [-pi/2, +pi/2]
A3_INLINE a3real a3asinr(const a3real x);

// A3: Inverse cosine with radian output.
//	param x: input to function, domain [-1, +1]
//	return: acos(x) in radians, range [0, +pi]
A3_INLINE a3real a3acosr(const a3real x);

// A3: Inverse tangent with radian output.
//	param x: input to function, domain (-inf, +inf)
//	return: atan(x) in radians, range [-pi/2, +pi/2]
A3_INLINE a3real a3atanr(const a3real x);


// A3: Cosecant with degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	return: 1/sin(x), range (-inf, -1] U [+1, +inf)
A3_INLINE a3real a3cscd(const a3real x);

// A3: Secant with degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	return: 1/cos(x), range (-inf, -1] U [+1, +inf)
A3_INLINE a3real a3secd(const a3real x);

// A3: Cotangent with degree input (cot = cos/sin).
//	param x: input to function in degrees, domain [-360, +360]
//	return: 1/tan(x), range (-inf, +inf)
A3_INLINE a3real a3cotd(const a3real x);

// A3: Cosecant with radian input.
//	param x: input to function in degrees, domain [-2pi, +2pi]
//	return: 1/sin(x), range (-inf, -1] U [+1, +inf)
A3_INLINE a3real a3cscr(const a3real x);

// A3: Secant with radian input.
//	param x: input to function in degrees, domain [-2pi, +2pi]
//	return: 1/cos(x), range (-inf, -1] U [+1, +inf)
A3_INLINE a3real a3secr(const a3real x);

// A3: Cotangent with degree input (cot = cos/sin).
//	param x: input to function in degrees, domain [-2pi, +2pi]
//	return: 1/tan(x), range (-inf, +inf)
A3_INLINE a3real a3cotr(const a3real x);


// A3: Inverse cosecant with degree output.
//	param x: input to function, domain (-inf, -1] U [+1, +inf)
//	return: acsc(x) in degrees, range (-90, +90)
A3_INLINE a3real a3acscd(const a3real x);

// A3: Inverse secant with degree output.
//	param x: input to function, domain (-inf, -1] U [+1, +inf)
//	return: asec(x) in degrees, range (0, +180)
A3_INLINE a3real a3asecd(const a3real x);

// A3: Inverse cotangent with degree output.
//	param x: input to function, domain (-inf, +inf)
//	return: acot(x) in degrees, range (-90, +90)
A3_INLINE a3real a3acotd(const a3real x);

// A3: Inverse cosecant with degree output.
//	param x: input to function, domain (-inf, -1] U [+1, +inf)
//	return: acsc(x) in degrees, range (-pi/2, +pi/2)
A3_INLINE a3real a3acscr(const a3real x);

// A3: Inverse secant with degree output.
//	param x: input to function, domain (-inf, -1] U [+1, +inf)
//	return: asec(x) in degrees, range (0, +pi)
A3_INLINE a3real a3asecr(const a3real x);

// A3: Inverse cotangent with degree output.
//	param x: input to function, domain (-inf, +inf)
//	return: acot(x) in degrees, range (-pi/2, +pi/2)
A3_INLINE a3real a3acotr(const a3real x);


// A3: Inverse tangent with degree output using two parameters; since 
//		tan(a) = y/x, breaks down inverse solution depending on y and x.
//	param y: numerator used to calculate tangent
//	param x: denominator used to calculate tangent
//	return: atan(y/x) in degrees, range (-90, +90)
A3_INLINE a3real a3atan2d(const a3real y, const a3real x);

// A3: Inverse tangent with radian output using two parameters; since 
//		tan(a) = y/x, breaks down inverse solution depending on y and x.
//	param y: numerator used to calculate tangent
//	param x: denominator used to calculate tangent
//	return: atan(y/x) in radians, range (-pi/2, +pi/2)
A3_INLINE a3real a3atan2r(const a3real y, const a3real x);


// A3: Set number of iterations in a Taylor series for solving trig functions.
//	param n: number of iterations
//	return: old number of iterations
A3_INLINE a3index a3trigSetTaylorIterations(const a3index n);

// A3: Get number of iterations in a Taylor series for solving trig functions.
//	return: number of iterations
A3_INLINE a3index a3trigGetTaylorIterations();


// A3: Calculate sine using a Taylor series given degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	return: sin(x), range [-1, +1]
A3_INLINE a3real a3sindTaylor(const a3real x);

// A3: Calculate cosine using a Taylor series given degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	return: cos(x), range [-1, +1]
A3_INLINE a3real a3cosdTaylor(const a3real x);

// A3: Calculate tangent using a Taylor series given degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	return: tan(x), range [-1, +1]
A3_INLINE a3real a3tandTaylor(const a3real x);

// A3: Calculate sine using a Taylor series given radian input.
//	param x: input to function in degrees, domain [-2pi, +2pi]
//	return: sin(x), range [-1, +1]
A3_INLINE a3real a3sinrTaylor(const a3real x);

// A3: Calculate cosine using a Taylor series given radian input.
//	param x: input to function in degrees, domain [-2pi, +2pi]
//	return: cos(x), range [-1, +1]
A3_INLINE a3real a3cosrTaylor(const a3real x);

// A3: Calculate tangent using a Taylor series given radian input.
//	param x: input to function in degrees, domain [-2pi, +2pi]
//	return: tan(x), range [-1, +1]
A3_INLINE a3real a3tanrTaylor(const a3real x);

// A3: Calculate sine and cosine using a Taylor series given degree input.
//	param x: input to function in degrees, domain [-360, +360]
//	param sin_out: pointer to value to store sine result, range [-1, +1]
//	param cos_out: pointer to value to store cosine result, range [-1, +1]
//	return: x (original input for reuse)
A3_INLINE a3real a3trigTaylor_sind_cosd(const a3real x, a3real *sin_out, a3real *cos_out);

// A3: Calculate sine and cosine using a Taylor series given radian input.
//	param x: input to function in radians, domain [-2pi, +2pi]
//	param sin_out: pointer to value to store sine result, range [-1, +1]
//	param cos_out: pointer to value to store cosine result, range [-1, +1]
//	return: x (original input for reuse)
A3_INLINE a3real a3trigTaylor_sinr_cosr(const a3real x, a3real *sin_out, a3real *cos_out);


//-----------------------------------------------------------------------------
// A3: Other trig-related operations: 
// compute the error ratio occurring from discrete geometry sampling
// e.g. since a sphere in graphics is not continuous, face centers 
//	are closer to the center than the vertices; calculate the 
//	uniform scale required to expand or contract geometry to match 
//	true continuous radii (application: volume lighting)

// A3: Calculate the size ratio of a real to discrete circle.
//	param totalAzimuth: arc coverage of circle in degrees, [0, 360]
//	param numSlices: number of slices in discrete circle
//	return: ratio of real to discrete circle size
A3_INLINE a3real a3trigPointToEdgeRatio(const a3real totalAzimuth, const a3count numSlices);

// A3: Calculate the size ratio of a discrete to real circle.
//	param totalAzimuth: arc coverage of circle in degrees, [0, 360]
//	param numSlices: number of slices in discrete circle
//	return: ratio of discrete to real circle size
A3_INLINE a3real a3trigEdgeToPointRatio(const a3real totalAzimuth, const a3count numSlices);

// A3: Calculate the size ratio of a real to discrete sphere.
//	param totalAzimuth: azimuth coverage of sphere in degrees, [0, 360]
//	param totalElevation: elevation coverage of sphere in degrees, [0, 180]
//	param numSlices: number of slices in discrete sphere (azimuth divisions)
//	param numStacks: number of stacks in discrete sphere (elevation divisions)
//	return: ratio of real to discrete sphere size
A3_INLINE a3real a3trigPointToFaceRatio(const a3real totalAzimuth, const a3real totalElevation, const a3count numSlices, const a3count numStacks);

// A3: Calculate the size ratio of a discrete to real sphere.
//	param totalAzimuth: azimuth coverage of sphere in degrees, [0, 360]
//	param totalElevation: elevation coverage of sphere in degrees, [0, 180]
//	param numSlices: number of slices in discrete sphere (azimuth divisions)
//	param numStacks: number of stacks in discrete sphere (elevation divisions)
//	return: ratio of discrete to real sphere size
A3_INLINE a3real a3trigFaceToPointRatio(const a3real totalAzimuth, const a3real totalElevation, const a3count numSlices, const a3count numStacks);


//-----------------------------------------------------------------------------
// A3: Trig-related macros for simplifying validation.
//	These should be called if input (x) is not writable, otherwise see below.

#define a3sindv(x)		a3sind(a3trigValid_sind(x))
#define a3cosdv(x)		a3cosd(a3trigValid_sind(x))
#define a3tandv(x)		a3tand(a3trigValid_sind(x))
#define a3sinrv(x)		a3sinr(a3trigValid_sinr(x))
#define a3cosrv(x)		a3cosr(a3trigValid_sinr(x))
#define a3tanrv(x)		a3tanr(a3trigValid_sinr(x))

#define a3asindv(x)		a3asind(a3trigValid_asin(x))
#define a3acosdv(x)		a3acosd(a3trigValid_asin(x))
#define a3atandv(x)		a3atand(a3trigValid_atan(x))
#define a3asinrv(x)		a3asinr(a3trigValid_asin(x))
#define a3acosrv(x)		a3acosr(a3trigValid_asin(x))
#define a3atanrv(x)		a3atanr(a3trigValid_atan(x))

#define a3cscdv(x)		a3cscd(a3trigValid_sind(x))
#define a3secdv(x)		a3secd(a3trigValid_sind(x))
#define a3cotdv(x)		a3cotd(a3trigValid_sind(x))
#define a3cscrv(x)		a3cscr(a3trigValid_sinr(x))
#define a3secrv(x)		a3secr(a3trigValid_sinr(x))
#define a3cotrv(x)		a3cotr(a3trigValid_sinr(x))

#define a3acscdv(x)		a3acscd(a3trigValid_asin(x))
#define a3asecdv(x)		a3asecd(a3trigValid_asin(x))
#define a3acotdv(x)		a3acotd(a3trigValid_atan(x))
#define a3acscrv(x)		a3acscr(a3trigValid_asin(x))
#define a3asecrv(x)		a3asecr(a3trigValid_asin(x))
#define a3acotrv(x)		a3acotr(a3trigValid_atan(x))


//-----------------------------------------------------------------------------
// A3: Trig-related macros for simplifying validation and overwriting input. 
//	These should only be called once if input (x) is writable and/or reusable.

#define a3sindvx(x)		a3sind(x = a3trigValid_sind(x))
#define a3cosdvx(x)		a3cosd(x = a3trigValid_sind(x))
#define a3tandvx(x)		a3tand(x = a3trigValid_sind(x))
#define a3sinrvx(x)		a3sinr(x = a3trigValid_sinr(x))
#define a3cosrvx(x)		a3cosr(x = a3trigValid_sinr(x))
#define a3tanrvx(x)		a3tanr(x = a3trigValid_sinr(x))

#define a3asindvx(x)	a3asind(x = a3trigValid_asin(x))
#define a3acosdvx(x)	a3acosd(x = a3trigValid_asin(x))
#define a3atandvx(x)	a3atand(x = a3trigValid_atan(x))
#define a3asinrvx(x)	a3asinr(x = a3trigValid_asin(x))
#define a3acosrvx(x)	a3acosr(x = a3trigValid_asin(x))
#define a3atanrvx(x)	a3atanr(x = a3trigValid_atan(x))

#define a3cscdvx(x)		a3cscd(x = a3trigValid_sind(x))
#define a3secdvx(x)		a3secd(x = a3trigValid_sind(x))
#define a3cotdvx(x)		a3cotd(x = a3trigValid_sind(x))
#define a3cscrvx(x)		a3cscr(x = a3trigValid_sinr(x))
#define a3secrvx(x)		a3secr(x = a3trigValid_sinr(x))
#define a3cotrvx(x)		a3cotr(x = a3trigValid_sinr(x))

#define a3acscdvx(x)	a3acscd(x = a3trigValid_asin(x))
#define a3asecdvx(x)	a3asecd(x = a3trigValid_asin(x))
#define a3acotdvx(x)	a3acotd(x = a3trigValid_atan(x))
#define a3acscrvx(x)	a3acscr(x = a3trigValid_asin(x))
#define a3asecrvx(x)	a3asecr(x = a3trigValid_asin(x))
#define a3acotrvx(x)	a3acotr(x = a3trigValid_atan(x))


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3trig_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_TRIG_H