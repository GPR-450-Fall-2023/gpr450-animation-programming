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
	
	_a3_dylib_config_export.h
	Multi-platform dynamic library export/import configuration.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_DYLIB_CONFIG_EXPORT_H
#define __ANIMAL3D_DYLIB_CONFIG_EXPORT_H


// platform-specific tags
#if (defined _WINDOWS || defined _WIN32 || !defined __GNUC__)
// Windows / non-GNU-C
#define A3DYLIBEXPORT __declspec(dllexport)
#define A3DYLIBIMPORT __declspec(dllimport)
#else	// !(defined _WINDOWS || defined _WIN32 || !defined __GNUC__)
// Unix (Mac/Linux) / GNU-C
#define A3DYLIBEXPORT __attribute__((visibility("default")))
#define A3DYLIBIMPORT __attribute__((weak_import))
#endif	// (defined _WINDOWS || defined _WIN32 || !defined __GNUC__)


// universal export/import tag definition
#ifdef ANIMAL3DDEMOPLUGIN_EXPORTS
#define A3DYLIBSYMBOL A3DYLIBEXPORT
#else	// !ANIMAL3DDEMOPLUGIN_EXPORTS
#ifdef	// ANIMAL3DDEMOPLUGIN_IMPORTS
#define A3DYLIBSYMBOL A3DYLIBIMPORT
#else	// !ANIMAL3DDEMOPLUGIN_IMPORTS
#define A3DYLIBSYMBOL 
#endif	// ANIMAL3DDEMOPLUGIN_IMPORTS
#endif	// ANIMAL3DDEMOPLUGIN_EXPORTS


#endif	// !__ANIMAL3D_DYLIB_CONFIG_EXPORT_H