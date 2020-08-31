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

	a3dualquaternion.h
	Declarations for dual quaternion functions.
*/

#ifndef __ANIMAL3D_A3DM_DUALQUATERNION_H
#define __ANIMAL3D_A3DM_DUALQUATERNION_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3macros.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------
// data structure to represent a pair of quaternions used for transformations
// a quaternion can be used to "encode" a rotation and/or uniform scale* (unit 
//	length indicates no scale encoded); dual quaternions add the ability to 
//	"encode" a translation in quaternion form, thereby representing a complete 
//	transformation with a rotation, uniform scale and translation
// useful reads: 
// https://www.cs.utah.edu/~ladislav/kavan06dual/kavan06dual.pdf
// http://www.xbdev.net/misc_demos/demos/dual_quaternions_beyond/paper.pdf
// http://web.cs.iastate.edu/~cs577/handouts/dual-quaternion.pdf
// https://www.essentialmath.com/GDC2013/GDC13_quaternions_final.pdf

// *NOTE: the scale encoding features have been removed as the current solution 
//	is not mathematically sound and will yield incorrect results; the only 
//	existing "non-rigid transforms" and dual quaternions reference available 
//	is the patent linked below, which suggests that scaling should be handled 
//	separately, using unit dual quaternions to represent rotation and scale...
//	furthermore, in the blog linked below, the solution presented is also not 
//	mathematically sound and therefore it should not be used here (proven on 
//	paper in so many different ways; it's intuitive but ultimately incorrect)
// ...here are literally the only things that currently exist on the matter: 
// patent: https://www.google.com/patents/US20150002517
// blog: http://roartindon.blogspot.ca/2015/04/encoding-uniform-scale-in-quaternions.html


#ifndef __cplusplus
typedef union a3dualquat a3dualquat;
#endif	// !__cplusplus


// A3: Dual quaternion container structure.
//	member rx: first real component
//	member ry: second real component
//	member rz: third real component
//	member rw: fourth real component
//	member dx: first dual component
//	member dy: second dual component
//	member dz: third dual component
//	member dw: fourth dual component
//	member r: real part quaternion
//	member d: dual part quaternion
//	member Q: all components as multi-dimensional array
//	member QQ: all components as linear array
union a3dualquat
{
	// components: 
	//	"a3real" part 'r' to encode rotation and/or scale*, and 
	//	"dual" part 'd' to encode translation
	struct { a3real rx, ry, rz, rw, dx, dy, dz, dw; };
	struct { a3quat r, d; };

	// arrays (INPUT FOR OPERATIONS)
	a3real4x2 Q;	// behaves as type 'a3real[][4]' or 'a3real*[4]'
	a3real4 QQ;		// behaves as type 'a3real[]' or 'a3real*'
};


// A3: Dual quaternion component enumerator.
enum a3dualquatComponent
{
	a3dualquatComp_real,	// encodes rotation and/or uniform scale*
	a3dualquatComp_dual		// encodes translation
};


//-----------------------------------------------------------------------------
// dual quaternion operations

// A3: Create identity dual quaternion.
//	param Q_out: dual quaternion to initialize
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSetIdentity(a3real4x2p Q_out);

// A3: Create dual quaternion from a pair of quaternions.
//	param Q_out: dual quaternion to initialize
//	param rotateScaleQuat: 'real' quaternion part to encode rotation/scale
//	param dualPartQuat: 'dual' quaternion part to encode translation
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSet(a3real4x2p Q_out, const a3real4 rotateScaleQuat, const a3real4 dualPartQuat);

// A3: Create dual quaternion from rotation axis and angle.
//	param Q_out: dual quaternion to initialize
//	param unitAxis: axis of rotation, unit length (for efficiency)
//	param degrees: validated rotation angle in degrees
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSetAxisAngle(a3real4x2p Q_out, const a3real3p unitAxis, const a3real degrees);

// A3: Create dual quaternion from rotation and translation.
//	param Q_out: dual quaternion to initialize
//	param unitAxis: axis of rotation, unit length
//	param degrees: validated rotation angle in degrees
//	param translate: translation vector to encode in dual part
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSetAxisAngleTranslate(a3real4x2p Q_out, const a3real3p unitAxis, const a3real degrees, const a3real3p translate);

// A3: Create dual quaternion from Euler angles.
//	param Q_out: dual quaternion to initialize
//	param degrees_x: validated rotation angle in degrees about X axis
//	param degrees_y: validated rotation angle in degrees about Y axis
//	param degrees_z: validated rotation angle in degrees about Z axis
//	param eulerIsXYZ: boolean to determine rotation order: 
//		XYZ if true, ZYX if false
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSetEuler(a3real4x2p Q_out, const a3real degrees_x, const a3real degrees_y, const a3real degrees_z, const a3boolean eulerIsXYZ);

