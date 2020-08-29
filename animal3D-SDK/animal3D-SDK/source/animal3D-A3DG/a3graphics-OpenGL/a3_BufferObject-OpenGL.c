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
	
	a3_BufferObject-OpenGL.c
	Definitions for OpenGL buffer object.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_BufferObject.h"

#include "GL/glew.h"

#include <stdio.h>


//-----------------------------------------------------------------------------

inline a3ui16 a3bufferInternalFlag(const a3_BufferObjectType bufferType)
{
	static const a3ui16 bufferBindings[] = { GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER, };
	return bufferBindings[bufferType];
}

inline a3ui16 a3bufferInternalFillHint(const a3_BufferObjectType bufferType)
{
	static const a3ui16 bufferFillHint[] = { GL_STATIC_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW, };
	return bufferFillHint[bufferType];
}


void a3bufferInternalFillSub(a3i32 binding, a3ui32 start, a3ui32 size, const void *data)
{
	glBufferSubData(binding, start, size, data);
}

a3ret a3bufferInternalFill(a3_BufferObject* buffer, const a3i32 section, const a3ui32 start, const a3ui32 end, const a3ui32 size, const void* data, a3ui32* start_out_opt)
{
	const a3ui32 bHandle = buffer->handle->handle;

	if (bHandle && size)
	{
		// check if data will fit in section
		if (start <= buffer->split[section] && end <= buffer->split[section])
		{
			// bind and fill
			glBindBuffer(buffer->internalBinding, bHandle);
			glBufferSubData(buffer->internalBinding, start, size, data);
			glBindBuffer(buffer->internalBinding, 0);

			// output starting point
			if (start_out_opt)
				* start_out_opt = start;

			// determine new tail
			if (end > buffer->used[section])
				buffer->used[section] = data ? end : start;

			// done
			return size;
		}
		else
			printf("\n A3 ERROR (BUF %u \'%s\'): \n\t Insufficient storage; failed to store data.", buffer->handle->handle, buffer->handle->name);

		// fail
		return 0;
	}
	return -1;
}

void a3bufferInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr)
{
	glDeleteBuffers(count, handlePtr);
}


//-----------------------------------------------------------------------------

a3ret a3bufferCreate(a3_BufferObject *buffer_out, const a3byte name_opt[32], const a3_BufferObjectType bufferType, const a3ui32 size, const void *initialData_opt)
{
	a3_BufferObject ret = { 0 };
	a3ui32 handle;
	a3ui32 binding;
	a3ui32 fillHint;

	if (buffer_out && size)
	{
		// check uninitialized
		if (!buffer_out->handle->handle)
		{
			// generate buffer
			glGenBuffers(1, &handle);
			if (handle)
			{
				binding = a3bufferInternalFlag(bufferType);
				fillHint = a3bufferInternalFillHint(bufferType);

				// bind and allocate space
				glBindBuffer(binding, handle);
				glBufferData(binding, size, initialData_opt, fillHint);
				glBindBuffer(binding, 0);

				// configure
				a3handleCreateHandle(ret.handle, a3bufferInternalReleaseFunc, name_opt, handle, 1);
				ret.type = bufferType;
				ret.internalBinding = binding;
				ret.size = ret.split[0] = ret.split[1] = size;
				ret.used[0] = ret.used[1] = initialData_opt ? size : 0;

				// done, copy output
				*buffer_out = ret;
				a3bufferReference(buffer_out);
				return 1;
			}
			else
				printf("\n A3 ERROR (BUF \'%s\'): \n\t Invalid handle; buffer not created.", name_opt);

			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3bufferCreateSplit(a3_BufferObject *buffer_out, const a3byte name_opt[32], const a3_BufferObjectType bufferType, const a3ui32 size0, const a3ui32 size1, const void *initialData0_opt, const void *initialData1_opt)
{
	a3_BufferObject ret = { 0 };
	a3ui32 handle;
	a3ui32 binding;
	a3ui32 fillHint;
	a3ui32 size = size0 + size1;

	if (buffer_out && size)
	{
		// check uninitialized
		if (!buffer_out->handle->handle)
		{
			// generate buffer
			glGenBuffers(1, &handle);
			if (handle)
			{
				binding = a3bufferInternalFlag(bufferType);
				fillHint = a3bufferInternalFillHint(bufferType);

				// bind and allocate space
				glBindBuffer(binding, handle);
				glBufferData(binding, size, 0, fillHint);
				glBufferSubData(binding, 0, size0, initialData0_opt);
				glBufferSubData(binding, size0, size1, initialData1_opt);
				glBindBuffer(binding, 0);

				// configure
				a3handleCreateHandle(ret.handle, a3bufferInternalReleaseFunc, name_opt, handle, 1);
				ret.type = bufferType;
				ret.internalBinding = binding;
				ret.size = ret.split[1] = size;
				ret.split[0] = size0;
				ret.used[0] = initialData0_opt ? size0 : 0;
				ret.used[1] = initialData1_opt ? size : size0;


				// done, copy output
				*buffer_out = ret;
				a3bufferReference(buffer_out);
				return 1;
			}
			else
				printf("\n A3 ERROR (BUF \'%s\'): \n\t Invalid handle; buffer not created.", name_opt);

			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3bufferActivate(const a3_BufferObject *buffer)
{
	if (buffer && buffer->handle->handle)
	{
		// bind valid
		glBindBuffer(buffer->internalBinding, buffer->handle->handle);
		return 1;
	}
	return -1;
}

a3ret a3bufferDeactivateType(const a3_BufferObjectType bufferType)
{
	glBindBuffer(a3bufferInternalFlag(bufferType), 0);
	return 0;
}


//-----------------------------------------------------------------------------
