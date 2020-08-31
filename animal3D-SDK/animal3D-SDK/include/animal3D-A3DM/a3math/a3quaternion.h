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

	a3quaternion.h
	Declarations for quaternion functions.
*/

#ifndef __ANIMAL3D_A3DM_QUATERNION_H
#define __ANIMAL3D_A3DM_QUATERNION_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3macros.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"
#include "a3matrix.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------
// A3: Data structure for quaternion.
// Crash course in quaternions: 
//	Quaternions are 4-dimensional numbers, most commonly used to represent a 
//	3-dimensional rotation.  Why 4D then?  Instead of thinking of a quaternion 
//	as a direct mapping between number and rotation, think of it more like an 
//	"encoding" of the components of a rotation: the angle and axis; the a3real 
//	part of the quaternion, w, "encodes" the angle, while the vector part, 
//	xyz, "encodes" the axis.  A unit quaternion (length is 1) represents a 
//	rotation, while a non-unit quaternion means that a uniform scale of 
//	length-squared is also "encoded" in the quaternion.
// NOTE: quaternions do not have handedness; they can be converted into a 
//	matrix with handedness

#ifndef __cplusplus
typedef union a3quat a3quat;
#endif	// !__cplusplus

// A3: Quaternion container structure.
//	member x: first imaginary component
//	member y: second imaginary component
//	member z: third imaginary component
//	member w: real/scalar component
//	member v: imaginary/vector components
//	member q: components as array
//	member qv: imaginary/vector components as array
union a3quat
{
	// components
	struct { a3real x, y, z, w; };
	a3vec3 v;

	// array (INPUT FOR OPERATIONS)
	a3real4 q;
	a3real3 qv;
};


//-----------------------------------------------------------------------------
// A3 Quaternion functions and operations.
// *note: treat this as an extension of the 4D vector; quaternions can do all 
//	the same stuff as vec4's, just call the vec4 function with a quaternion 
//	array as input and the behavior will work as expected (e.g. add, sub, dot)

// A3: Initialize quaternion as identity.
//	param q_out: quaternion to initialize
//	return: q_out
A3_INLINE a3real4r a3quatSetIdentity(a3real4p q_out);

// A3: Initialize quaternion with components.
//	param q_out: quaternion to initialize
//	params x,y,z: imaginary components
//	param w: real component
//	return: q_out
A3_INLINE a3real4r a3quatSet(a3real4p q_out, const a3real x, const a3real y, const a3real z, const a3real w);

// A3: Initialize quaternion with 4D vector.
//	param q_out: quaternion to initialize
//	param v: vector to copy
//	return: q_out
A3_INLINE a3real4r a3quatSetReal4(a3real4p q_out, const a3real4p v);


// A3: Create rotation quaternion from axis and angle.
//	param q_out: quaternion to initialize
//	param unitAxis: axis of rotation, unit length (for efficiency)
//	param degrees: validated rotation angle in degrees
//	return: q_out
A3_INLINE a3real4r a3quatSetAxisAngle(a3real4p q_out, const a3real3p unitAxis, const a3real degrees);

// A3: Create rotation quaternion about X axis given angle.
//	param q_out: quaternion to initialize
//	param degrees: validated rotation angle in degrees about X axis
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerX(a3real4p q_out, const a3real degrees_x);

// A3: Create rotation quaternion about Y axis given angle.
//	param q_out: quaternion to initialize
//	param degrees: validated rotation angle in degrees about Y axis
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerY(a3real4p q_out, const a3real degrees_y);

// A3: Create rotation quaternion about Z axis given angle.
//	param q_out: quaternion to initialize
//	param degrees: validated rotation angle in degrees about Z axis
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerZ(a3real4p q_out, const a3real degrees_z);

// A3: Set quaternion to represent rotation transformation given Euler angles 
//		in XYZ order.
//	param q_out: quaternion to initialize
//	param degrees_x: validated rotation angle in degrees about X axis
//	param degrees_y: validated rotation angle in degrees about Y axis
//	param degrees_z: validated rotation angle in degrees about Z axis
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerXYZ(a3real4p q_out, const a3real degrees_x, const a3real degrees_y, const a3real degrees_z);

// A3: Set quaternion to represent rotation transformation given Euler angles 
//		in ZYX order.
//	param q_out: quaternion to initialize
//	param degrees_x: validated rotation angle in degrees about X axis
//	param degrees_y: validated rotation angle in degrees about Y axis
//	param degrees_z: validated rotation angle in degrees about Z axis
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerZYX(a3real4p q_out, const a3real degrees_x, const a3real degrees_y, const a3real degrees_z);

