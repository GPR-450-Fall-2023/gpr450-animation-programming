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

	a3interpolation.h
	Declarations for basic interpolation algorithms.
*/

#ifndef __ANIMAL3D_A3DM_INTERPOLATION_H
#define __ANIMAL3D_A3DM_INTERPOLATION_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3macros.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------

// A3: Linear interpolation ("LERP"): calculate linear interpolation between 
//		reference values 'n0' and 'n1' using a interpolation parameter 'param'
//		(sometimes referred to as 't' parameter or value).
//	param n0: initial value, result when t=0
//	param n1: goal/end value, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: linearly interpolated value from n0 to n1 at t (param)
A3_INLINE a3real a3lerpFunc(const a3real n0, const a3real n1, const a3real param);

// A3: Inverse linear interpolation: given a known value 'value' and reference 
//		values 'n0' and 'n1' (same as above), compute the 't' parameter that 
//		would yield 'value' as a result if used as parameter in LERP.
//	param n0: initial value, result when t=0
//	param n1: goal/end value, result when t=1
//	param value: desired result of linear interpolation (lerp at t)
//	return: interpolation parameter (t) that would yield input value
A3_INLINE a3real a3lerpInverse(const a3real n0, const a3real n1, const a3real value);

// A3: Safe inverse linear interpolation; ensures input values are different.
//	param n0: initial value
//	param n1: goal/end value (not equal to n0)
//	param value: desired result of linear interpolation
//	return: interpolation parameter that would yield input value
A3_INLINE a3real a3lerpSafeInverse(const a3real n0, const a3real n1, const a3real value);

// A3: Remap value: calculate value contained in one range to another range.
//	param n0_new: initial value of new range
//	param n1_new: goal/end value of new range
//	param n0: initial value of original range
//	param n1: goal/end value of original range
//	param value: input to be remapped from original to new range
//	return: input value relative to new range
A3_INLINE a3real a3remapFunc(const a3real n0_new, const a3real n1_new, const a3real n0, const a3real n1, const a3real value);

// A3: Safe remap value; ensures input values are different.
//	param n0_new: initial value of new range
//	param n1_new: goal/end value of new range
//	param n0: initial value of original range
//	param n1: goal/end value of original range (not equal to n0)
//	param value: input to be remapped from original to new range
//	return: input value relative to new range
A3_INLINE a3real a3remapSafe(const a3real n0_new, const a3real n1_new, const a3real n0, const a3real n1, const a3real value);


// A3: Bi-linear interpolation: calculate lerp between each range of numbers, 
//		[n00, n01] and [n10, n11], and then calculate lerp between the results.
//	param n00: initial value of first range
//	param n01: goal/end value of first range
//	param n10: initial value of second range
//	param n11: goal/end value of second range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending results of lerp (bi-lerp)
//	return: interpolation between the results of interpolation within ranges
A3_INLINE a3real a3bilerp(const a3real n00, const a3real n01, const a3real n10, const a3real n11, const a3real param0, const a3real param1);

// A3: Tri-linear interpolation: calculate bi-lerp between each set of ranges, 
//		then lerp between the results.
//	param n000: initial value of first range
//	param n001: goal/end value of first range
//	param n010: initial value of second range
//	param n011: goal/end value of second range
//	param n100: initial value of third range
//	param n101: goal/end value of third range
//	param n110: initial value of fourth range
//	param n111: goal/end value of fourth range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending in sets 000-011 and 100-111 (bi-lerp)
//	param param2: parameter for blending results of bi-lerp (tri-lerp)
//	return: interpolation between results of bi-lerp within sets
A3_INLINE a3real a3trilerp(const a3real n000, const a3real n001, const a3real n010, const a3real n011, const a3real n100, const a3real n101, const a3real n110, const a3real n111, const a3real param0, const a3real param1, const a3real param2);


// A3: Catmull-Rom spline interpolation.
//	param nPrev: previous control value (before n0)
//	param n0: initial control value of curve segment
//	param n1: goal/end control value of curve segment
//	param nNext: next control value (after n1)
//	param param: interpolation parameter (t)
//		result at t=0 is n0, result at t=1 is n1
//	return: Catmull-Rom spline interpolation
A3_INLINE a3real a3CatmullRom(const a3real nPrev, const a3real n0, const a3real n1, const a3real nNext, const a3real param);


