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

	a3config.h
	Global configurations and preprocessing.
*/

#ifndef __ANIMAL3D_CONFIG_H
#define __ANIMAL3D_CONFIG_H


//-----------------------------------------------------------------------------
// KEYWORDS & COMPILER TAGS

// macros for 32 or 64 bit (should be exclusive)
#if (WIN64 || _x64 || _M_X64 || __LP64__ || __LLP64__ || __arm64__ || __x86_64__)
#define A3_64_BIT	1
#define A3_ARCH		64
#else	// !(WIN64 || x64 || _M_X64 || __LP64__ || __LLP64__ || __arm64__ || __x86_64__)
#define A3_32_BIT	1
#define A3_ARCH		32
#endif	// (WIN64 || x64 || _M_X64 || __LP64__ || __LLP64__ || __arm64__ || __x86_64__)


// macros for debug and/or release
#ifdef _DEBUG
#define A3_DEBUG	1
#define A3_RELEASE	0
#else	// !_DEBUG
#ifndef NDEBUG
#define A3_DEBUG	1
#define A3_RELEASE	0
#endif	// !NDEBUG
#define A3_DEBUG	0
#define A3_RELEASE	1
#endif	// _DEBUG


// inline declaration
#ifdef A3_OPEN_SOURCE
#define A3_INLINE	static inline
#define A3_GLOBAL	static
#else	// !A3_OPEN_SOURCE
#define A3_INLINE	inline
#define A3_GLOBAL
#endif	// A3_OPEN_SOURCE


// begin and end definitions, namespace macros (if applicable)
#ifdef __cplusplus
#define A3_BEGIN		//namespace a3dm {
#define A3_END			//}
#define A3_BEGIN_DECL	A3_BEGIN	extern "C" {
#define A3_END_DECL		A3_END	}
#define A3_BEGIN_IMPL	A3_BEGIN
#define A3_END_IMPL		A3_END
#define A3				//a3dm::
#else	// !__cplusplus
#define A3_BEGIN		
#define A3_END		
#define A3_BEGIN_DECL	
#define A3_END_DECL	
#define A3_BEGIN_IMPL	
#define A3_END_IMPL	
#define A3			
#endif	// __cplusplus



//-----------------------------------------------------------------------------
// FLOAT MANAGEMENT
//	order of precedence: closed-source, intrinsic, f64, f128

// precompiled binaries in use; cannot use intrin, 64-bit or 128-bit real...
//	...force-off intrin, 64-bit or 128-bit float
#ifndef A3_OPEN_SOURCE

#ifdef A3_USING_INTRIN
#pragma message ("\
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n\
/// ****A3 WARNING**** User specified A3_USING_INTRIN not supported with precompiled library; undefining INTRIN macro.      /// \n\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   ")
#undef A3_USING_INTRIN
#endif	// A3_USING_INTRIN

#ifdef A3_REAL_F64
#pragma message ("\
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n\
/// ****A3 WARNING**** User specified A3_REAL_F64 not supported with precompiled library; undefining F64 macro.             /// \n\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   ")
#undef A3_REAL_F64
#endif	// A3_REAL_F64

#ifdef A3_REAL_F128
#pragma message ("\
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n\
/// ****A3 WARNING**** User specified A3_REAL_F128 not supported with precompiled library; undefining F128 macro.           /// \n\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   ")
#undef A3_REAL_F128
#endif	// A3_REAL_F128

#endif	// !A3_OPEN_SOURCE


// if user wants to use intrinsics but does not want 32-bit float...
//	...force-off 64-bit or 128-bit float
#ifdef A3_USING_INTRIN

#ifdef A3_REAL_F64
#pragma message ("\
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n\
/// ****A3 WARNING**** User specified A3_USING_INTRIN which takes precedence over A3_REAL_F64; undefining F64 macro.        /// \n\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   ")
#undef A3_REAL_F64
#endif	// A3_REAL_F64

#ifdef A3_REAL_F128
#pragma message ("\
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n\
/// ****A3 WARNING**** User specified A3_USING_INTRIN which takes precedence over A3_REAL_F128; undefining F128 macro.      /// \n\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   ")
#undef A3_REAL_F128
#endif	// A3_REAL_F128

#endif	// A3_USING_INTRIN


// if user opts to use 64-bit floating point *and* 128-bit floating point...
//	...force-off 128-bit float
#ifdef A3_REAL_F64

#ifdef A3_REAL_F128
#pragma message ("\
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// \n\
/// ****A3 WARNING**** User specified A3_REAL_F64 which takes precedence over A3_REAL_F128; undefining F128 macro.          /// \n\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   ")
#undef A3_REAL_F128
#endif	// A3_REAL_F128

#endif	// A3_REAL_F64


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_CONFIG_H