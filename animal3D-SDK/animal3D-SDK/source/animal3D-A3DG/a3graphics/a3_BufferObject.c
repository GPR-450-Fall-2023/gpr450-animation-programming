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
	
	a3_BufferObject.c
	Definitions for common buffer object functions.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_BufferObject.h"


//-----------------------------------------------------------------------------
// internal utility declarations

a3ret a3bufferInternalFill(a3_BufferObject* buffer, const a3i32 section, const a3ui32 start, const a3ui32 end, const a3ui32 size, const void* data, a3ui32* start_out_opt);

void a3bufferInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr);


//-----------------------------------------------------------------------------

a3ret a3bufferAppend(a3_BufferObject* buffer, a3i32 section, const a3ui32 size, const void* data, a3ui32* start_out_opt)
{
	a3ui32 start_tmp;

	if (buffer)
	{
		section = section ? 1 : 0;
		start_tmp = buffer->used[section];
		return a3bufferInternalFill(buffer, section, start_tmp, start_tmp + size, size, data, start_out_opt);
	}
	return -1;
}

a3ret a3bufferAppendOffset(a3_BufferObject* buffer, a3i32 section, const a3ui32 offset, const a3ui32 size, const void* data, a3ui32* start_out_opt)
{
	a3ui32 start_tmp;

	if (buffer)
	{
		section = section ? 1 : 0;
		start_tmp = buffer->used[section] + offset;
		return a3bufferInternalFill(buffer, section, start_tmp, start_tmp + size, size, data, start_out_opt);
	}
	return -1;
}

a3ret a3bufferRefill(a3_BufferObject* buffer, a3i32 section, const a3ui32 size, const void* data)
{
	if (buffer)
	{
		section = section ? 1 : 0;
		return a3bufferInternalFill(buffer, section, 0, size, size, data, 0);
	}
	return -1;
}

a3ret a3bufferRefillOffset(a3_BufferObject* buffer, a3i32 section, const a3ui32 offset, const a3ui32 size, const void* data)
{
	if (buffer)
	{
		section = section ? 1 : 0;
		return a3bufferInternalFill(buffer, section, offset, offset + size, size, data, 0);
	}
	return -1;
}

a3ret a3bufferHandleUpdateReleaseCallback(a3_BufferObject *buffer)
{
	if (buffer)
		return a3handleSetReleaseFunc(buffer->handle, a3bufferInternalReleaseFunc);
	return -1;
}


//-----------------------------------------------------------------------------