// A3: Create rotation quaternion between two arbitrary 3D vectors.
//	param q_out: quaternion to initialize
//	param unitV0: first vector
//	param unitV1: second vector
//	return: q_out
A3_INLINE a3real4r a3quatSetVectorDelta(a3real4p q_out, const a3real3p unitV0, const a3real3p unitV1);


// A3: Create rotation and uniform scale quaternion from axis, angle and 
//		uniform scale.
//	param q_out: quaternion to initialize
//	param unitAxis: axis of rotation, unit length (for efficiency)
//	param degrees: validated rotation angle in degrees
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetAxisAngleScale(a3real4p q_out, const a3real3p unitAxis, const a3real degrees, const a3real s);

// A3: Create rotation and uniform scale quaternion about X axis given angle 
//		and uniform scale.
//	param q_out: quaternion to initialize
//	param degrees: validated rotation angle in degrees about X axis
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerXScale(a3real4p q_out, const a3real degrees_x, const a3real s);

// A3: Create rotation and uniform scale quaternion about Y axis given angle 
//		and uniform scale.
//	param q_out: quaternion to initialize
//	param degrees: validated rotation angle in degrees about Y axis
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerYScale(a3real4p q_out, const a3real degrees_y, const a3real s);

// A3: Create rotation and uniform scale quaternion about Z axis given angle 
//		and uniform scale.
//	param q_out: quaternion to initialize
//	param degrees: validated rotation angle in degrees about Z axis
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerZScale(a3real4p q_out, const a3real degrees_z, const a3real s);

// A3: Set quaternion to represent rotation and uniform scale transformation 
//		given Euler angles in XYZ order and uniform scale.
//	param q_out: quaternion to initialize
//	param degrees_x: validated rotation angle in degrees about X axis
//	param degrees_y: validated rotation angle in degrees about Y axis
//	param degrees_z: validated rotation angle in degrees about Z axis
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerXYZScale(a3real4p q_out, const a3real degrees_x, const a3real degrees_y, const a3real degrees_z, const a3real s);

// A3: Set quaternion to represent rotation and uniform scale transformation 
//		given Euler angles in ZYX order and uniform scale.
//	param q_out: quaternion to initialize
//	param degrees_x: validated rotation angle in degrees about X axis
//	param degrees_y: validated rotation angle in degrees about Y axis
//	param degrees_z: validated rotation angle in degrees about Z axis
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetEulerZYXScale(a3real4p q_out, const a3real degrees_x, const a3real degrees_y, const a3real degrees_z, const a3real s);

// A3: Create rotation and uniform scale quaternion between two arbitrary 
//		3D vectors.
//	param q_out: quaternion to initialize
//	param unitV0: first vector
//	param unitV1: second vector
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetVectorDeltaScale(a3real4p q_out, const a3real3p unitV0, const a3real3p unitV1, const a3real s);

// A3: Create uniform scale quaternion.
//	param q_out: quaternion to initialize
//	param s: uniform scale
//	return: q_out
A3_INLINE a3real4r a3quatSetScale(a3real4p q_out, const a3real s);


// A3: Get conjugate of quaternion: vector part is negated.
//	param q_out: output quaternion to store conjugate
//	param q: input quaternion whose conjugate to get
//	return: q_out
A3_INLINE a3real4r a3quatGetConjugated(a3real4p q_out, const a3real4p q);

// A3: Get inverse of quaternion: multiplying a quaternion by its own inverse 
//		yields the identity quaternion (1).
//	param q_out: output quaternion to store inverse
//	param q: input quaternion whose inverse to get
//	return: q_out
A3_INLINE a3real4r a3quatGetInverse(a3real4p q_out, const a3real4p q);

// A3: Get inverse of and uniform scale encoded by quaternion.
//	param q_out: output quaternion to store inverse
//	param s_out: pointer to variable to capture uniform scale
//	param q: input quaternion whose inverse and scale to get
//	return: q_out
A3_INLINE a3real4r a3quatGetInverseGetScale(a3real4p q_out, a3real *s_out, const a3real4p q);

// A3: Get inverse of assumed unit quaternion; same as get conjugate.
//	param q_out: output quaternion to store inverse
//	param q: input quaternion whose inverse to get
//	return: q_out
A3_INLINE a3real4r a3quatGetInverseIgnoreScale(a3real4p q_out, const a3real4p q);

