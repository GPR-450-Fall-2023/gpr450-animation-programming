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
	
	a3_GraphicsObjectHandle.inl
	Inline definitions for reference-counting object handle.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_GRAPHICSOBJECTHANDLE_H
#ifndef __ANIMAL3D_GRAPHICSOBJECTHANDLE_INL
#define __ANIMAL3D_GRAPHICSOBJECTHANDLE_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3handleCreateHandle(a3_GraphicsObjectHandle *handle_out, const a3_GraphicsObjectReleaseFunc releaseFunc_opt, const a3byte name_opt[32], const a3ui32 handleValue, const a3ui32 objectCount)
{
	// if valid
	if (handle_out && handleValue)
	{
		// if not already in-use
		if (!handle_out->handle)
		{
			handle_out->releaseFunc = releaseFunc_opt;
			handle_out->objectCount = objectCount;
			handle_out->refCount = 0;
			a3handleSetName(handle_out, name_opt);
			return (handle_out->handle = handleValue);
		}
	}
	return -1;
}

A3_INLINE a3ret a3handleSetReleaseFunc(a3_GraphicsObjectHandle *handle, const a3_GraphicsObjectReleaseFunc releaseFunc_opt)
{
	if (handle)
	{
		handle->releaseFunc = releaseFunc_opt;
		return 1;
	}
	return -1;
}

A3_INLINE a3ret a3handleIncrementCount(a3_GraphicsObjectHandle *handle)
{
	// if valid, increment count and return
	if (handle && handle->handle)
		return (++handle->refCount);
	return -1;
}

A3_INLINE a3ret a3handleDecrementCount(a3_GraphicsObjectHandle *handle)
{
	// if valid
	if (handle && handle->handle)
	{
		// decrement count; if invalidated, release
		if (--handle->refCount <= 0)
			return a3handleRelease(handle);
		return handle->refCount;
	}
	return -1;
}

A3_INLINE a3ret a3handleRelease(a3_GraphicsObjectHandle *handle)
{
	// if valid
	if (handle && handle->handle)
	{
		if (handle->releaseFunc)
			handle->releaseFunc(handle->objectCount, handle->handlePtr);

		// reset
		handle->releaseFunc = 0;
		handle->objectCount = 0;
		handle->refCount = 0;
		handle->handle = 0;
		return 0;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_GRAPHICSOBJECTHANDLE_INL
#endif	// __ANIMAL3D_GRAPHICSOBJECTHANDLE_H