// A3: Cubic Hermite spline (c-spline) interpolation using control handles.
//	param n0: initial control value of curve segment
//	param n1: goal/end control value of curve segment
//	param nControl0: control handle corresponding to n0
//	param nControl1: control handle corresponding to n1
//	param param: interpolation parameter (t)
//		result at t=0 is n0, result at t=1 is n1
//	return: c-spline interpolation
A3_INLINE a3real a3HermiteControl(const a3real n0, const a3real n1, const a3real nControl0, const a3real nControl1, const a3real param);

// A3: Cubic Hermite spline (c-spline) interpolation using control tangents.
//	param n0: initial control value of curve segment
//	param n1: goal/end control value of curve segment
//	param nTangent0: control tangent corresponding to n0
//	param nTangent1: control tangent corresponding to n1
//	param param: interpolation parameter (t)
//		result at t=0 is n0, result at t=1 is n1
//	return: c-spline interpolation
A3_INLINE a3real a3HermiteTangent(const a3real n0, const a3real n1, const a3real nTangent0, const a3real nTangent1, const a3real param);


// A3: Bezier interpolation of order/degree zero (constant).
//	param n0: first control value
//	param param: interpolation parameter (t), not used
//	return: n0 (constant)
A3_INLINE a3real a3Bezier0(const a3real n0, const a3real param);

// A3: Bezier interpolation of first order/degree (linear).
//	param n0: first control value
//	param n1: second control value
//	param param: interpolation parameter (t), used for lerp; 
//		result at t=0 is n0, result at t=1 is n1
//	return: lerp(n0, n1, t)
A3_INLINE a3real a3Bezier1(const a3real n0, const a3real n1, const a3real param);

// A3: Bezier interpolation of second order/degree (quadratic).
//	param n0: first control value
//	param n1: second control value
//	param n2: third control value
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is n0, result at t=1 is n2
//	return: lerp(Bezier1(n0, n1, t), Bezier1(n1, n2, t), t)
A3_INLINE a3real a3Bezier2(const a3real n0, const a3real n1, const a3real n2, const a3real param);

// A3: Bezier interpolation of third order/degree (cubic).
//	param n0: first control value
//	param n1: second control value
//	param n2: third control value
//	param n3: fourth control value
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is n0, result at t=1 is n3
//	return: lerp(Bezier2(n0, n1, n2, t), Bezier2(n1, n2, n3, t), t)
A3_INLINE a3real a3Bezier3(const a3real n0, const a3real n1, const a3real n2, const a3real n3, const a3real param);

// A3: Recursive Bezier interpolation of higher order (quartic, etc.).
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param n: array of control values
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is n[0], result at t=1 is n[order_N]
//	return: recursive lerp through each order/degree of Bezier until zero
A3_INLINE a3real a3BezierN(a3count order_N, const a3real n[], const a3real param);


//-----------------------------------------------------------------------------
// A3: Utilities to calculate the arc length along a curved segment.
// returns the total arc length of the segment
// outputs the samples, sample parameters and arc lengths at samples to arrays
// number of elements in each array must be 'numDivisions' + 1
// user can also choose to auto-normalize parameters
// 'paramTable_out' is an optional input; 't' params will not be stored if null

// A3: Calculate arc length of a Catmull-Rom curve segment.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param nPrev: previous control value (before n0)
//	param n0: initial control value of curve segment
//	param n1: goal/end control value of curve segment
//	param nNext: next control value (after n1)
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3calculateArcLengthCatmullRom(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real nPrev, const a3real n0, const a3real n1, const a3real nNext);

// A3: Calculate arc length of a cubic Hermite curve segment with handles.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param n0: initial control value of curve segment
//	param n1: goal/end control value of curve segment
//	param nControl0: control handle corresponding to n0
//	param nControl1: control handle corresponding to n1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3calculateArcLengthHermiteControl(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real n0, const a3real n1, const a3real nControl0, const a3real nControl1);

// A3: Calculate arc length of a cubic Hermite curve segment with tangents.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param n0: initial control value of curve segment
//	param n1: goal/end control value of curve segment
//	param nControl0: control tangent corresponding to n0
//	param nControl1: control tangent corresponding to n1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3calculateArcLengthHermiteTangent(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real n0, const a3real n1, const a3real nTangent0, const a3real nTangent1);