// A3: Create dual quaternion from Euler angles and translation.
//	param Q_out: dual quaternion to initialize
//	param degrees_x: validated rotation angle in degrees about X axis
//	param degrees_y: validated rotation angle in degrees about Y axis
//	param degrees_z: validated rotation angle in degrees about Z axis
//	param eulerIsXYZ: boolean to determine rotation order: 
//		XYZ if true, ZYX if false
//	param translate: translation vector to encode in dual part
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSetEulerTranslate(a3real4x2p Q_out, const a3real degrees_x, const a3real degrees_y, const a3real degrees_z, const a3boolean eulerIsXYZ, const a3real3p translate);

// A3: Create dual quaternion from translation.
//	param Q_out: dual quaternion to initialize
//	param translate: translation vector to encode in dual part
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSetTranslate(a3real4x2p Q_out, const a3real3p translate);

// A3: Create dual quaternion from another dual quaternion.
//	param Q_out: dual quaternion to initialize
//	param Q: dual quaternion from which to copy
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatCopy(a3real4x2p Q_out, const a3real4x2p Q);


// A3: Get conjugate of dual quaternion: vector parts are negated.
//	param Q_out: output dual quaternion to store conjugate
//	param Q: input dual quaternion whose conjugate to get
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatGetConjugated(a3real4x2p Q_out, const a3real4x2p Q);

// A3: Get inverse of assumed unit dual quaternion; same as get conjugate.
//	param Q_out: output dual quaternion to store inverse
//	param Q: input dual quaternion whose inverse to get
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatGetInverseIgnoreScale(a3real4x2p Q_out, const a3real4x2p Q);

// A3: Conjugate dual quaternion (negate vector parts).
//	param Q_inout: dual quaternion to conjugate and store
//	return: Q_inout
A3_INLINE a3real4x2r a3dualquatConjugate(a3real4x2p Q_inout);

// A3: Invert assumed unit dual quaternion; same as conjugate.
//	param Q_inout: dual quaternion to invert and store
//	return: Q_inout
A3_INLINE a3real4x2r a3dualquatInvertIgnoreScale(a3real4x2p Q_inout);

// A3: Get the axis and angle of rotation of an assumed unit dual quaternion.
//	param Q: dual quaternion from which to get axis and angle
//	param unitAxis_out: variable to capture unit axis
//	param degrees_out: pointer to variable to capture rotation angle in degrees
//	return: Q
A3_INLINE a3real4x2rk a3dualquatGetAxisAngleIgnoreScale(const a3real4x2p Q, a3real3p unitAxis_out, a3real *degrees_out);

// A3: Get rotation axis/angle, translate of an assumed unit dual quaternion.
//	param Q: dual quaternion from which to get axis and angle
//	param unitAxis_out: variable to capture unit axis
//	param degrees_out: pointer to variable to capture rotation angle in degrees
//	param translate_out: vector to capture translation vector
//	return: Q
A3_INLINE a3real4x2rk a3dualquatGetAxisAngleTranslateIgnoreScale(const a3real4x2p Q, a3real3p unitAxis_out, a3real *degrees_out, a3real3p translate_out);


// A3: Get the squared length (magnitude) of a dual quaternion.
//	param Q: dual quaternion whose squared length to get
//	return: squared length of dual quaternion: |Q|^2
A3_INLINE a3real a3dualquatLengthSquared(const a3real4x2p Q);

// A3: Get the inverse (reciprocal) squared length of a dual quaternion.
//	param Q: dual quaternion whose inverse squared length to get
//	return: inverse squared length of dual quaternion: 1 / |Q|^2
A3_INLINE a3real a3dualquatLengthSquaredInverse(const a3real4x2p Q);

// A3: Get the length (magnitude) of a dual quaternion.
//	param Q: dual quaternion whose length to get
//	return: length of dual quaternion: |Q|
A3_INLINE a3real a3dualquatLength(const a3real4x2p Q);

// A3: Get the inverse (reciprocal) length of a dual quaternion.
//	param Q: dual quaternion whose inverse length to get
//	return: inverse length of dual quaternion: 1 / |Q|
A3_INLINE a3real a3dualquatLengthInverse(const a3real4x2p Q);

// A3: Get unit dual quaternion, representing encoded rotation and translation.
//	param Q_out: output dual quaternion to store unit of input: Q / |Q|
//	param Q: input dual quaternion
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatGetUnit(a3real4x2p Q_out, const a3real4x2p Q);

