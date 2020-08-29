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
	
	a3_dylib_config_load.h
	Multi-platform dynamic library loading configuration.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_DYLIB_CONFIG_LOAD_H
#define __ANIMAL3D_DYLIB_CONFIG_LOAD_H


// platform-specific tags
#if (defined _WINDOWS || defined _WIN32)
// Windows
#include <Windows.h>
#define A3DYLIBHANDLE								HMODULE
#define A3DYLIBSYMBOL								FARPROC
#define A3DYLIBLOAD(lib_path)						LoadLibraryA(lib_path)
#define A3DYLIBUNLOAD(lib_handle)					FreeLibrary(lib_handle)
#define A3DYLIBGETSYMBOL(lib_handle, symbol_name)	GetProcAddress(lib_handle, symbol_name)

#else	// !(defined _WINDOWS || defined _WIN32)
// Unix (Mac/Linux)
#include <dlfcn.h>
#define A3DYLIBHANDLE								void *
#define A3DYLIBSYMBOL								void *
#define A3DYLIBLOAD(lib_path)						dlopen(lib_path, RTLD_NOW)
#define A3DYLIBUNLOAD(lib_handle)					dlclose(lib_handle)
#define A3DYLIBGETSYMBOL(lib_handle, symbol_name)	dlsym(lib_handle, symbol_name)

#endif	// (defined _WINDOWS || defined _WIN32)


// universal wrapper functions
///

// A3 load library
inline A3DYLIBHANDLE a3dylibLoad(const a3byte *libPath)
{
	return A3DYLIBLOAD(libPath);
}

// A3 unload library
inline a3ret a3dylibUnload(A3DYLIBHANDLE libHandle)
{
	return A3DYLIBUNLOAD(libHandle);
}

// A3 get symbol
inline A3DYLIBSYMBOL a3dylibGetSymbol(A3DYLIBHANDLE libHandle, const a3byte *symbolName)
{
	return A3DYLIBGETSYMBOL(libHandle, symbolName);
}


#endif	// !__ANIMAL3D_DYLIB_CONFIG_LOAD_H