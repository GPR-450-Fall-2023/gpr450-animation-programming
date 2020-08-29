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
	
	a3_BufferObject.inl
	Inline definitions for buffer object.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_BUFFEROBJECT_H
#ifndef __ANIMAL3D_BUFFEROBJECT_INL
#define __ANIMAL3D_BUFFEROBJECT_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3bufferValidateBlockSize(const a3_BufferObject *buffer, a3i32 section, const a3ui32 size)
{
	section = section ? 1 : 0;
	if (buffer && buffer->handle->handle && size)
		return ((buffer->used[section] + size) <= buffer->split[section]);
	return -1;
}

A3_INLINE a3ret a3bufferGetCurrentOffset(const a3_BufferObject *buffer, a3i32 section)
{
	if (buffer && buffer->handle->handle && buffer->size)
		return (buffer->used[section ? 1 : 0]);
	return -1;
}

A3_INLINE a3ret a3bufferReference(a3_BufferObject *buffer)
{
	if (buffer)
		return a3handleIncrementCount(buffer->handle);
	return -1;
}

A3_INLINE a3ret a3bufferRelease(a3_BufferObject *buffer)
{
	if (buffer)
	{
		a3i32 ret = a3handleDecrementCount(buffer->handle);
		if (ret == 0)
			buffer->size = buffer->split[0] = buffer->split[1] = buffer->used[0] = buffer->used[1] = buffer->internalBinding = 0;
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_BUFFEROBJECT_H
#endif	// __ANIMAL3D_BUFFEROBJECT_H