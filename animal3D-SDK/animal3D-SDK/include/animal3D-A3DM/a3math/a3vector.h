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

	a3vector.h
	Declarations for vector functions.
*/

#ifndef __ANIMAL3D_A3DM_VECTOR_H
#define __ANIMAL3D_A3DM_VECTOR_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3macros.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------
// A3: Simple wrapper data structures/containers.
// these data structures ultimately serve as containers or aliases for simple 
//	arrays of a3real numbers/floats; all of the vector functions and operations 
//	below can be used to process the variable 'v' contained in each structure, 
//	which is ultimately just an array, or some other user-defined containers 
//	of vector data that hold an array

#ifndef __cplusplus
typedef union a3vec2 a3vec2;
typedef union a3vec3 a3vec3;
typedef union a3vec4 a3vec4;
#endif	// !__cplusplus

// A3: 2D vector container structure.
//	members x, s, r, v0: the first component
//	members y, t, g, v1: the second component
//	member v: raw data as array
union a3vec2
{
	// components
	struct { a3real x, y; };
	struct { a3real s, t; };
	struct { a3real r, g; };
	struct { a3real v0, v1; };

	// array (INPUT FOR OPERATIONS)
	a3real2 v;
};

// A3: 3D vector container structure.
//	members x, s, r, v0: the first component
//	members y, t, g, v1: the second component
//	members z, p, b, v2: the third component
//	member v: raw data as array
//	members xy, st, rg: first and second component subgroups
union a3vec3
{
	// components
	struct { a3real x, y, z; };
	struct { a3real s, t, p; };
	struct { a3real r, g, b; };
	struct { a3real v0, v1, v2; };

	// array (INPUT FOR OPERATIONS)
	a3real3 v;

	// partial swizzle
	a3vec2 xy;
	a3vec2 st;
	a3vec2 rg;
};

// A3: 4D vector container structure.
//	members x, s, r, v0: the first component
//	members y, t, g, v1: the second component
//	members z, p, b, v2: the third component
//	members w, q, a, v3: the fourth component
//	member v: raw data as array
//	members xyz, stp, rgb: first through third component subgroups
//	members xy, st, rg: first and second component subgroups
union a3vec4
{
	// components
	struct { a3real x, y, z, w; };
	struct { a3real s, t, p, q; };
	struct { a3real r, g, b, a; };
	struct { a3real v0, v1, v2, v3; };

	// array (INPUT FOR OPERATIONS)
	a3real4 v;

	// partial swizzle
	a3vec3 xyz;
	a3vec3 stp;
	a3vec3 rgb;
	a3vec2 xy;
	a3vec2 st;
	a3vec2 rg;
};


//-----------------------------------------------------------------------------
// A3: Function declarations using array types. All functions return a pointer 
//		the affected input/argument so the user may chain operations with it.
// for all functions that use other inputs to modify a vector, the 
//	result/output appears as the first parameter 'v_out'
// functions that modify a vector directly have a parameter 'v_inout'
// any vector used as input is labelled 'v' and may have some suffix
//	to give it a better description when needed

// general function descriptions (to avoid redundant comments): 
// --set: load values into a vector array
// --length squared: returns the squared length of a vector
// --length: returns the magnitude of a vector
//	*note: do not do length * length to get squared length as this is slow and 
//	redundant; use the squared length function to do this as it is optimized!
// --get unit/normalize: get the unit-length vector that points in the same 
//	direction of the input, or normalize the input to make it unit-length
//	*note: these return inverse length to avoid needing to re-calculate it
// --get negative/negate: get the negated input or negate the input
// --dot: the dot product of two vectors (x0*x1 + y0*y1 ...)
// --distance squared: returns the squared distance between two points
// --distance: returns the distance between two points
//	*note: see note for length, same idea
// --projection ratio: the ratio used to perform vector projection
// --projection: perform vector projection onto the base vector; essentially 
//	flattens the input against the base, so that the result has the same 
//	direction but a different length
//	*note: returns projection ratio to avoid needing to re-calculate
// --sum/add: compute the sum of two vectors; add stores result in left input
// --diff/sub: compute the difference, same idea for sub
// --product/mul: multiply vector by scalar s
// --quotient/div: divide vector by scalar s
// --product/mul components: multiply respective components of vectors
// --quotient/div components: divide respective components of vectors
// --all interpolation algorithms implemented for a3real numbers are also 
//	implemented for vectors (Catmull-Rom, Hermite, tables...)
// --Gram-Schmidt orthonormalization: using a base vector as reference, modify 
//	vector(s) so that they are perpendicular (orthogonal) to all the others
// --triangle area/doubled/squared/normal: calculate the area of a triangle 
//	formed by 3 points with modifiers; 3D has option to calculate normal
// --cross product: calculate a vector that is perpendicular to both inputs
//	*note: flipping the input yields a negated result (non-commutative)

//-----------------------------------------------------------------------------
// 2D VECTOR FUNCTIONS

// A3: Initialize vector with individual components.
//	param v_out: vector to initialize
//	param x: first component
//	param y: second component
//	return: v_out
A3_INLINE a3real2r a3real2Set(a3real2p v_out, const a3real x, const a3real y);

// A3: Initialize vector with a 2D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real2r a3real2SetReal2(a3real2p v_out, const a3real2p v);

// A3: Initialize vector with a 3D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real2r a3real2SetReal3(a3real2p v_out, const a3real3p v);

// A3: Initialize vector with 4D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real2r a3real2SetReal4(a3real2p v_out, const a3real4p v);


// A3: Calculate squared length (magnitude) of vector.
//	param v: input vector
//	return: squared length of vector: |v|^2
A3_INLINE a3real a3real2LengthSquared(const a3real2p v);

// A3: Calculate inverse (reciprocal) squared length (magnitude) of vector.
//	param v: input vector
//	return: inverse squared length of vector: 1 / |v|^2
A3_INLINE a3real a3real2LengthSquaredInverse(const a3real2p v);

// A3: Calculate length (magnitude) of vector.
//	param v: input vector
//	return: length of vector: |v|
A3_INLINE a3real a3real2Length(const a3real2p v);

// A3: Calculate inverse (reciprocal) length (magnitude) of vector.
//	param v: input vector
//	return: inverse length of vector: 1 / |v|
A3_INLINE a3real a3real2LengthInverse(const a3real2p v);

// A3: Get unit direction of vector.
//	param v_out: output vector, direction of input: v / |v|
//	param v: input vector
//	return: v_out
A3_INLINE a3real2r a3real2GetUnit(a3real2p v_out, const a3real2p v);

// A3: Normalize vector, making it unit length (direction).
//	param v_inout: vector to normalize: v /= |v|
//	return: v_inout
A3_INLINE a3real2r a3real2Normalize(a3real2p v_inout);

// A3: Get unit direction of vector and inverse length.
//	param v_out: output vector, direction of input: v / |v|
//	param v: input vector
//	param invLength_out: pointer to variable capturing inverse length: 1 / |v|
//	return: v_out
A3_INLINE a3real2r a3real2GetUnitInvLength(a3real2p v_out, const a3real2p v, a3real *invLength_out);

// A3: Normalize vector, making it unit length (direction), get inverse length.
//	param v_inout: vector to normalize: v /= |v|
//	param invLength_out: pointer to variable capturing inverse length: 1 / |v|
//	return: v_inout
A3_INLINE a3real2r a3real2NormalizeGetInvLength(a3real2p v_inout, a3real *invLength_out);

// A3: Get negative vector.
//	param v_out: output vector, negative of input: -v
//	param v: input vector
//	return: v_out
A3_INLINE a3real2r a3real2GetNegative(a3real2p v_out, const a3real2p v);

// A3: Negate vector.
//	param v_inout: vector to negate: v = -v
//	return: v_inout
A3_INLINE a3real2r a3real2Negate(a3real2p v_inout);