// A3: Get unit dual quaternion and inverse length.
//	param Q_out: output dual quaternion to store unit of input: Q / |Q|
//	param Q: input dual quaternion
//	param invLength_out: pointer to variable capturing inverse length: 1 / |Q|
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatGetUnitInvLength(a3real4x2p Q_out, const a3real4x2p Q, a3real *invLength_out);

// A3: Normalize dual quaternion, making it unit length.
//	param Q_inout: output dual quaternion to store unit of input: Q /= |Q|
//	return: Q_inout
A3_INLINE a3real4x2r a3dualquatNormalize(a3real4x2p Q_inout);

// A3: Normalize dual quaternion and get inverse length.
//	param Q_inout: output dual quaternion to store unit of input: Q /= |Q|
//	param invLength_out: pointer to variable capturing inverse length: 1 / |Q|
//	return: Q_inout
A3_INLINE a3real4x2r a3dualquatNormalizeGetInvLength(a3real4x2p Q_inout, a3real *invLength_out);


// A3: Convert dual quaternion to 4D transformation matrix.
//	param m_out: output matrix representing rotation and translation
//	param Q: input dual quaternion to convert
//	return: m_out
A3_INLINE a3real4x4r a3dualquatConvertToMat4IgnoreScale(a3real4x4p m_out, const a3real4x2 Q);


// A3: Calculate sum of dual quaternions.
//	param Q_out: output dual quaternion to store sum of two inputs
//	param QL: input left side dual quaternion
//	param QR: input right side dual quaternion
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSum(a3real4x2p Q_out, const a3real4x2p QL, const a3real4x2p QR);

// A3: Calculate difference of dual quaternions.
//	param Q_out: output dual quaternion to store difference of two inputs
//	param QL: input left side dual quaternion
//	param QR: input right side dual quaternion
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatDiff(a3real4x2p Q_out, const a3real4x2p QL, const a3real4x2p QR);

// A3: Calculate product of dual quaternion and scalar.
//	param Q_out: output dual quaternion to store scaled dual quaternion
//	param Q: input vector to be scaled
//	param s: scalar by which to multiply dual quaternion
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatProductS(a3real4x2p Q_out, const a3real4x2p Q, const a3real s);

// A3: Calculate quotient of dual quaternion and scalar.
//	param Q_out: output dual quaternion to store scaled dual quaternion
//	param Q: input vector to be scaled, numerator
//	param s: scalar by which to divide dual quaternion, denominator
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatQuotientS(a3real4x2p Q_out, const a3real4x2p Q, const a3real s);

// A3: Add dual quaternions and store to left operand.
//	param QL_inout: left side dual quaternion to which to add
//	param QR: input right side dual quaternion
//	return: QL_inout
A3_INLINE a3real4x2r a3dualquatAdd(a3real4x2p QL_inout, const a3real4x2p QR);

// A3: Subtract dual quaternions and store to left operand.
//	param QL_inout: left side dual quaternion from which to subtract
//	param QR: input right side dual quaternion
//	return: QL_inout
A3_INLINE a3real4x2r a3dualquatSub(a3real4x2p QL_inout, const a3real4x2p QR);

// A3: Multiply dual quaternion by scalar.
//	param Q_inout: dual quaternion to scale
//	param s: scalar by which to multiply dual quaternion
//	return: Q_inout
A3_INLINE a3real4x2r a3dualquatMulS(a3real4x2p Q_inout, const a3real s);

// A3: Divide dual quaternion by scalar.
//	param Q_inout: dual quaternion to scale, numerator
//	param s: scalar by which to divide dual quaternion, denominator
//	return: Q_inout
A3_INLINE a3real4x2r a3dualquatDivS(a3real4x2p Q_inout, const a3real s);


// A3: Calculate dual quaternion product.
//	param Q_out: output dual quaternion to store product
//	param QL: input left dual quaternion
//	param QR: input right dual quaternion
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatProduct(a3real4x2p Q_out, const a3real4x2p QL, const a3real4x2 QR);

// A3: Concatenate (multiply) dual quaternions and store in left input.
//	param QL_inout: left input dual quaternion to store product
//	param QR: input right dual quaternion
//	return: QL_inout
A3_INLINE a3real4x2r a3dualquatConcatL(a3real4x2p QL_inout, const a3real4x2 QR);

// A3: Concatenate (multiply) dual quaternions and store in right input.
//	param QL: input left dual quaternion
//	param QR_inout: right input dual quaternion to store product
//	return: QR_inout
A3_INLINE a3real4x2r a3dualquatConcatR(const a3real4x2p QL, a3real4x2 QR_inout);

// A3: Calculate vector transformed by assumed unit dual quaternion.
//	param v_out: output rotated vector
//	param v: input vector to transform by dual quaternion
//	param Q: dual quaternion to transform vector
//	return: v_out
A3_INLINE a3real3r a3dualquatVec3GetTransformedIgnoreScale(a3real3p v_out, const a3real3p v, const a3real4x2p Q);

