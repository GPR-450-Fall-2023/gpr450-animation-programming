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
	
	a3_GraphicsObjectHandle.c
	Definitions for generic reference counting handle.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_GraphicsObjectHandle.h"

#include <string.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------

a3ret a3handleSetName(a3_GraphicsObjectHandle *handle, const a3byte name_opt[32])
{
	static a3ui32 handleNameCtr = 0;
	if (handle)
	{
		if (name_opt && *name_opt)
		{
			strncpy(handle->name, name_opt, sizeof(handle->name));
		}
		else
		{
			strcpy(handle->name, "a3handle_");
			_itoa(handleNameCtr++, handle->name + 9, 10);
		}
		handle->name[sizeof(handle->name) - 1] = 0;
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------
// removed from header

/*
	//	Single/Array: actual OpenGL-defined release functions (closed-source)
	typedef void(__stdcall *a3_GraphicsObjectReleaseFunc_Single)(a3ui32);
	typedef void(__stdcall *a3_GraphicsObjectReleaseFunc_Array)(a3i32, a3ui32 *);
*/

/*
		union {
			a3_GraphicsObjectReleaseFunc releaseFunc;
			a3_GraphicsObjectReleaseFunc_Single releaseFunc1;
			a3_GraphicsObjectReleaseFunc_Array releaseFuncA;
		};

		a3i32 release;
*/

/*
	// A3: Create generic handle with a single-object release function.
	//	param handle_out: non-null pointer to graphics object handle container
	//	param releaseFunc: non-null pointer to release function with 
	//		return type void and one integer pointer argument
	//	param handleValue: non-zero initial value of handle
	//	return: handleValue if successfully created
	//	return: -1 if invalid params or if already in-use
	a3ret a3handleCreateHandleSingle(a3_GraphicsObjectHandle *handle_out, const a3_GraphicsObjectReleaseFunc_Single releaseFunc, const a3ui32 handleValue);

	// A3: Create generic handle with an object array release function.
	//	param handle_out: non-null pointer to graphics object handle container
	//	param releaseFunc: non-null pointer to release function with 
	//		return type void, integer argument and integer pointer argument
	//	param handleValue: non-zero initial value of handle
	//	return: handleValue if successfully created
	//	return: -1 if invalid params or if already in-use
	a3ret a3handleCreateHandleArray(a3_GraphicsObjectHandle *handle_out, const a3_GraphicsObjectReleaseFunc_Array releaseFunc, const a3ui32 handleValue);

	// A3: Update handle delete callback with predifined function.
	//	param releaseFunc: non-null pointer to release function with
	//		return type void and one integer pointer argument
	//	return: 1 if successfully updated
	//	return: -1 if invalid params
	a3ret a3handleSetReleaseFuncSingle(a3_GraphicsObjectHandle *handle, const a3_GraphicsObjectReleaseFunc_Single releaseFunc);

	// A3: Update handle delete callback with predefined array function.
	//	param releaseFunc: non-null pointer to release function with
	//		return type void, integer argument and integer pointer argument
	//	return: 1 if successfully updated
	//	return: -1 if invalid params
	a3ret a3handleSetReleaseFuncArray(a3_GraphicsObjectHandle *handle, const a3_GraphicsObjectReleaseFunc_Array releaseFunc);

*/


//-----------------------------------------------------------------------------
// removed from inline

/*
inline a3ret a3handleCreateHandleSingle(a3_GraphicsObjectHandle *handle_out, const a3_GraphicsObjectReleaseFunc_Single releaseFunc, const a3ui32 handleValue)
{
	if (handle_out && releaseFunc && handleValue)
	{
		if (!handle_out->handle)
		{
			handle_out->releaseFunc1 = releaseFunc;
			handle_out->release = 1;
			handle_out->refCount = 0;
			return (handle_out->handle = handleValue);
		}
	}
	return -1;
}

inline a3ret a3handleCreateHandleArray(a3_GraphicsObjectHandle *handle_out, const a3_GraphicsObjectReleaseFunc_Array releaseFunc, const a3ui32 handleValue)
{
	if (handle_out && releaseFunc && handleValue)
	{
		if (!handle_out->handle)
		{
			handle_out->releaseFuncA = releaseFunc;
			handle_out->release = 2;
			handle_out->refCount = 0;
			return (handle_out->handle = handleValue);
		}
	}
	return -1;
}

inline a3ret a3handleSetReleaseFuncSingle(a3_GraphicsObjectHandle *handle, const a3_GraphicsObjectReleaseFunc_Single releaseFunc)
{
	if (handle && releaseFunc)
	{
		handle->releaseFunc1 = releaseFunc;
		handle->release = 1;
		return 1;
	}
	return -1;
}

inline a3ret a3handleSetReleaseFuncArray(a3_GraphicsObjectHandle *handle, const a3_GraphicsObjectReleaseFunc_Array releaseFunc)
{
	if (handle && releaseFunc)
	{
		handle->releaseFuncA = releaseFunc;
		handle->release = 2;
		return 1;
	}
	return -1;
}

*/

/*
			// call release function
			switch (handle->release)
			{
			case 0:
				handle->releaseFunc(handle->handlePtr);
				break;
			case 1:
				handle->releaseFunc1(handle->handle);
				break;
			case 2:
				handle->releaseFuncA(1, handle->handlePtr);
				break;
			}
		}
*/

//-----------------------------------------------------------------------------