// A3: Calculate dot product (relative alignment) of two vectors.
//	param vL: input left side vector
//	param vR: input right side vector
//	return: dot product: vL dot vR
A3_INLINE a3real a3real2Dot(const a3real2p vL, const a3real2p vR);

// A3: Calculate squared distance between two vectors.
//	param v0: input first vector
//	param v1: input second vector
//	return: squared distance between vectors: |v1 - v0|^2
A3_INLINE a3real a3real2DistanceSquared(const a3real2p v0, const a3real2p v1);

// A3: Calculate distance between two vectors.
//	param v0: input first vector
//	param v1: input second vector
//	return: distance between vectors: |v1 - v0|
A3_INLINE a3real a3real2Distance(const a3real2p v0, const a3real2p v1);

// A3: Calculate size ratio of vector projection.
//	param v: input vector to project
//	param vBase: vector to project onto
//	return: ratio of projected vector length to base vector length: 
//		(v dot vBase) / |vBase|^2
A3_INLINE a3real a3real2ProjRatio(const a3real2p v, const a3real2p vBase);

// A3: Calculate vector projection.
//	param v_out: output of vector projection: v projected onto vBase
//	param v: input vector to project
//	param vBase: vector to project onto
//	return: v_out
A3_INLINE a3real2r a3real2Projected(a3real2p v_out, const a3real2p v, const a3real2p vBase);

// A3: Project vector.
//	param v_inout: vector to project
//	param vBase: vector to project onto
//	return: v_inout
A3_INLINE a3real2r a3real2Proj(a3real2p v_inout, const a3real2p vBase);

// A3: Calculate vector projection and get projection ratio.
//	param v_out: output of vector projection: v projected onto vBase
//	param v: input vector to project
//	param vBase: vector to project onto
//	param ratio_out: pointer to variable capturing projection ratio: 
//		(v dot vBase) / |vBase|^2
//	return: v_out
A3_INLINE a3real2r a3real2ProjectedGetRatio(a3real2p v_out, const a3real2p v, const a3real2p vBase, a3real *ratio_out);

// A3: Project vector and get projection ratio.
//	param v_inout: vector to project
//	param vBase: vector to project onto
//	param ratio_out: pointer to variable capturing projection ratio: 
//		(v dot vBase) / |vBase|^2
//	return: v_inout
A3_INLINE a3real2r a3real2ProjGetRatio(a3real2p v_inout, const a3real2p vBase, a3real *ratio_out);

// A3: Calculate sum of vectors.
//	param v_out: output vector to store sum of two inputs
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real2r a3real2Sum(a3real2p v_out, const a3real2p vL, const a3real2p vR);

// A3: Calculate difference of vectors.
//	param v_out: output vector to store difference of two inputs
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real2r a3real2Diff(a3real2p v_out, const a3real2p vL, const a3real2p vR);

// A3: Calculate product of vector and scalar.
//	param v_out: output vector to store scaled vector
//	param v: input vector to be scaled
//	param s: scalar by which to multiply vector
//	return: v_out
A3_INLINE a3real2r a3real2ProductS(a3real2p v_out, const a3real2p v, const a3real s);

// A3: Calculate quotient of vector and scalar.
//	param v_out: output vector to store scaled vector
//	param v: input vector to be scaled, numerator
//	param s: scalar by which to divide vector, denominator
//	return: v_out
A3_INLINE a3real2r a3real2QuotientS(a3real2p v_out, const a3real2p v, const a3real s);

// A3: Calculate component-wise product of two vectors.
//	param v_out: output vector to store product vector: vL * vR components
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real2r a3real2ProductComp(a3real2p v_out, const a3real2p vL, const a3real2p vR);

// A3: Calculate component-wise quotient of two vectors.
//	param v_out: output vector to store quotient vector: vL / vR components
//	param vL: input left side vector, numerator
//	param vR: input right side vector, denominator
//	return: v_out
A3_INLINE a3real2r a3real2QuotientComp(a3real2p v_out, const a3real2p vL, const a3real2p vR);

// A3: Add vectors and store to left operand.
//	param vL_inout: left side vector to which to add
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real2r a3real2Add(a3real2p vL_inout, const a3real2p vR);

// A3: Subtract vectors and store to left operand.
//	param vL_inout: left side vector from which to subtract
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real2r a3real2Sub(a3real2p vL_inout, const a3real2p vR);

// A3: Multiply vector by scalar.
//	param v_inout: vector to scale
//	param s: scalar by which to multiply vector
//	return: v_inout
A3_INLINE a3real2r a3real2MulS(a3real2p v_inout, const a3real s);

// A3: Divide vector by scalar.
//	param v_inout: vector to scale, numerator
//	param s: scalar by which to multiply vector, denominator
//	return: v_inout
A3_INLINE a3real2r a3real2DivS(a3real2p v_inout, const a3real s);

// A3: Component-wise product of two vectors, store in left operand.
//	param vL_inout: left side vector to multiply and store: vL *= vR components
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real2r a3real2MulComp(a3real2p vL_inout, const a3real2p vR);

// A3: Component-wise quotient of two vectors, store in left operand.
//	param vL_inout: left side vector to divide and store: vL /= vR components
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real2r a3real2DivComp(a3real2p vL_inout, const a3real2p vR);


// A3: Linear interpolation ("LERP"): calculate linear interpolation between 
//		reference vectors 'v0' and 'v1' using a interpolation parameter 'param'
//		(sometimes referred to as 't' parameter or value).
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real2r a3real2Lerp(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real param);

// A3: Normalized linear interpolation ("NLERP"): calculate lerp between 
//		reference vectors 'v0' and 'v1' using a interpolation parameter 
//		'param' (sometimes referred to as 't' parameter or value), and 
//		normalize the result.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real2r a3real2NLerp(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real param);

// A3: Bi-linear interpolation: calculate lerp between each range of numbers, 
//		[v00, v01] and [v10, v11], and then calculate lerp between the results.
//	param v_out: output interpolated vector
//	param v00: initial vector of first range
//	param v01: goal/end vector of first range
//	param v10: initial vector of second range
//	param v11: goal/end vector of second range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending results of lerp (bi-lerp)
//	return: v_out
A3_INLINE a3real2r a3real2Bilerp(a3real2p v_out, const a3real2p v00, const a3real2p v01, const a3real2p v10, const a3real2p v11, const a3real param0, const a3real param1);

// A3: Tri-linear interpolation: calculate bi-lerp between each set of ranges, 
//		then lerp between the results.
//	param v_out: output interpolated vector
//	param v000: initial vector of first range
//	param v001: goal/end vector of first range
//	param v010: initial vector of second range
//	param v011: goal/end vector of second range
//	param v100: initial vector of third range
//	param v101: goal/end vector of third range
//	param v110: initial vector of fourth range
//	param v111: goal/end vector of fourth range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending in sets 000-011 and 100-111 (bi-lerp)
//	param param2: parameter for blending results of bi-lerp (tri-lerp)
//	return: v_out
A3_INLINE a3real2r a3real2Trilerp(a3real2p v_out, const a3real2p v000, const a3real2p v001, const a3real2p v010, const a3real2p v011, const a3real2p v100, const a3real2p v101, const a3real2p v110, const a3real2p v111, const a3real param0, const a3real param1, const a3real param2);

// A3: Catmull-Rom spline interpolation.
//	param v_out: output interpolated vector
//	param vPrev: previous control vector (before v0)
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vNext: next control vector (after v1)
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real2r a3real2CatmullRom(a3real2p v_out, const a3real2p vPrev, const a3real2p v0, const a3real2p v1, const a3real2p vNext, const a3real param);

// A3: Cubic Hermite spline (c-spline) interpolation using control handles.
//	param v_out: output interpolated vector
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control handle corresponding to v0
//	param vControl1: control handle corresponding to v1
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real2r a3real2HermiteControl(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real2p vControl0, const a3real2p vControl1, const a3real param);