// A3: Conjugate quaternion (negate vector part).
//	param q_inout: quaternion to conjugate and store
//	return: q_inout
A3_INLINE a3real4r a3quatConjugate(a3real4p q_inout);

// A3: Invert quaternion.
//	param q_inout: quaternion to invert and store
//	return: q_inout
A3_INLINE a3real4r a3quatInvert(a3real4p q_inout);

// A3: Invert quaternion and get scale.
//	param q_inout: quaternion to invert and store
//	param s_out: pointer to variable to capture uniform scale
//	return: q_inout
A3_INLINE a3real4r a3quatInvertGetScale(a3real4p q_inout, a3real *s_out);

// A3: Invert assumed unit quaternion; same as conjugate.
//	param q_inout: quaternion to invert and store
//	return: q_inout
A3_INLINE a3real4r a3quatInvertIgnoreScale(a3real4p q_inout);

// A3: Get the axis and angle of rotation from a quaternion.
//	param q: quaternion from which to get axis and angle
//	param unitAxis_out: variable to capture unit axis
//	param degrees_out: variable to capture rotation amount in degrees
//	return: q
A3_INLINE a3real4rk a3quatGetAxisAngle(const a3real4p q, a3real3p unitAxis_out, a3real *degrees_out);

// A3: Get the axis and angle of rotation and uniform scale from a quaternion.
//	param q: quaternion from which to get axis, angle and scale
//	param unitAxis_out: variable to capture unit axis
//	param degrees_out: pointer to variable to capture rotation angle in degrees
//	param s_out: pointer to variable to capture uniform scale
//	return: q
A3_INLINE a3real4rk a3quatGetAxisAngleScale(const a3real4p q, a3real3p unitAxis_out, a3real *degrees_out, a3real *s_out);

// A3: Get the axis and angle of rotation of an assumed unit quaternion.
//	param q: quaternion from which to get axis and angle
//	param unitAxis_out: variable to capture unit axis
//	param degrees_out: pointer to variable to capture rotation angle in degrees
//	return: q
A3_INLINE a3real4rk a3quatGetAxisAngleIgnoreScale(const a3real4p q, a3real3p unitAxis_out, a3real *degrees_out);

// A3: Get Euler angles from a quaternion in XYZ order.
//	param q: quaternion from which to get Euler angles in degrees
//	param degrees_x_out: pointer to variable to capture angle about X axis
//	param degrees_y_out: pointer to variable to capture angle about Y axis
//	param degrees_z_out: pointer to variable to capture angle about Z axis
//	return: q
A3_INLINE a3real4rk a3quatGetEulerXYZ(const a3real4p q, a3real *degrees_x_out, a3real *degrees_y_out, a3real *degrees_z_out);

// A3: Get Euler angles from a quaternion in ZYX order.
//	param q: quaternion from which to get Euler angles in degrees
//	param degrees_x_out: pointer to variable to capture angle about X axis
//	param degrees_y_out: pointer to variable to capture angle about Y axis
//	param degrees_z_out: pointer to variable to capture angle about Z axis
//	return: q
A3_INLINE a3real4rk a3quatGetEulerZYX(const a3real4p q, a3real *degrees_x_out, a3real *degrees_y_out, a3real *degrees_z_out);

// A3: Get Euler angles and uniform scale from a quaternion in XYZ order.
//	param q: quaternion from which to get Euler angles in degrees
//	param degrees_x_out: pointer to variable to capture angle about X axis
//	param degrees_y_out: pointer to variable to capture angle about Y axis
//	param degrees_z_out: pointer to variable to capture angle about Z axis
//	param s_out: pointer to variable to capture uniform scale
//	return: q
A3_INLINE a3real4rk a3quatGetEulerXYZScale(const a3real4p q, a3real *degrees_x_out, a3real *degrees_y_out, a3real *degrees_z_out, a3real *s_out);

// A3: Get Euler angles and uniform scale from a quaternion in ZYX order.
//	param q: quaternion from which to get Euler angles in degrees
//	param degrees_x_out: pointer to variable to capture angle about X axis
//	param degrees_y_out: pointer to variable to capture angle about Y axis
//	param degrees_z_out: pointer to variable to capture angle about Z axis
//	param s_out: pointer to variable to capture uniform scale
//	return: q
A3_INLINE a3real4rk a3quatGetEulerZYXScale(const a3real4p q, a3real *degrees_x_out, a3real *degrees_y_out, a3real *degrees_z_out, a3real *s_out);