// A3: Calculate arc length of order zero Bezier curve (constant); this is 
//		an odd one to make an appearance because it always returns zero, but 
//		it is here to be consistent with everything else having this option.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param n0: first control value
//	param param: interpolation parameter (t), not used
//	return: zero
A3_INLINE a3real a3calculateArcLengthBezier0(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real n0);

// A3: Calculate arc length of first order Bezier curve (linear).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param n0: first control value
//	param n1: second control value
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3calculateArcLengthBezier1(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real n0, const a3real n1);

// A3: Calculate arc length of second order Bezier curve (quadratic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param n0: first control value
//	param n1: second control value
//	param n2: third control value
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3calculateArcLengthBezier2(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real n0, const a3real n1, const a3real n2);

// A3: Calculate arc length of third order Bezier curve (cubic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param n0: first control value
//	param n1: second control value
//	param n2: third control value
//	param n3: fourth control value
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3calculateArcLengthBezier3(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real n0, const a3real n1, const a3real n2, const a3real n3);

// A3: Calculate arc length of higher-order Bezier curve (quartic, etc.).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param n: array of control values
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3calculateArcLengthBezierN(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3count order_N, const a3real n[]);


//-----------------------------------------------------------------------------
// A3: Utilities to sample from a table using linear interpolation, where 
//	'valueTable' is the table of sampled values, 
//	'paramTable' is the table of sampling parameters used, 
//	'param' is the input value we are searching for, and 
//	'i' is the a3index to start searching from
// assumes all values are valid (does not check)

// A3: Find index of parameter in table and approximate value by interpolating 
//		surrounding samples in table; assumes that parameter values increase 
//		as we step through the table (parameter increases as index increases); 
//		search continues until tabled parameter is greater than ref param.
//	param valueTable: array of values acquired using sampling function 
//		(e.g. one of the arc length calculators above)
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param i: starting search index; zero if starting from beginning of tables
//	return: approximated value interpolated at t in table
A3_INLINE a3real a3sampleTableLerpIncrement(const a3real valueTable[], const a3real paramTable[], const a3real param, a3index i);

// A3: Find index of parameter in table and approximate value by interpolating 
//		surrounding samples in table; assumes that parameter values decrease 
//		as we step through the table (parameter decreases as index increases);
//		search continues until tabled parameter is less than ref param.
//	param valueTable: array of values acquired using sampling function 
//		(e.g. one of the arc length calculators above)
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param i: starting search index; zero if starting from beginning of tables
//	return: approximated value interpolated at t in table
A3_INLINE a3real a3sampleTableLerpDecrement(const a3real valueTable[], const a3real paramTable[], const a3real param, a3index i);

// A3: Find index of parameter in table and approximate value by interpolating 
//		surrounding samples in table; assumes that parameter values increase 
//		as we step through the table (parameter increases as index increases); 
//		search continues until tabled parameter is greater than ref param; 
//		step through table using an interval instead of one-by-one.
//	param valueTable: array of values acquired using sampling function 
//		(e.g. one of the arc length calculators above)
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param step: number of elements to increment by each iteration
//	param i: starting search index; zero if starting from beginning of tables
//	return: approximated value interpolated at t in table
A3_INLINE a3real a3sampleTableLerpIncrementStep(const a3real valueTable[], const a3real paramTable[], const a3real param, const a3count step, a3index i);

// A3: Find index of parameter in table and approximate value by interpolating 
//		surrounding samples in table; assumes that parameter values decrease 
//		as we step through the table (parameter decreases as index increases);
//		search continues until tabled parameter is less than ref param; 
//		step through table using an interval instead of one-by-one.
//	param valueTable: array of values acquired using sampling function 
//		(e.g. one of the arc length calculators above)
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param step: number of elements to increment by each iteration
//	param i: starting search index; zero if starting from beginning of tables
//	return: approximated value interpolated at t in table
A3_INLINE a3real a3sampleTableLerpDecrementStep(const a3real valueTable[], const a3real paramTable[], const a3real param, const a3count step, a3index i);