// A3: Cubic Hermite spline (c-spline) interpolation using control tangents.
//	param v_out: output interpolated vector
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param nTangent0: control tangent corresponding to v0
//	param nTangent1: control tangent corresponding to v1
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real2r a3real2HermiteTangent(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real2p vTangent0, const a3real2p vTangent1, const a3real param);

// A3: Bezier interpolation of order/degree zero (constant).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param param: interpolation parameter (t), not used
//	return: v_out
A3_INLINE a3real2r a3real2Bezier0(a3real2p v_out, const a3real2p v0, const a3real param);

// A3: Bezier interpolation of first order/degree (linear).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param param: interpolation parameter (t), used for lerp; 
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real2r a3real2Bezier1(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real param);

// A3: Bezier interpolation of second order/degree (quadratic).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v0, result at t=1 is v2
//	return: v_out
A3_INLINE a3real2r a3real2Bezier2(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real2p v2, const a3real param);

// A3: Bezier interpolation of third order/degree (cubic).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param v3: fourth control vector
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v0, result at t=1 is v3
//	return: v_out
A3_INLINE a3real2r a3real2Bezier3(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real2p v2, const a3real2p v3, const a3real param);

// A3: Recursive Bezier interpolation of higher order (quartic, etc.).
//	param v_out: output interpolated vector
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param v: array of control vectors
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v[0], result at t=1 is v[order_N]
//	return: v_out
A3_INLINE a3real2r a3real2BezierN(a3real2p v_out, a3count order_N, const a3real2 v[], const a3real param);

// A3: Spherical linear interpolation ("SLERP"): interpolation along the arc 
//		with a uniform rate of change; improves on nlerp which yields the same 
//		results but does not have a uniform rate of change.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real2r a3real2Slerp(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real param);

// A3: Spherical linear interpolation ("SLERP"): interpolation along the arc 
//		with a uniform rate of change; improves on nlerp which yields the same 
//		results but does not have a uniform rate of change; this version 
//		assumes prior knowledge that the inputs are unit length.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real2r a3real2SlerpUnit(a3real2p v_out, const a3real2p v0, const a3real2p v1, const a3real param);

// A3: Calculate arc length of a Catmull-Rom curve segment.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param vPrev: previous control vector (before v0)
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vNext: next control vector (after v1)
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthCatmullRom(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p vPrev, const a3real2p v0, const a3real2p v1, const a3real2p vNext);

// A3: Calculate arc length of a cubic Hermite curve segment with handles.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control handle corresponding to v0
//	param vControl1: control handle corresponding to v1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthHermiteControl(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0, const a3real2p v1, const a3real2p vControl0, const a3real2p vControl1);

// A3: Calculate arc length of a cubic Hermite curve segment with tangents.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control tangent corresponding to v0
//	param vControl1: control tangent corresponding to v1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthHermiteTangent(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0, const a3real2p v1, const a3real2p vTangent0, const a3real2p vTangent1);

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
//	param v0: first control vector
//	param param: interpolation parameter (t), not used
//	return: zero
A3_INLINE a3real a3real2CalculateArcLengthBezier0(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0);

// A3: Calculate arc length of first order Bezier curve (linear).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthBezier1(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0, const a3real2p v1);

// A3: Calculate arc length of second order Bezier curve (quadratic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthBezier2(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0, const a3real2p v1, const a3real2p v2);

// A3: Calculate arc length of third order Bezier curve (cubic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param v3: fourth control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthBezier3(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0, const a3real2p v1, const a3real2p v2, const a3real2p v3);

// A3: Calculate arc length of higher-order Bezier curve (quartic, etc.).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param v: array of control vectors
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthBezierN(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3count order_N, const a3real2 v[]);

// A3: Calculate arc length of a slerp curve segment.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthSlerp(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0, const a3real2p v1);

// A3: Calculate arc length of a slerp curve segment with unit length inputs.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real2CalculateArcLengthSlerpUnit(a3real2 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real2p v0, const a3real2p v1);


// A3: Gram-Schmidt orthogonalization: flatten the input vector so that it 
//		becomes perpendicular to the base vector; useful for calculating 
//		transformation basis vectors.
//	param v_out: result of Gram-Schmidt
//	param v: vector to orthogonalize
//	param vBase: fixed base vector to orthogonalize against
//	return: v_out
A3_INLINE a3real2r a3real2GramSchmidtOrtho(a3real2p v_out, const a3real2p v, const a3real2p vBase);

// A3: Gram-Schmidt orthogonalize.
//	param v_inout: vector to orthogonalize and store
//	param vBase: fixed base vector to orthogonalize against
//	return: v_out
A3_INLINE a3real2r a3real2GramSchmidt(a3real2p v_inout, const a3real2p vBase);


// A3: 2D cross product: get the signed length of the perpendicular axis (Z).
//	param vL: input left vector
//	param vR: input right vector
//	return: length of perpendicular axis, also area of parallelogram
A3_INLINE a3real a3real2CrossZ(const a3real2p vL, const a3real2p vR);

// A3: Calculate double area of a triangle.
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: double area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real2TriangleAreaDoubled(const a3real2p v0, const a3real2p v1, const a3real2p v2);

// A3: Calculate squared area of a triangle.
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: squared area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real2TriangleAreaSquared(const a3real2p v0, const a3real2p v1, const a3real2p v2);

// A3: Calculate area of a triangle.
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real2TriangleArea(const a3real2p v0, const a3real2p v1, const a3real2p v2);


// A3: Test whether point is in triangle.
//	param p: point to test whether contained in triangle
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: boolean describing whether triangle is in triangle
A3_INLINE a3boolean a3real2PointIsInTriangle(const a3real2p p, const a3real2p v0, const a3real2p v1, const a3real2p v2);

// A3: Test whether point is in triangle and get barycentric coordinates.
//	param p: point to test whether contained in triangle
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	param param0: pointer to variable to capture first barycentric coordinate
//	param param1: pointer to variable to capture second barycentric coordinate
//	param param2: pointer to variable to capture third barycentric coordinate
//	return: boolean describing whether triangle is in triangle
A3_INLINE a3boolean a3real2PointIsInTriangleBarycentric(const a3real2p p, const a3real2p v0, const a3real2p v1, const a3real2p v2, a3real *param0, a3real *param1, a3real *param2);


//-----------------------------------------------------------------------------
// 3D VECTOR FUNCTIONS

// A3: Initialize vector with individual components.
//	param v_out: vector to initialize
//	param x: first component
//	param y: second component
//	param z: third component
//	return: v_out
A3_INLINE a3real3r a3real3Set(a3real3p v_out, const a3real x, const a3real y, const a3real z);

// A3: Initialize vector with a 2D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real3r a3real3SetReal2(a3real3p v_out, const a3real2p v);

// A3: Initialize vector with a 2D vector and a third component.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	param z: third component
//	return: v_out
A3_INLINE a3real3r a3real3SetReal2Z(a3real3p v_out, const a3real2p v, const a3real z);

// A3: Initialize vector with a 3D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real3r a3real3SetReal3(a3real3p v_out, const a3real3p v);

// A3: Initialize vector with a 4D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real3r a3real3SetReal4(a3real3p v_out, const a3real4p v);


// A3: Calculate squared length (magnitude) of vector.
//	param v: input vector
//	return: squared length of vector: |v|^2
A3_INLINE a3real a3real3LengthSquared(const a3real3p v);

// A3: Calculate inverse (reciprocal) squared length (magnitude) of vector.
//	param v: input vector
//	return: inverse squared length of vector: 1 / |v|^2
A3_INLINE a3real a3real3LengthSquaredInverse(const a3real3p v);

// A3: Calculate length (magnitude) of vector.
//	param v: input vector
//	return: length of vector: |v|
A3_INLINE a3real a3real3Length(const a3real3p v);

