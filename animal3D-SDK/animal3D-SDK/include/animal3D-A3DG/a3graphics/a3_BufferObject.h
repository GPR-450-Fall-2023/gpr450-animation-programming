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
	
	a3_BufferObject.h
	Generic wrapper for OpenGL buffer object.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_BUFFEROBJECT_H
#define __ANIMAL3D_BUFFEROBJECT_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_GraphicsObjectHandle.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_BufferObject		a3_BufferObject;
	typedef enum a3_BufferObjectType	a3_BufferObjectType;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Enumeration of buffer data types.
	//	a3buffer_vertex: store vertex data in a "vertex buffer object" (VBO)
	//	a3buffer_index: store index data in an "index buffer object" (IBO), 
	//		a.k.a. "element buffer object" (EBO)
	//	a3buffer_uniform: store uniform data in a "uniform buffer object" (UBO)
	//	NOTE: It is also possible for vertex and index data to coexist in the 
	//		same buffer; use either a3buffer_vertex or a3buffer_index mode to 
	//		achieve this; avoid using 'deactivate' function for shared buffer.
	enum a3_BufferObjectType
	{
		a3buffer_vertex,
		a3buffer_index,
		a3buffer_uniform,
	};


	// A3: Generic buffer object container.
	//	member handle: graphics handle container
	//	member type: the type of data stored in the buffer
	//	member internalBinding: the correct OpenGL binding for this buffer
	//	member size: the capacity of the buffer in bytes
	//	member split: divisions in the buffer (e.g. sharing different data)
	//	member used: space used in divisions
	struct a3_BufferObject
	{
		a3_GraphicsObjectHandle handle[1];
		a3_BufferObjectType type;
		a3ui32 internalBinding;
		a3ui32 size;
		a3ui32 split[2];
		a3ui32 used[2];
	};