// A3: Search for reference parameter in table, stop and get index when found; 
//		parameter increases as index increases, does not interpolate; index 
//		can be used to perform manual interplation, index returned would be 
//		that of 'n1' (goal/end value) in user's value table: index-1 is 'n0'
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param i: starting search index; zero if starting from beginning of tables
//	param param_out: optional pointer to value to capture interpolation param
//	return: index of goal/end value index in table
A3_INLINE a3index a3sampleTableLerpIncrementIndex(const a3real paramTable[], const a3real param, a3index i, a3real *param_out);

// A3: Search for reference parameter in table, stop and get index when found; 
//		parameter decreases as index increases, does not interpolate; index 
//		can be used to perform manual interplation, index returned would be 
//		that of 'n1' (goal/end value) in user's value table: index-1 is 'n0'
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param i: starting search index; zero if starting from beginning of tables
//	param param_out: optional pointer to value to capture interpolation param
//	return: index of goal/end value index in table
A3_INLINE a3index a3sampleTableLerpDecrementIndex(const a3real paramTable[], const a3real param, a3index i, a3real *param_out);

// A3: Search for reference parameter in table, stop and get index when found; 
//		parameter increases as index increases, does not interpolate; index 
//		can be used to perform manual interplation, index returned would be 
//		that of 'n1' (goal/end value) in user's value table: index-step is 'n0'
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param step: number of elements to increment by each iteration
//	param i: starting search index; zero if starting from beginning of tables
//	param param_out: optional pointer to value to capture interpolation param
//	return: index of goal/end value index in table
A3_INLINE a3index a3sampleTableLerpIncrementStepIndex(const a3real paramTable[], const a3real param, const a3count step, a3index i, a3real *param_out);

// A3: Search for reference parameter in table, stop and get index when found; 
//		parameter decreases as index increases, does not interpolate; index 
//		can be used to perform manual interplation, index returned would be 
//		that of 'n1' (goal/end value) in user's value table: index-step is 'n0'
//	param paramTable: array of interpolation parameters or other set of input 
//		values to search calculated using sampling function (e.g. arc length)
//	param param: reference interpolation parameter (t) or other input value to 
//		search for or approximate in paramTable
//	param step: number of elements to increment by each iteration
//	param i: starting search index; zero if starting from beginning of tables
//	param param_out: optional pointer to value to capture interpolation param
//	return: index of goal/end value index in table
A3_INLINE a3index a3sampleTableLerpDecrementStepIndex(const a3real paramTable[], const a3real param, const a3count step, a3index i, a3real *param_out);


// A3: Generate a sample table by sampling a user function returning a real 
//		and taking one real argument.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param numDivisions: number of intermediate samples on curve segment
//	param numSubdivisions: number of subsamples between samples
//	param paramMin: minimum parameter to user function (may not be [0, 1])
//	param paramMax: maximum parameter to user function (may not be [0, 1])
//	param func: pointer to user function returning real and taking one real arg
//	return: total number of samples (numDivisions*numSubdivisions + 1)
A3_INLINE a3index a3sampleTableGenerate(a3real sampleTable_out[], a3real paramTable_out[], const a3count numDivisions, const a3count numSubdivisions, const a3real paramMin, const a3real paramMax, const a3realfunc func);

// A3: Generate a sample table by sampling a user function returning a real 
//		and taking one real argument; also stores arc length at each sample.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param numDivisions: number of intermediate samples on curve segment
//	param numSubdivisions: number of subsamples between samples
//	param paramMin: minimum parameter to user function (may not be [0, 1])
//	param paramMax: maximum parameter to user function (may not be [0, 1])
//	param func: pointer to user function returning real and taking one real arg
//	return: total number of samples (numDivisions*numSubdivisions + 1)
A3_INLINE a3index a3sampleTableGenerateArcLength(a3real sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3count numSubdivisions, const a3real paramMin, const a3real paramMax, const a3realfunc func);

// A3: Get the number of samples (elements) needed to hold a user sample table.
//	param numDivisions: number of segment divisions/samples
//	param numSubdivisions: number of subsamples between samples
//	return: total number of samples (numDivisions*numSubdivisions + 1)
A3_INLINE a3index a3sampleTableGenerateNumSamplesRequired(const a3index numDivisions, const a3count numSubdivisions);


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3interpolation_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_INTERPOLATION_H