// A3: Calculate inverse (reciprocal) length (magnitude) of vector.
//	param v: input vector
//	return: inverse length of vector: 1 / |v|
A3_INLINE a3real a3real3LengthInverse(const a3real3p v);

// A3: Get unit direction of vector.
//	param v_out: output vector, direction of input: v / |v|
//	param v: input vector
//	return: v_out
A3_INLINE a3real3r a3real3GetUnit(a3real3p v_out, const a3real3p v);

// A3: Normalize vector, making it unit length (direction).
//	param v_inout: vector to normalize: v /= |v|
//	return: v_inout
A3_INLINE a3real3r a3real3Normalize(a3real3p v_inout);

// A3: Get unit direction of vector and inverse length.
//	param v_out: output vector, direction of input: v / |v|
//	param v: input vector
//	param invLength_out: pointer to variable capturing inverse length: 1 / |v|
//	return: v_out
A3_INLINE a3real3r a3real3GetUnitInvLength(a3real3p v_out, const a3real3p v, a3real *invLength_out);

// A3: Normalize vector, making it unit length (direction), get inverse length.
//	param v_inout: vector to normalize: v /= |v|
//	param invLength_out: pointer to variable capturing inverse length: 1 / |v|
//	return: v_inout
A3_INLINE a3real3r a3real3NormalizeGetInvLength(a3real3p v_inout, a3real *invLength_out);

// A3: Get negative vector.
//	param v_out: output vector, negative of input: -v
//	param v: input vector
//	return: v_out
A3_INLINE a3real3r a3real3GetNegative(a3real3p v_out, const a3real3p v);

// A3: Negate vector.
//	param v_inout: vector to negate: v = -v
//	return: v_inout
A3_INLINE a3real3r a3real3Negate(a3real3p v_inout);


// A3: Calculate dot product (relative alignment) of two vectors.
//	param vL: input left side vector
//	param vR: input right side vector
//	return: dot product: vL dot vR
A3_INLINE a3real a3real3Dot(const a3real3p vL, const a3real3p vR);

// A3: Calculate squared distance between two vectors.
//	param v0: input first vector
//	param v1: input second vector
//	return: squared distance between vectors: |v1 - v0|^2
A3_INLINE a3real a3real3DistanceSquared(const a3real3p v0, const a3real3p v1);

// A3: Calculate distance between two vectors.
//	param v0: input first vector
//	param v1: input second vector
//	return: distance between vectors: |v1 - v0|
A3_INLINE a3real a3real3Distance(const a3real3p v0, const a3real3p v1);

// A3: Calculate size ratio of vector projection.
//	param v: input vector to project
//	param vBase: vector to project onto
//	return: ratio of projected vector length to base vector length: 
//		(v dot vBase) / |vBase|^2
A3_INLINE a3real a3real3ProjRatio(const a3real3p v, const a3real3p vBase);

// A3: Calculate vector projection.
//	param v_out: output of vector projection: v projected onto vBase
//	param v: input vector to project
//	param vBase: vector to project onto
//	return: v_out
A3_INLINE a3real3r a3real3Projected(a3real3p v_out, const a3real3p v, const a3real3p vBase);

// A3: Project vector.
//	param v_inout: vector to project
//	param vBase: vector to project onto
//	return: v_inout
A3_INLINE a3real3r a3real3Proj(a3real3p v_inout, const a3real3p vBase);

// A3: Calculate vector projection and get projection ratio.
//	param v_out: output of vector projection: v projected onto vBase
//	param v: input vector to project
//	param vBase: vector to project onto
//	param ratio_out: pointer to variable capturing projection ratio: 
//		(v dot vBase) / |vBase|^2
//	return: v_out
A3_INLINE a3real3r a3real3ProjectedGetRatio(a3real3p v_out, const a3real3p v, const a3real3p vBase, a3real *ratio_out);

// A3: Project vector and get projection ratio.
//	param v_inout: vector to project
//	param vBase: vector to project onto
//	param ratio_out: pointer to variable capturing projection ratio: 
//		(v dot vBase) / |vBase|^2
//	return: v_inout
A3_INLINE a3real3r a3real3ProjGetRatio(a3real3p v_inout, const a3real3p vBase, a3real *ratio_out);

// A3: Calculate sum of vectors.
//	param v_out: output vector to store sum of two inputs
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real3r a3real3Sum(a3real3p v_out, const a3real3p vL, const a3real3p vR);

// A3: Calculate difference of vectors.
//	param v_out: output vector to store difference of two inputs
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real3r a3real3Diff(a3real3p v_out, const a3real3p vL, const a3real3p vR);

// A3: Calculate product of vector and scalar.
//	param v_out: output vector to store scaled vector
//	param v: input vector to be scaled
//	param s: scalar by which to multiply vector
//	return: v_out
A3_INLINE a3real3r a3real3ProductS(a3real3p v_out, const a3real3p v, const a3real s);

// A3: Calculate quotient of vector and scalar.
//	param v_out: output vector to store scaled vector
//	param v: input vector to be scaled, numerator
//	param s: scalar by which to divide vector, denominator
//	return: v_out
A3_INLINE a3real3r a3real3QuotientS(a3real3p v_out, const a3real3p v, const a3real s);

// A3: Calculate component-wise product of two vectors.
//	param v_out: output vector to store product vector: vL * vR components
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real3r a3real3ProductComp(a3real3p v_out, const a3real3p vL, const a3real3p vR);

// A3: Calculate component-wise quotient of two vectors.
//	param v_out: output vector to store quotient vector: vL / vR components
//	param vL: input left side vector, numerator
//	param vR: input right side vector, denominator
//	return: v_out
A3_INLINE a3real3r a3real3QuotientComp(a3real3p v_out, const a3real3p vL, const a3real3p vR);

// A3: Add vectors and store to left operand.
//	param vL_inout: left side vector to which to add
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real3r a3real3Add(a3real3p vL_inout, const a3real3p vR);

// A3: Subtract vectors and store to left operand.
//	param vL_inout: left side vector from which to subtract
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real3r a3real3Sub(a3real3p vL_inout, const a3real3p vR);

// A3: Multiply vector by scalar.
//	param v_inout: vector to scale
//	param s: scalar by which to multiply vector
//	return: v_inout
A3_INLINE a3real3r a3real3MulS(a3real3p v_inout, const a3real s);

// A3: Divide vector by scalar.
//	param v_inout: vector to scale, numerator
//	param s: scalar by which to multiply vector, denominator
//	return: v_inout
A3_INLINE a3real3r a3real3DivS(a3real3p v_inout, const a3real s);

// A3: Component-wise product of two vectors, store in left operand.
//	param vL_inout: left side vector to multiply and store: vL *= vR components
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real3r a3real3MulComp(a3real3p vL_inout, const a3real3p vR);

// A3: Component-wise quotient of two vectors, store in left operand.
//	param vL_inout: left side vector to divide and store: vL /= vR components
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real3r a3real3DivComp(a3real3p vL_inout, const a3real3p vR);


// A3: Linear interpolation ("LERP"): calculate linear interpolation between 
//		reference vectors 'v0' and 'v1' using a interpolation parameter 'param'
//		(sometimes referred to as 't' parameter or value).
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real3r a3real3Lerp(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real param);

// A3: Normalized linear interpolation ("NLERP"): calculate lerp between 
//		reference vectors 'v0' and 'v1' using a interpolation parameter 
//		'param' (sometimes referred to as 't' parameter or value), and 
//		normalize the result.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real3r a3real3NLerp(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real param);