// A3: Transform vector by assumed unit quaternion.
//	param v_inout: vector to rotate and store
//	param Q: dual quaternion to transform vector
//	return: v_inout
A3_INLINE a3real3r a3dualquatVec3TransformIgnoreScale(a3real3p v_inout, const a3real4x2p Q);


// A3: Screw linear interpolation ("ScLERP") for assumed unit dual quaternions.
//		Essentially SLERP for dual quaternions, accounting for the translation.
//	param Q_out: output dual quaternion to store interpolation result
//	param Q0: initial dual quaternion
//	param Q1: goal/end dual quaternion
//	param param: interpolation parameter (t); 
//		result is Q0 when t=0 and Q1 when t=1
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSclerpUnit(a3real4x2p Q_out, const a3real4x2p Q0, const a3real4x2p Q1, const a3real param);

// A3: Screw linear interpolation ("ScLERP") from identity to input.
//	param Q_out: output dual quaternion to store interpolation result
//	param Q1: goal/end dual quaternion
//	param param: interpolation parameter (t); 
//		result is identity when t=0 and Q1 when t=1
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSclerpUnitIdentityQ0(a3real4x2p Q_out, const a3real4x2p Q1, const a3real param);

// A3: Screw linear interpolation ("ScLERP") from input to identity.
//	param Q_out: output dual quaternion to store interpolation result
//	param Q0: initial dual quaternion
//	param param: interpolation parameter (t); 
//		result is Q0 when t=0 and identity when t=1
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatSclerpUnitIdentityQ1(a3real4x2p Q_out, const a3real4x2p Q0, const a3real param);

// A3: Dual linear blend ("DLB") for dual quaternions with two inputs.
//		Similar to NLERP for dual quaternions, just a normalized sum.
//	param Q_out: output dual quaternion to store blend result
//	param Q0: first input
//	param Q1: second input
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatDLB2(a3real4x2p Q_out, const a3real4x2p Q0, const a3real4x2p Q1);

// A3: Dual linear blend ("DLB") for dual quaternions with four inputs.
//		Similar to NLERP for dual quaternions, just a normalized sum.
//	param Q_out: output dual quaternion to store blend result
//	param Q0: first input
//	param Q1: second input
//	param Q2: third input
//	param Q3: fourth input
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatDLB4(a3real4x2p Q_out, const a3real4x2p Q0, const a3real4x2p Q1, const a3real4x2p Q2, const a3real4x2p Q3);

// A3: Dual linear blend ("DLB") for dual quaternions with array of inputs.
//		Similar to NLERP for dual quaternions, just a normalized sum.
//	param Q_out: output dual quaternion to store blend result
//	param Q: array of inputs
//	param count: number of inputs
//	return: Q_out
A3_INLINE a3real4x2r a3dualquatDLB(a3real4x2p Q_out, const a3real4x2 Q[], const a3count count);


// A3: Encode translation vector into dual part of dual quaternion.
//	param d_out: output dual part quaternion
//	param r: real part quaternion
//	param translate: translation vector to encode to dual part
//	return: d_out
A3_INLINE a3real4r a3dualquatCalculateDualPart(a3real4p d_out, const a3real4p r, const a3real3p translate);

// A3: Decode dual part of dual quaternion to extract translation vector.
//	param translate_out: output translation vector encoded in dual part
//	param r: real part quaternion
//	param d: dual part quaternion
//	return: translate_out
A3_INLINE a3real3r a3dualquatCalculateTranslateIgnoreScale(a3real3p translate_out, const a3real4p r, const a3real4p d);


// A3: Get kinematic screw parameters from dual quaternion.
//	param Q: input dual quaternion to get parameters from
//	param unitAxis_out: output unit axis of rotation
//	param moment_out: output axis of motion
//	param translate_out: output translation vector
//	param degrees_out: pointer to variable to capture rotation angle in degrees
//	param distance_out: pointer to variable to capture translation distance
//	return: Q
A3_INLINE a3real4x2rk a3dualquatCalculateScrewParamsIgnoreScale(const a3real4x2p Q, a3real3p unitAxis_out, a3real3p moment_out, a3real3p translate_out, a3real *degrees_out, a3real *distance_out);


//-----------------------------------------------------------------------------

#ifndef A3_OPEN_SOURCE
// pre-defined constant for identity dual quaternion
// extern if closed-source, exposed if open-source
///
extern const a3dualquat a3dualquat_identity;
#endif	// !A3_OPEN_SOURCE


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3dualquaternion_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_DUALQUATERNION_H