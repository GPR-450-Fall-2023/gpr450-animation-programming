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

	a3types_integer.h
	Shorthand integral type definitions.
*/

#ifndef __ANIMAL3D_TYPES_INTEGER_H
#define __ANIMAL3D_TYPES_INTEGER_H


#include "a3config.h"


A3_BEGIN


//-----------------------------------------------------------------------------

// signed integer types: 
typedef __int8				a3i8;
typedef __int16				a3i16;
typedef __int32				a3i32;
typedef __int64				a3i64;

// unsigned integer types: 
typedef unsigned __int8		a3ui8;
typedef unsigned __int16	a3ui16;
typedef unsigned __int32	a3ui32;
typedef unsigned __int64	a3ui64;


// aliases for integer types: 
typedef a3i8				a3byte;
typedef a3ui8				a3ubyte;
typedef a3ui16				a3unicode;
typedef a3i32				a3boolean;
typedef a3i32				a3integer;
typedef a3ui32				a3index;
typedef a3ui32				a3count;
typedef a3i64				a3biginteger;
typedef a3ui64				a3bigindex;
typedef a3ui64				a3bigcount;
typedef a3i32				a3ret;


#ifdef __SIZEOF_INT128__
// huge integers
typedef __int128			a3i128;
typedef unsigned __int128	a3ui128;
typedef a3i128				a3hugeinteger;
typedef a3ui128				a3hugeindex;
#endif	// __SIZEOF_INT128__


// "pointer" as integer type
#if A3_32_BIT
typedef a3ui32				a3address;
typedef a3i32				a3addressdiff;
#else	// !A3_32_BIT
typedef a3ui64				a3address;
typedef a3i64				a3addressdiff;
#endif	// A3_32_BIT
typedef a3addressdiff		a3span, a3size;


//-----------------------------------------------------------------------------


A3_END


#endif	// !__ANIMAL3D_TYPES_INTEGER_H