// A3: Bi-linear interpolation: calculate lerp between each range of numbers, 
//		[v00, v01] and [v10, v11], and then calculate lerp between the results.
//	param v_out: output interpolated vector
//	param v00: initial vector of first range
//	param v01: goal/end vector of first range
//	param v10: initial vector of second range
//	param v11: goal/end vector of second range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending results of lerp (bi-lerp)
//	return: v_out
A3_INLINE a3real3r a3real3Bilerp(a3real3p v_out, const a3real3p v00, const a3real3p v01, const a3real3p v10, const a3real3p v11, const a3real param0, const a3real param1);

// A3: Tri-linear interpolation: calculate bi-lerp between each set of ranges, 
//		then lerp between the results.
//	param v_out: output interpolated vector
//	param v000: initial vector of first range
//	param v001: goal/end vector of first range
//	param v010: initial vector of second range
//	param v011: goal/end vector of second range
//	param v100: initial vector of third range
//	param v101: goal/end vector of third range
//	param v110: initial vector of fourth range
//	param v111: goal/end vector of fourth range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending in sets 000-011 and 100-111 (bi-lerp)
//	param param2: parameter for blending results of bi-lerp (tri-lerp)
//	return: v_out
A3_INLINE a3real3r a3real3Trilerp(a3real3p v_out, const a3real3p v000, const a3real3p v001, const a3real3p v010, const a3real3p v011, const a3real3p v100, const a3real3p v101, const a3real3p v110, const a3real3p v111, const a3real param0, const a3real param1, const a3real param2);

// A3: Catmull-Rom spline interpolation.
//	param v_out: output interpolated vector
//	param vPrev: previous control vector (before v0)
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vNext: next control vector (after v1)
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real3r a3real3CatmullRom(a3real3p v_out, const a3real3p vPrev, const a3real3p v0, const a3real3p v1, const a3real3p vNext, const a3real param);

// A3: Cubic Hermite spline (c-spline) interpolation using control handles.
//	param v_out: output interpolated vector
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control handle corresponding to v0
//	param vControl1: control handle corresponding to v1
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real3r a3real3HermiteControl(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real3p vControl0, const a3real3p vControl1, const a3real param);

// A3: Cubic Hermite spline (c-spline) interpolation using control tangents.
//	param v_out: output interpolated vector
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param nTangent0: control tangent corresponding to v0
//	param nTangent1: control tangent corresponding to v1
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real3r a3real3HermiteTangent(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real3p vTangent0, const a3real3p vTangent1, const a3real param);

// A3: Bezier interpolation of order/degree zero (constant).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param param: interpolation parameter (t), not used
//	return: v_out
A3_INLINE a3real3r a3real3Bezier0(a3real3p v_out, const a3real3p v0, const a3real param);

// A3: Bezier interpolation of first order/degree (linear).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param param: interpolation parameter (t), used for lerp; 
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real3r a3real3Bezier1(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real param);

// A3: Bezier interpolation of second order/degree (quadratic).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v0, result at t=1 is v2
//	return: v_out
A3_INLINE a3real3r a3real3Bezier2(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real3p v2, const a3real param);

// A3: Bezier interpolation of third order/degree (cubic).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param v3: fourth control vector
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v0, result at t=1 is v3
//	return: v_out
A3_INLINE a3real3r a3real3Bezier3(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real3p v2, const a3real3p v3, const a3real param);

// A3: Recursive Bezier interpolation of higher order (quartic, etc.).
//	param v_out: output interpolated vector
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param v: array of control vectors
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v[0], result at t=1 is v[order_N]
//	return: v_out
A3_INLINE a3real3r a3real3BezierN(a3real3p v_out, a3count order_N, const a3real3 v[], const a3real param);

// A3: Spherical linear interpolation ("SLERP"): interpolation along the arc 
//		with a uniform rate of change; improves on nlerp which yields the same 
//		results but does not have a uniform rate of change.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real3r a3real3Slerp(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real param);

// A3: Spherical linear interpolation ("SLERP"): interpolation along the arc 
//		with a uniform rate of change; improves on nlerp which yields the same 
//		results but does not have a uniform rate of change; this version 
//		assumes prior knowledge that the inputs are unit length.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real3r a3real3SlerpUnit(a3real3p v_out, const a3real3p v0, const a3real3p v1, const a3real param);

// A3: Calculate arc length of a Catmull-Rom curve segment.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param vPrev: previous control vector (before v0)
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vNext: next control vector (after v1)
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthCatmullRom(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p vPrev, const a3real3p v0, const a3real3p v1, const a3real3p vNext);

// A3: Calculate arc length of a cubic Hermite curve segment with handles.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control handle corresponding to v0
//	param vControl1: control handle corresponding to v1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthHermiteControl(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0, const a3real3p v1, const a3real3p vControl0, const a3real3p vControl1);

// A3: Calculate arc length of a cubic Hermite curve segment with tangents.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control tangent corresponding to v0
//	param vControl1: control tangent corresponding to v1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthHermiteTangent(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0, const a3real3p v1, const a3real3p vTangent0, const a3real3p vTangent1);

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
//	param v0: first control vector
//	param param: interpolation parameter (t), not used
//	return: zero
A3_INLINE a3real a3real3CalculateArcLengthBezier0(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0);

// A3: Calculate arc length of first order Bezier curve (linear).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthBezier1(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0, const a3real3p v1);

// A3: Calculate arc length of second order Bezier curve (quadratic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthBezier2(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate arc length of third order Bezier curve (cubic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param v3: fourth control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthBezier3(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0, const a3real3p v1, const a3real3p v2, const a3real3p v3);

// A3: Calculate arc length of higher-order Bezier curve (quartic, etc.).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param v: array of control vectors
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthBezierN(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3count order_N, const a3real3 v[]);

// A3: Calculate arc length of a slerp curve segment.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthSlerp(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0, const a3real3p v1);

// A3: Calculate arc length of a slerp curve segment with unit length inputs.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real3CalculateArcLengthSlerpUnit(a3real3 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real3p v0, const a3real3p v1);


// A3: Gram-Schmidt orthogonalization: flatten the input vector so that it 
//		becomes perpendicular to the base vector; useful for calculating 
//		transformation basis vectors.
//	param v_out: result of Gram-Schmidt
//	param v: vector to orthogonalize
//	param vBase: fixed base vector to orthogonalize against
//	return: v_out
A3_INLINE a3real3r a3real3GramSchmidtOrtho(a3real3p v_out, const a3real3p v, const a3real3p vBase);

// A3: Gram-Schmidt orthogonalization for multiple vectors.
//	param v0_out: result of Gram-Schmidt process on first vector
//	param v1_out: result of Gram-Schmidt process on second vector
//	param v0: first vector to orthogonalize
//	param v1: second vector to orthogonalize
//	param vBase: fixed base vector to orthogonalize against
//	return: v1_out
A3_INLINE a3real3r a3real3GramSchmidtOrtho2(a3real3p v0_out, a3real3p v1_out, const a3real3p v0, const a3real3p v1, const a3real3p vBase);

// A3: Gram-Schmidt orthogonalize.
//	param v_inout: vector to orthogonalize and store
//	param vBase: fixed base vector to orthogonalize against
//	return: v_out
A3_INLINE a3real3r a3real3GramSchmidt(a3real3p v_inout, const a3real3p vBase);

// A3: Gram-Schmidt orthogonalize multiple vectors.
//	param v0_inout: first vector to orthogonalize and store
//	param v1_inout: second vector to orthogonalize and store
//	param vBase: fixed base vector to orthogonalize against
//	return: v1_out
A3_INLINE a3real3r a3real3GramSchmidt2(a3real3p v0_inout, a3real3p v1_inout, const a3real3p vBase);


// A3: Cross product: calculate the vector perpendicular to both inputs.
//	param v_out: cross product of vL and vR, magnitude is parallelapiped volume
//	param vL: input left vector
//	param vR: input right vector
//	return: v_out
A3_INLINE a3real3r a3real3Cross(a3real3p v_out, const a3real3p vL, const a3real3p vR);