// A3: Get Euler angles from an assumed quaternion in XYZ order.
//	param q: quaternion from which to get Euler angles in degrees
//	param degrees_x_out: pointer to variable to capture angle about X axis
//	param degrees_y_out: pointer to variable to capture angle about Y axis
//	param degrees_z_out: pointer to variable to capture angle about Z axis
//	return: q
A3_INLINE a3real4rk a3quatGetEulerXYZIgnoreScale(const a3real4p q, a3real *degrees_x_out, a3real *degrees_y_out, a3real *degrees_z_out);

// A3: Get Euler angles from an assumed quaternion in ZYX order.
//	param q: quaternion from which to get Euler angles in degrees
//	param degrees_x_out: pointer to variable to capture angle about X axis
//	param degrees_y_out: pointer to variable to capture angle about Y axis
//	param degrees_z_out: pointer to variable to capture angle about Z axis
//	return: q
A3_INLINE a3real4rk a3quatGetEulerZYXIgnoreScale(const a3real4p q, a3real *degrees_x_out, a3real *degrees_y_out, a3real *degrees_z_out);

// A3: Get uniform scale from a quaternion.
//	param q: quaternion from which to get uniform scale
//	param s_out: pointer to variable to capture uniform scale
//	return: q
A3_INLINE a3real4rk a3quatGetScale(const a3real4p q, a3real *s_out);


// A3: Convert quaternion to 3D matrix.
//	param m_out: output matrix converted from quaternion
//	param q: input quaternion to convert
//	return: m_out
A3_INLINE a3real3x3r a3quatConvertToMat3(a3real3x3p m_out, const a3real4p q);

// A3: Convert quaternion to 4D matrix.
//	param m_out: output matrix converted from quaternion
//	param q: input quaternion to convert
//	return: m_out
A3_INLINE a3real4x4r a3quatConvertToMat4(a3real4x4p m_out, const a3real4p q);

// A3: Convert quaternion to 4D matrix and add translation vector.
//	param m_out: output matrix converted from quaternion
//	param q: input quaternion to convert
//	param translate: translation vector to store in matrix
//	return: m_out
A3_INLINE a3real4x4r a3quatConvertToMat4Translate(a3real4x4p m_out, const a3real4p q, const a3real3p translate);

// A3: Convert 3D matrix to quaternion.
//	param q_out: output quaternion converted from matrix
//	param m: input matrix to convert
//	return: q_out
A3_INLINE a3real4r a3quatConvertFromMat3(a3real4p q_out, const a3real3x3p m);

// A3: Convert 4D matrix to quaternion.
//	param q_out: output quaternion converted from matrix
//	param m: input matrix to convert
//	return: q_out
A3_INLINE a3real4r a3quatConvertFromMat4(a3real4p q_out, const a3real4x4p m);

// A3: Convert 4D matrix to quaternion and translation vector.
//	param q_out: output quaternion converted from matrix
//	param translate_out: vector to capture translation vector
//	param m: input matrix to convert
//	return: q_out
A3_INLINE a3real4r a3quatConvertFromMat4Translate(a3real4p q_out, a3real3p translate_out, const a3real4x4p m);

// A3: Convert 3D matrix to quaternion alternative; slower but safer.
//	param q_out: output quaternion converted from matrix
//	param m: input matrix to convert
//	return: q_out
A3_INLINE a3real4r a3quatConvertFromMat3Safe(a3real4p q_out, const a3real3x3p m);

// A3: Convert 4D matrix to quaternion; slower but safer.
//	param q_out: output quaternion converted from matrix
//	param m: input matrix to convert
//	return: q_out
A3_INLINE a3real4r a3quatConvertFromMat4Safe(a3real4p q_out, const a3real4x4p m);

// A3: Convert 4D matrix to quaternion and translation vector; slower but safer.
//	param q_out: output quaternion converted from matrix
//	param translate_out: vector to capture translation vector
//	param m: input matrix to convert
//	return: q_out
A3_INLINE a3real4r a3quatConvertFromMat4SafeTranslate(a3real4p q_out, a3real3p translate_out, const a3real4x4p m);


// A3: Calculate quaternion product.
//	param q_out: output quaternion to store product
//	param qL: input left quaternion
//	param qR: input right quaternion
//	return: q_out
A3_INLINE a3real4r a3quatProduct(a3real4p q_out, const a3real4p qL, const a3real4p qR);