//-----------------------------------------------------------------------------

	// A3: Create generic buffer object.
	//	param buffer_out: non-null pointer to uninitialized buffer object
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param bufferType: type of buffer to create
	//	param size: non-zero size of space to allocate for buffer in bytes
	//	param initialData_opt: optional pointer to data to fill buffer with
	//		upon initialization
	//	param split_opt: adds a division to the buffer; specifies the number 
	//		of bytes from the start to separate; both sections are the same if 
	//		this is zero or exceeds the size of the buffer
	//	return: 1 if success
	//	return: 0 if buffer creation failed
	//	return: -1 if invalid params or buffer already initialized
	a3ret a3bufferCreate(a3_BufferObject *buffer_out, const a3byte name_opt[32], const a3_BufferObjectType bufferType, const a3ui32 size, const void *initialData_opt);

	// A3: Create generic buffer object with a division for sharing data.
	//	param buffer_out: non-null pointer to uninitialized buffer object
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param bufferType: type of buffer to create
	//	param size0: non-zero size of first division
	//	param size1: non-zero size of second division (total = size0 + size1)
	//	param initialData0_opt: optional pointer to data to fill first 
	//		section of buffer with upon initialization
	//	param initialData1_opt: optional pointer to data to fill second 
	//		section of buffer with upon initialization
	//	return: 1 if success
	//	return: 0 if buffer creation failed
	//	return: -1 if invalid params or buffer already initialized
	a3ret a3bufferCreateSplit(a3_BufferObject *buffer_out, const a3byte name_opt[32], const a3_BufferObjectType bufferType, const a3ui32 size0, const a3ui32 size1, const void *initialData0_opt, const void *initialData1_opt);

	// A3: Append section of buffer of specified type with data.
	//	param buffer: non-null pointer to initialized buffer object
	//	param section: section which chunk to store data in (boolean, use 
	//		zero if buffer was not created with split)
	//	param size: size of data to be stored in bytes (non-zero)
	//	param data: pointer to data to fill buffer with; null to erase
	//	param start_out_opt: optional pointer to an integer to store the 
	//		offset of this chunk of data (i.e. its location in the buffer)
	//	return: size of data stored
	//	return: 0 if data could not be stored (exceeds space allocation)
	//	return: -1 if invalid params or buffer not initialized
	a3ret a3bufferAppend(a3_BufferObject *buffer, a3i32 section, const a3ui32 size, const void *data, a3ui32 *start_out_opt);

	// A3: Append section of buffer with an offset.
	//	param buffer: non-null pointer to initialized buffer object
	//	param section: section which chunk to store data in (boolean, use 
	//		zero if buffer was not created with split)
	//	param offset: start point in specified section to begin filling
	//	param size: size of data to be stored in bytes (non-zero)
	//	param data: pointer to data to fill buffer with; null to erase
	//	param start_out_opt: optional pointer to an integer to store the 
	//		offset of this chunk of data (i.e. its location in the buffer)
	//	return: size of data stored
	//	return: 0 if data could not be stored (exceeds space allocation)
	//	return: -1 if invalid params or buffer not initialized
	a3ret a3bufferAppendOffset(a3_BufferObject *buffer, a3i32 section, const a3ui32 offset, const a3ui32 size, const void *data, a3ui32 *start_out_opt);

	// A3: Fill section of buffer from beginning.
	//	param buffer: non-null pointer to initialized buffer object
	//	param section: section which chunk to store data in (boolean, use 
	//		zero if buffer was not created with split)
	//	param size: size of data to be stored in bytes (non-zero)
	//	param data: pointer to data to fill buffer with; null to erase
	//	return: size of data stored
	//	return: 0 if data could not be stored (exceeds space allocation)
	//	return: -1 if invalid params or buffer not initialized
	a3ret a3bufferRefill(a3_BufferObject* buffer, a3i32 section, const a3ui32 size, const void* data);

	// A3: Fill section of buffer from beginning with an offset.
	//	param buffer: non-null pointer to initialized buffer object
	//	param section: section which chunk to store data in (boolean, use 
	//		zero if buffer was not created with split)
	//	param offset: start point in specified section to begin filling
	//	param size: size of data to be stored in bytes (non-zero)
	//	param data: pointer to data to fill buffer with; null to erase
	//	return: size of data stored
	//	return: 0 if data could not be stored (exceeds space allocation)
	//	return: -1 if invalid params or buffer not initialized
	a3ret a3bufferRefillOffset(a3_BufferObject* buffer, a3i32 section, const a3ui32 offset, const a3ui32 size, const void* data);

	// A3: Check if buffer can fit data at end.
	//	param buffer: non-null pointer to initialized buffer object
	//	param section: section to check (boolean, use zero if buffer was not 
	//		created with split)
	//	param size: size of data to be stored in bytes
	//	return: 1 if the specified block size will fit
	//	return: 0 if the specified block size will not fit
	//	return: -1 if invalid params or buffer not initialized
	a3ret a3bufferValidateBlockSize(const a3_BufferObject *buffer, a3i32 section, const a3ui32 size);

	// A3: Get current buffer offset.
	//	param buffer: non-null pointer to initialized buffer object
	//	param section: section to check (boolean, use zero if buffer was not 
	//		created with split)
	//	return: current offset from start of buffer in bytes
	//	return: -1 if invalid params
	a3ret a3bufferGetCurrentOffset(const a3_BufferObject *buffer, a3i32 section);

	// A3: Activate buffer.
	//	param buffer: non-null pointer to initialized buffer object
	//	return: 1 if buffer activated
	//	return: -1 if invalid param or buffer not initialized
	a3ret a3bufferActivate(const a3_BufferObject *buffer);

	// A3: Deactivate active buffer of specified type.
	//	param bufferType: type of buffer to deactivate
	//	return: 0
	a3ret a3bufferDeactivateType(const a3_BufferObjectType bufferType);

	// A3: Reference buffer object for use; call release when done with it.
	//	param buffer: non-null pointer to initialized buffer object
	//	return: reference count if success
	//	return: -1 if invalid param or buffer not initialized
	a3ret a3bufferReference(a3_BufferObject *buffer);

	// A3: Release buffer object.
	//	param buffer: non-null pointer to initialized buffer object
	//	return: positive if removed reference
	//	return: 0 if object was permanently released; descriptor is reset
	//	return: -1 if invalid param or buffer not initialized
	a3ret a3bufferRelease(a3_BufferObject *buffer);

	// A3: Update buffer release function (hotload quick-fix).
	//	param buffer: non-null pointer to buffer to update
	//	return: 1 if success
	//	return: -1 if invalid params
	a3ret a3bufferHandleUpdateReleaseCallback(a3_BufferObject *buffer);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_BufferObject.inl"


#endif	// !__ANIMAL3D_BUFFEROBJECT_H