// A3: Cross product, unit length: calculate cross product, normalize result.
//	param v_out: cross product of vL and vR, magnitude is parallelapiped volume
//	param vL: input left vector
//	param vR: input right vector
//	return: v_out
A3_INLINE a3real3r a3real3CrossUnit(a3real3p v_out, const a3real3p vL, const a3real3p vR);

// A3: Calculate the normal vector of a triangle.
//	param n_out: normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: v_out
A3_INLINE a3real3r a3real3TriangleNormal(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate the unit length normal vector of a triangle.
//	param n_out: unit normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: v_out
A3_INLINE a3real3r a3real3TriangleNormalUnit(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate double area of a triangle and normal.
//	param n_out: normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: double area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaDoubledNormal(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate squared area of a triangle and normal.
//	param n_out: normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: squared area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaSquaredNormal(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate area of a triangle and normal.
//	param n_out: normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaNormal(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate double area of a triangle and unit normal.
//	param n_out: unit normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: double area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaDoubledNormalUnit(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate squared area of a triangle and unit normal.
//	param n_out: unit normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: squared area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaSquaredNormalUnit(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate area of a triangle and unit normal.
//	param n_out: unit normal vector of triangle formed by v0, v1 and v2
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaNormalUnit(a3real3p n_out, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate double area of a triangle.
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: double area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaDoubled(const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate squared area of a triangle.
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: squared area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleAreaSquared(const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Calculate area of a triangle.
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: area of triangle formed by v0, v1 and v2
A3_INLINE a3real a3real3TriangleArea(const a3real3p v0, const a3real3p v1, const a3real3p v2);


// A3: Test whether point is in triangle.
//	param p: point to test whether contained in triangle
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	return: boolean describing whether triangle is in triangle
A3_INLINE a3boolean a3real3PointIsInTriangle(const a3real3p p, const a3real3p v0, const a3real3p v1, const a3real3p v2);

// A3: Test whether point is in triangle and get barycentric coordinates.
//	param p: point to test whether contained in triangle
//	param v0: first vertex of triangle
//	param v1: second vertex of triangle
//	param v2: third vertex of triangle
//	param param0: pointer to variable to capture first barycentric coordinate
//	param param1: pointer to variable to capture second barycentric coordinate
//	param param2: pointer to variable to capture third barycentric coordinate
//	return: boolean describing whether triangle is in triangle
A3_INLINE a3boolean a3real3PointIsInTriangleBarycentric(const a3real3p p, const a3real3p v0, const a3real3p v1, const a3real3p v2, a3real *param0, a3real *param1, a3real *param2);


//-----------------------------------------------------------------------------
// 4D VECTOR FUNCTIONS

// A3: Initialize vector with individual components.
//	param v_out: vector to initialize
//	param x: first component
//	param y: second component
//	param z: third component
//	param w: fourth component
//	return: v_out
A3_INLINE a3real4r a3real4Set(a3real4p v_out, const a3real x, const a3real y, const a3real z, const a3real w);

// A3: Initialize vector with a 2D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real4r a3real4SetReal2(a3real4p v_out, const a3real2p v);

// A3: Initialize vector with a 2D vector and a third component.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	param z: third component
//	return: v_out
A3_INLINE a3real4r a3real4SetReal2Z(a3real4p v_out, const a3real2p v, const a3real z);

// A3: Initialize vector with a 2D vector, a third and a fourth component.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	param z: third component
//	param w: fourth component
//	return: v_out
A3_INLINE a3real4r a3real4SetReal2ZW(a3real4p v_out, const a3real2p v, const a3real z, const a3real w);

// A3: Initialize vector with a 3D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real4r a3real4SetReal3(a3real4p v_out, const a3real3p v);

// A3: Initialize vector with a 3D vector and a fourth component.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	param w: fourth component
//	return: v_out
A3_INLINE a3real4r a3real4SetReal3W(a3real4p v_out, const a3real3p v, const a3real w);

// A3: Initialize vector with a 4D vector.
//	param v_out: vector to initialize
//	param v: other vector to copy values from
//	return: v_out
A3_INLINE a3real4r a3real4SetReal4(a3real4p v_out, const a3real4p v);


// A3: Calculate squared length (magnitude) of vector.
//	param v: input vector
//	return: squared length of vector: |v|^2
A3_INLINE a3real a3real4LengthSquared(const a3real4p v);

// A3: Calculate inverse (reciprocal) squared length (magnitude) of vector.
//	param v: input vector
//	return: inverse squared length of vector: 1 / |v|^2
A3_INLINE a3real a3real4LengthSquaredInverse(const a3real4p v);

// A3: Calculate length (magnitude) of vector.
//	param v: input vector
//	return: length of vector: |v|
A3_INLINE a3real a3real4Length(const a3real4p v);

// A3: Calculate inverse (reciprocal) length (magnitude) of vector.
//	param v: input vector
//	return: inverse length of vector: 1 / |v|
A3_INLINE a3real a3real4LengthInverse(const a3real4p v);

// A3: Get unit direction of vector.
//	param v_out: output vector, direction of input: v / |v|
//	param v: input vector
//	return: v_out
A3_INLINE a3real4r a3real4GetUnit(a3real4p v_out, const a3real4p v);

// A3: Normalize vector, making it unit length (direction).
//	param v_inout: vector to normalize: v /= |v|
//	return: v_inout
A3_INLINE a3real4r a3real4Normalize(a3real4p v_inout);

// A3: Normalize vector, making it unit length (direction), get inverse length.
//	param v_inout: vector to normalize: v /= |v|
//	param invLength_out: pointer to variable capturing inverse length: 1 / |v|
//	return: v_inout
A3_INLINE a3real4r a3real4GetUnitInvLength(a3real4p v_out, const a3real4p v, a3real *invLen_out);

// A3: Get negative vector.
//	param v_out: output vector, negative of input: -v
//	param v: input vector
//	return: v_out
A3_INLINE a3real4r a3real4NormalizeGetInvLength(a3real4p v_inout, a3real *invLen_out);

// A3: Get negative vector.
//	param v_out: output vector, negative of input: -v
//	param v: input vector
//	return: v_out
A3_INLINE a3real4r a3real4GetNegative(a3real4p v_out, const a3real4p v);

// A3: Negate vector.
//	param v_inout: vector to negate: v = -v
//	return: v_inout
A3_INLINE a3real4r a3real4Negate(a3real4p v_inout);


// A3: Calculate dot product (relative alignment) of two vectors.
//	param vL: input left side vector
//	param vR: input right side vector
//	return: dot product: vL dot vR
A3_INLINE a3real a3real4Dot(const a3real4p vL, const a3real4p vR);

// A3: Calculate squared distance between two vectors.
//	param v0: input first vector
//	param v1: input second vector
//	return: squared distance between vectors: |v1 - v0|^2
A3_INLINE a3real a3real4DistanceSquared(const a3real4p v0, const a3real4p v1);

// A3: Calculate distance between two vectors.
//	param v0: input first vector
//	param v1: input second vector
//	return: distance between vectors: |v1 - v0|
A3_INLINE a3real a3real4Distance(const a3real4p v0, const a3real4p v1);

// A3: Calculate size ratio of vector projection.
//	param v: input vector to project
//	param vBase: vector to project onto
//	return: ratio of projected vector length to base vector length: 
//		(v dot vBase) / |vBase|^2
A3_INLINE a3real a3real4ProjRatio(const a3real4p v, const a3real4p vBase);

// A3: Calculate vector projection.
//	param v_out: output of vector projection: v projected onto vBase
//	param v: input vector to project
//	param vBase: vector to project onto
//	return: v_out
A3_INLINE a3real4r a3real4Projected(a3real4p v_out, const a3real4p v, const a3real4p vBase);

// A3: Project vector.
//	param v_inout: vector to project
//	param vBase: vector to project onto
//	return: v_inout
A3_INLINE a3real4r a3real4Proj(a3real4p v_inout, const a3real4p vBase);

// A3: Calculate vector projection and get projection ratio.
//	param v_out: output of vector projection: v projected onto vBase
//	param v: input vector to project
//	param vBase: vector to project onto
//	param ratio_out: pointer to variable capturing projection ratio: 
//		(v dot vBase) / |vBase|^2
//	return: v_out
A3_INLINE a3real4r a3real4ProjectedGetRatio(a3real4p v_out, const a3real4p v, const a3real4p vBase, a3real *ratio_out);

// A3: Project vector and get projection ratio.
//	param v_inout: vector to project
//	param vBase: vector to project onto
//	param ratio_out: pointer to variable capturing projection ratio: 
//		(v dot vBase) / |vBase|^2
//	return: v_inout
A3_INLINE a3real4r a3real4ProjGetRatio(a3real4p v_inout, const a3real4p vBase, a3real *ratio_out);

// A3: Calculate sum of vectors.
//	param v_out: output vector to store sum of two inputs
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real4r a3real4Sum(a3real4p v_out, const a3real4p vL, const a3real4p vR);

// A3: Calculate difference of vectors.
//	param v_out: output vector to store difference of two inputs
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real4r a3real4Diff(a3real4p v_out, const a3real4p vL, const a3real4p vR);

// A3: Calculate product of vector and scalar.
//	param v_out: output vector to store scaled vector
//	param v: input vector to be scaled
//	param s: scalar by which to multiply vector
//	return: v_out
A3_INLINE a3real4r a3real4ProductS(a3real4p v_out, const a3real4p v, const a3real s);

// A3: Calculate quotient of vector and scalar.
//	param v_out: output vector to store scaled vector
//	param v: input vector to be scaled, numerator
//	param s: scalar by which to divide vector, denominator
//	return: v_out
A3_INLINE a3real4r a3real4QuotientS(a3real4p v_out, const a3real4p v, const a3real s);

// A3: Calculate component-wise product of two vectors.
//	param v_out: output vector to store product vector: vL * vR components
//	param vL: input left side vector
//	param vR: input right side vector
//	return: v_out
A3_INLINE a3real4r a3real4ProductComp(a3real4p v_out, const a3real4p vL, const a3real4p vR);

// A3: Calculate component-wise quotient of two vectors.
//	param v_out: output vector to store quotient vector: vL / vR components
//	param vL: input left side vector, numerator
//	param vR: input right side vector, denominator
//	return: v_out
A3_INLINE a3real4r a3real4QuotientComp(a3real4p v_out, const a3real4p vL, const a3real4p vR);

// A3: Add vectors and store to left operand.
//	param vL_inout: left side vector to which to add
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real4r a3real4Add(a3real4p vL_inout, const a3real4p vR);

// A3: Subtract vectors and store to left operand.
//	param vL_inout: left side vector from which to subtract
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real4r a3real4Sub(a3real4p vL_inout, const a3real4p vR);

// A3: Multiply vector by scalar.
//	param v_inout: vector to scale
//	param s: scalar by which to multiply vector
//	return: v_inout
A3_INLINE a3real4r a3real4MulS(a3real4p v_inout, const a3real s);

// A3: Divide vector by scalar.
//	param v_inout: vector to scale, numerator
//	param s: scalar by which to multiply vector, denominator
//	return: v_inout
A3_INLINE a3real4r a3real4DivS(a3real4p v_inout, const a3real s);

// A3: Component-wise product of two vectors, store in left operand.
//	param vL_inout: left side vector to multiply and store: vL *= vR components
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real4r a3real4MulComp(a3real4p vL_inout, const a3real4p vR);

// A3: Component-wise quotient of two vectors, store in left operand.
//	param vL_inout: left side vector to divide and store: vL /= vR components
//	param vR: input right side vector
//	return: vL_inout
A3_INLINE a3real4r a3real4DivComp(a3real4p vL_inout, const a3real4p vR);


// A3: Linear interpolation ("LERP"): calculate linear interpolation between 
//		reference vectors 'v0' and 'v1' using a interpolation parameter 'param'
//		(sometimes referred to as 't' parameter or value).
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real4r a3real4Lerp(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real param);

// A3: Normalized linear interpolation ("NLERP"): calculate lerp between 
//		reference vectors 'v0' and 'v1' using a interpolation parameter 
//		'param' (sometimes referred to as 't' parameter or value), and 
//		normalize the result.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real4r a3real4NLerp(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real param);

// A3: Bi-linear interpolation: calculate lerp between each range of numbers, 
//		[v00, v01] and [v10, v11], and then calculate lerp between the results.
//	param v_out: output interpolated vector
//	param v00: initial vector of first range
//	param v01: goal/end vector of first range
//	param v10: initial vector of second range
//	param v11: goal/end vector of second range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending results of lerp (bi-lerp)
//	return: v_out
A3_INLINE a3real4r a3real4Bilerp(a3real4p v_out, const a3real4p v00, const a3real4p v01, const a3real4p v10, const a3real4p v11, const a3real param0, const a3real param1);

// A3: Tri-linear interpolation: calculate bi-lerp between each set of ranges, 
//		then lerp between the results.
//	param v_out: output interpolated vector
//	param v000: initial vector of first range
//	param v001: goal/end vector of first range
//	param v010: initial vector of second range
//	param v011: goal/end vector of second range
//	param v100: initial vector of third range
//	param v101: goal/end vector of third range
//	param v110: initial vector of fourth range
//	param v111: goal/end vector of fourth range
//	param param0: parameter for interpolation within ranges (lerp)
//	param param1: parameter for blending in sets 000-011 and 100-111 (bi-lerp)
//	param param2: parameter for blending results of bi-lerp (tri-lerp)
//	return: v_out
A3_INLINE a3real4r a3real4Trilerp(a3real4p v_out, const a3real4p v000, const a3real4p v001, const a3real4p v010, const a3real4p v011, const a3real4p v100, const a3real4p v101, const a3real4p v110, const a3real4p v111, const a3real param0, const a3real param1, const a3real param2);

// A3: Catmull-Rom spline interpolation.
//	param v_out: output interpolated vector
//	param vPrev: previous control vector (before v0)
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vNext: next control vector (after v1)
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real4r a3real4CatmullRom(a3real4p v_out, const a3real4p vPrev, const a3real4p v0, const a3real4p v1, const a3real4p vNext, const a3real param);

// A3: Cubic Hermite spline (c-spline) interpolation using control handles.
//	param v_out: output interpolated vector
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control handle corresponding to v0
//	param vControl1: control handle corresponding to v1
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real4r a3real4HermiteControl(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real4p vControl0, const a3real4p vControl1, const a3real param);

// A3: Cubic Hermite spline (c-spline) interpolation using control tangents.
//	param v_out: output interpolated vector
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param nTangent0: control tangent corresponding to v0
//	param nTangent1: control tangent corresponding to v1
//	param param: interpolation parameter (t)
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real4r a3real4HermiteTangent(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real4p vTangent0, const a3real4p vTangent1, const a3real param);

// A3: Bezier interpolation of order/degree zero (constant).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param param: interpolation parameter (t), not used
//	return: v_out
A3_INLINE a3real4r a3real4Bezier0(a3real4p v_out, const a3real4p v0, const a3real param);

// A3: Bezier interpolation of first order/degree (linear).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param param: interpolation parameter (t), used for lerp; 
//		result at t=0 is v0, result at t=1 is v1
//	return: v_out
A3_INLINE a3real4r a3real4Bezier1(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real param);

// A3: Bezier interpolation of second order/degree (quadratic).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v0, result at t=1 is v2
//	return: v_out
A3_INLINE a3real4r a3real4Bezier2(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real4p v2, const a3real param);

// A3: Bezier interpolation of third order/degree (cubic).
//	param v_out: output interpolated vector
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param v3: fourth control vector
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v0, result at t=1 is v3
//	return: v_out
A3_INLINE a3real4r a3real4Bezier3(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real4p v2, const a3real4p v3, const a3real param);

// A3: Recursive Bezier interpolation of higher order (quartic, etc.).
//	param v_out: output interpolated vector
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param v: array of control vectors
//	param param: interpolation parameter (t), used for recursive lerp; 
//		result at t=0 is v[0], result at t=1 is v[order_N]
//	return: v_out
A3_INLINE a3real4r a3real4BezierN(a3real4p v_out, a3count order_N, const a3real4 v[], const a3real param);

// A3: Spherical linear interpolation ("SLERP"): interpolation along the arc 
//		with a uniform rate of change; improves on nlerp which yields the same 
//		results but does not have a uniform rate of change.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real4r a3real4Slerp(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real param);

// A3: Spherical linear interpolation ("SLERP"): interpolation along the arc 
//		with a uniform rate of change; improves on nlerp which yields the same 
//		results but does not have a uniform rate of change; this version 
//		assumes prior knowledge that the inputs are unit length.
//	param v_out: output interpolated vector
//	param v0: initial vector, result when t=0
//	param v1: goal/end vector, result when t=1
//	param param: interpolation parameter; t values in [0, 1] result in 
//		interpolation, others yield extrapolation
//	return: v_out
A3_INLINE a3real4r a3real4SlerpUnit(a3real4p v_out, const a3real4p v0, const a3real4p v1, const a3real param);

// A3: Calculate arc length of a Catmull-Rom curve segment.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param vPrev: previous control vector (before v0)
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vNext: next control vector (after v1)
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthCatmullRom(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p vPrev, const a3real4p v0, const a3real4p v1, const a3real4p vNext);

// A3: Calculate arc length of a cubic Hermite curve segment with handles.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control handle corresponding to v0
//	param vControl1: control handle corresponding to v1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthHermiteControl(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0, const a3real4p v1, const a3real4p vControl0, const a3real4p vControl1);

// A3: Calculate arc length of a cubic Hermite curve segment with tangents.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	param vControl0: control tangent corresponding to v0
//	param vControl1: control tangent corresponding to v1
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthHermiteTangent(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0, const a3real4p v1, const a3real4p vTangent0, const a3real4p vTangent1);

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
//	param v0: first control vector
//	param param: interpolation parameter (t), not used
//	return: zero
A3_INLINE a3real a3real4CalculateArcLengthBezier0(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0);

// A3: Calculate arc length of first order Bezier curve (linear).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthBezier1(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0, const a3real4p v1);

// A3: Calculate arc length of second order Bezier curve (quadratic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthBezier2(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0, const a3real4p v1, const a3real4p v2);

// A3: Calculate arc length of third order Bezier curve (cubic).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: first control vector
//	param v1: second control vector
//	param v2: third control vector
//	param v3: fourth control vector
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthBezier3(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0, const a3real4p v1, const a3real4p v2, const a3real4p v3);

// A3: Calculate arc length of higher-order Bezier curve (quartic, etc.).
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param order_N: order/degree of function (4 for quartic, etc.)
//	param v: array of control vectors
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthBezierN(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3count order_N, const a3real4 v[]);

// A3: Calculate arc length of a slerp curve segment.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthSlerp(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0, const a3real4p v1);

// A3: Calculate arc length of a slerp curve segment with unit length inputs.
//	param sampleTable_out: array of samples captured while calculating length
//	param paramTable_out: array of interpolation params matched to samples
//	param arclenTable_out: array of accumulated arc lengths at each sample
//	param autoNormalize: option to normalize arc lengths
//	param numDivisions: number of intermediate samples on curve segment; 
//		precision of arc length increases as this number increases; 
//		tables should have (numDivisions + 1) elements allocated
//	param v0: initial control vector of curve segment
//	param v1: goal/end control vector of curve segment
//	return: total arc length of segment; zero if invalid params
A3_INLINE a3real a3real4CalculateArcLengthSlerpUnit(a3real4 sampleTable_out[], a3real paramTable_out[], a3real arclenTable_out[], const a3boolean autoNormalize, const a3count numDivisions, const a3real4p v0, const a3real4p v1);


// A3: Gram-Schmidt orthogonalization: flatten the input vector so that it 
//		becomes perpendicular to the base vector; useful for calculating 
//		transformation basis vectors.
//	param v_out: result of Gram-Schmidt
//	param v: vector to orthogonalize
//	param vBase: fixed base vector to orthogonalize against
//	return: v_out
A3_INLINE a3real4r a3real4GramSchmidtOrtho(a3real4p v_out, const a3real4p v, const a3real4p vBase);

// A3: Gram-Schmidt orthogonalization for multiple vectors.
//	param v0_out: result of Gram-Schmidt process on first vector
//	param v1_out: result of Gram-Schmidt process on second vector
//	param v0: first vector to orthogonalize
//	param v1: second vector to orthogonalize
//	param vBase: fixed base vector to orthogonalize against
//	return: v1_out
A3_INLINE a3real4r a3real4GramSchmidtOrtho2(a3real4p v0_out, a3real4p v1_out, const a3real4p v0, const a3real4p v1, const a3real4p vBase);

// A3: Gram-Schmidt orthogonalization for multiple vectors.
//	param v0_out: result of Gram-Schmidt process on first vector
//	param v1_out: result of Gram-Schmidt process on second vector
//	param v2_out: result of Gram-Schmidt process on third vector
//	param v0: first vector to orthogonalize
//	param v1: second vector to orthogonalize
//	param v2: third vector to orthogonalize
//	param vBase: fixed base vector to orthogonalize against
//	return: v2_out
A3_INLINE a3real4r a3real4GramSchmidtOrtho3(a3real4p v0_out, a3real4p v1_out, a3real4p v2_out, const a3real4p v0, const a3real4p v1, const a3real4p v2, const a3real4p vBase);

// A3: Gram-Schmidt orthogonalize.
//	param v_inout: vector to orthogonalize and store
//	param vBase: fixed base vector to orthogonalize against
//	return: v_out
A3_INLINE a3real4r a3real4GramSchmidt(a3real4p v_inout, const a3real4p vBase);

// A3: Gram-Schmidt orthogonalize multiple vectors.
//	param v0_inout: first vector to orthogonalize and store
//	param v1_inout: second vector to orthogonalize and store
//	param vBase: fixed base vector to orthogonalize against
//	return: v1_out
A3_INLINE a3real4r a3real4GramSchmidt2(a3real4p v0_inout, a3real4p v1_inout, const a3real4p vBase);

// A3: Gram-Schmidt orthogonalize multiple vectors.
//	param v0_inout: first vector to orthogonalize and store
//	param v1_inout: second vector to orthogonalize and store
//	param v2_inout: third vector to orthogonalize and store
//	param vBase: fixed base vector to orthogonalize against
//	return: v2_out
A3_INLINE a3real4r a3real4GramSchmidt3(a3real4p v0_inout, a3real4p v1_inout, a3real4p v2_inout, const a3real4p vBase);


//-----------------------------------------------------------------------------

#ifndef A3_OPEN_SOURCE
// pre-defined constants for vector presets for quick copying
// extern if closed-source, exposed if open-source
///
extern const a3vec2 a3vec2_zero, a3vec2_one, a3vec2_x, a3vec2_y;
extern const a3vec3 a3vec3_zero, a3vec3_one, a3vec3_x, a3vec3_y, a3vec3_z;
extern const a3vec4 a3vec4_zero, a3vec4_one, a3vec4_x, a3vec4_y, a3vec4_z, a3vec4_w;
#endif	// !A3_OPEN_SOURCE


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3vector2_impl.inl"
#include "_inl/a3vector3_impl.inl"
#include "_inl/a3vector4_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_VECTOR_H