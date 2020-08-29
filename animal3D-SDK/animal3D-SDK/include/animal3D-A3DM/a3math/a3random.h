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

	a3random.h
	Declarations for random number generator functions.
*/

#ifndef __ANIMAL3D_A3DM_RANDOM_H
#define __ANIMAL3D_A3DM_RANDOM_H


#include "animal3D/a3/a3config.h"
#include "animal3D/a3/a3macros.h"
#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


A3_BEGIN_DECL


//-----------------------------------------------------------------------------

// A3: Get maximum random number.
//	return: max random number
A3_INLINE a3integer a3randomGetMax();

// A3: Get current random seed (also last random returned).
//	return: random seed (last random returned)
A3_INLINE a3integer a3randomGetSeed();

// A3: Set random seed, defines new seed for random number generator.
//	param seed: new random seed
//	return: old random seed (last random returned)
A3_INLINE a3integer a3randomSetSeed(const a3integer seed);


// A3: Quick n' dirty random number generator that's been around since the 
//		70's; don't forget to set the seed for actual randomness.
//	return: random integer in [0, max)
A3_INLINE a3integer a3randomInt();

// A3: Generate random decimal number.
//	return: random real number in [0, max)
A3_INLINE a3real a3random();

// A3: Generate non-negative normalized random decimal number.
//	return: random real number in [0, 1)
A3_INLINE a3real a3randomNormalized();

// A3: Generate symmetric (bi-directional) normalized random decimal number. 
//	return: random real number in [-1, 1)
A3_INLINE a3real a3randomSymmetric();


// A3: Generate non-negative ranged random decimal number.
//	param nMax: maximum real number in range
//	return: random real number in [0, nMax)
A3_INLINE a3real a3randomMax(const a3real nMax);

// A3: Generate non-negative ranged random integer.
//	param nMax: maximum integer in range
//	return: random integer in [0, nMax)
A3_INLINE a3integer a3randomMaxInt(const a3integer nMax);


// A3: Generate ranged random decimal number.
//	param nMin: minimum real number in range
//	param nMax: maximum real number in range
//	return: random real number in [nMin, nMax)
A3_INLINE a3real a3randomRange(const a3real nMin, const a3real nMax);

// A3: Generate non-negative ranged random decimal number.
//	param nMin: minimum integer in range
//	param nMax: maximum integer in range
//	return: random integer in [nMin, nMax)
A3_INLINE a3integer a3randomRangeInt(const a3integer nMin, const a3integer nMax);


//-----------------------------------------------------------------------------


A3_END_DECL


#ifdef A3_OPEN_SOURCE
#include "_inl/a3random_impl.inl"
#endif	// A3_OPEN_SOURCE

#endif	// !__ANIMAL3D_A3DM_RANDOM_H