// A3: Concatenate (multiply) quaternions and store in left input.
//	param qL_inout: left input quaternion to store product
//	param qR: input right quaternion
//	return: qL_inout
A3_INLINE a3real4r a3quatConcatL(a3real4p qL_inout, const a3real4p qR);

// A3: Concatenate (multiply) quaternions and store in right input.
//	param qL: input left quaternion
//	param qR_inout: right input quaternion to store product
//	return: qR_inout
A3_INLINE a3real4r a3quatConcatR(const a3real4p qL, a3real4p qR_inout);


// A3: Calculate vector rotated by assumed unit quaternion.
//	param v_out: output rotated vector
//	param v: input vector to rotate by quaternion
//	param q: quaternion to rotate vector
//	return: v_out
A3_INLINE a3real3r a3quatVec3GetRotatedIgnoreScale(a3real3p v_out, const a3real3p v, const a3real4p q);

// A3: Calculate vector transformed by quaternion.
//	param v_out: output transformed vector
//	param v: input vector to transform by quaternion
//	param q: quaternion to transform vector
//	return: v_out
A3_INLINE a3real3r a3quatVec3GetRotatedScaled(a3real3p v_out, const a3real3p v, const a3real4p q);

// A3: Calculate vector rotated by quaternion, correct scale.
//	param v_out: output rotated vector
//	param v: input vector to rotate by quaternion
//	param q: quaternion to rotate vector
//	return: v_out
A3_INLINE a3real3r a3quatVec3GetRotated(a3real3p v_out, const a3real3p v, const a3real4p q);

// A3: Rotate vector by assumed unit quaternion.
//	param v_inout: vector to rotate and store
//	param q: quaternion to rotate vector
//	return: v_inout
A3_INLINE a3real3r a3quatVec3RotateIgnoreScale(a3real3p v_inout, const a3real4p q);

// A3: Transform vector by quaternion.
//	param v_inout: vector to transform and store
//	param q: quaternion to transform vector
//	return: v_inout
A3_INLINE a3real3r a3quatVec3RotateScale(a3real3p v_inout, const a3real4p q);

// A3: Rotate vector by quaternion, correct scale.
//	param v_inout: vector to rotate and store
//	param q: quaternion to rotate vector
//	return: v_inout
A3_INLINE a3real3r a3quatVec3Rotate(a3real3p v_inout, const a3real4p q);


// A3: Calculate product of a pure quaternion and a regular quaternion.
//	param q_out: output quaternion to store product
//	param qPureL: input left pure quaternion
//	param qR: input right quaternion
//	return: q_out
A3_INLINE a3real4r a3quatProductPureL(a3real4p q_out, const a3real4p qPureL, const a3real4p qR);

// A3: Calculate product of a regular quaternion and a pure quaternion.
//	param q_out: output quaternion to store product
//	param qL: input left quaternion
//	param qPureR: input right pure quaternion
//	return: q_out
A3_INLINE a3real4r a3quatProductPureR(a3real4p q_out, const a3real4p qL, const a3real4p qPureR);

// A3: Concatenate (multiply) a pure quaternion and a regular quaternion.
//	param qPureL_inout: left pure quaternion to concatenate and store
//	param qR: input right quaternion
//	return: qPureL_inout
A3_INLINE a3real4r a3quatConcatLPureL(a3real4p qPureL_inout, const a3real4p qR);

// A3: Concatenate (multiply) a pure quaternion and a regular quaternion.
//	param qPureL: input left pure quaternion
//	param qR_inout: right quaternion to concatenate and store
//	return: qR_inout
A3_INLINE a3real4r a3quatConcatRPureL(const a3real4p qPureL, a3real4p qR_inout);

// A3: Concatenate (multiply) a regular quaternion and a pure quaternion.
//	param qL_inout: left quaternion to concatenate and store
//	param qPureR: input right pure quaternion
//	return: qL_inout
A3_INLINE a3real4r a3quatConcatLPureR(a3real4p qL_inout, const a3real4p qPureR);

// A3: Concatenate (multiply) a regular quaternion and a pure quaternion.
//	param qL: input left quaternion
//	param qPureR_inout: right pure quaternion to concatenate and store
//	return: qPureR_inout
A3_INLINE a3real4r a3quatConcatRPureR(const a3real4p qL, a3real4p qPureR_inout);


// A3: Spherical linear interpolation ("SLERP") for quaternions.
//	param q_out: output quaternion to store interpolation result
//	param q0: initial quaternion
//	param q1: goal/end quaternion
//	param param: interpolation parameter (t); 
//		result is q0 when t=0 and q1 when t=1
//	return: q_out
A3_INLINE a3real4r a3quatSlerp(a3real4p q_out, const a3real4p q0, const a3real4p q1, const a3real param);

// A3: Spherical linear interpolation ("SLERP") for assumed unit quaternions.
//	param q_out: output quaternion to store interpolation result
//	param q0: initial quaternion
//	param q1: goal/end quaternion
//	param param: interpolation parameter (t); 
//		result is q0 when t=0 and q1 when t=1
//	return: q_out
A3_INLINE a3real4r a3quatSlerpUnit(a3real4p q_out, const a3real4p q0, const a3real4p q1, const a3real param);

// A3: Spherical linear interpolation ("SLERP") from identity to input.
//	param q_out: output quaternion to store interpolation result
//	param q1: goal/end quaternion
//	param param: interpolation parameter (t); 
//		result is identity when t=0 and q1 when t=1
//	return: q_out
A3_INLINE a3real4r a3quatSlerpIdentityQ0(a3real4p q_out, const a3real4p q1, const a3real param);

// A3: Spherical linear interpolation ("SLERP") from input to identity.
//	param q_out: output quaternion to store interpolation result
//	param q0: initial quaternion
//	param param: interpolation parameter (t); 
//		result is q0 when t=0 and identity when t=1
//	return: q_out
A3_INLINE a3real4r a3quatSlerpIdentityQ1(a3real4p q_out, const a3real4p q0, const a3real param);

// A3: Spherical linear interpolation ("SLERP") from identity to unit input.
//	param q_out: output quaternion to store interpolation result
//	param q1: goal/end quaternion
//	param param: interpolation parameter (t); 
//		result is identity when t=0 and q1 when t=1
//	return: q_out
A3_INLINE a3real4r a3quatSlerpUnitIdentityQ0(a3real4p q_out, const a3real4p q1, const a3real param);

// A3: Spherical linear interpolation ("SLERP") from unit input to identity.
//	param q_out: output quaternion to store interpolation result
//	param q0: initial quaternion
//	param param: interpolation parameter (t); 
//		result is q0 when t=0 and identity when t=1
//	return: q_out
A3_INLINE a3real4r a3quatSlerpUnitIdentityQ1(a3real4p q_out, const a3real4p q0, const a3real param);


// A3: Gram-Schmidt orthonogonalization for multiple quaternions.
//	param q0_out: result of Gram-Schmidt process on first quaternion
//	param q1_out: result of Gram-Schmidt process on second quaternion
//	param q2_out: result of Gram-Schmidt process on third quaternion
//	param q3_out: result of Gram-Schmidt process on fourth quaternion
//	param q0: first quaternion to orthogonalize
//	param q1: second quaternion to orthogonalize
//	param q2: third quaternion to orthogonalize
//	param q3: fourth quaternion to orthogonalize
//	param qBase: fixed base quaternion to orthogonalize against
//	return: q3_out
A3_INLINE a3real4r a3quatGramSchmidtOrtho4(a3real4p q0_out, a3real4p q1_out, a3real4p q2_out, a3real4p q3_out, const a3real4p q0, const a3real4p q1, const a3real4p q2, const a3real4p q3, const a3real4p qBase);

// A3: Gram-Schmidt orthonogonalize multiple quaternions.
//	param q0_inout: first quaternion to orthogonalize and store
//	param q1_inout: second quaternion to orthogonalize and store
//	param q2_inout: third quaternion to orthogonalize and store
//	param q3_inout: fourth quaternion to orthogonalize and store
//	param qBase: fixed base quaternion to orthogonalize against
//	return: q3_out
A3_INLINE a3real4r a3quatGramSchmidt4(a3real4p q0_inout, a3real4p q1_inout, a3real4p q2_inout, a3real4p q3_inout, const a3real4p qBase);


//-----------------------------------------------------------------------------

#ifndef A3_OPEN_SOURCE
// pre-defined constant for identity quaternion
// extern if closed-source, exposed if open-source
///
extern const a3quat a3quat_identity;
#endif	// !A3_OPEN_SOURCE


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3quaternion_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_QUATERNION_H