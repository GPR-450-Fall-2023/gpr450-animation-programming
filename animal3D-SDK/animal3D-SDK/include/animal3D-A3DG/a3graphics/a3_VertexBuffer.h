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
	
	a3_VertexBuffer.h
	Vertex buffer object (VBO) utilities, including index buffer object or 
		element buffer object (IBO/EBO) and vertex array object (VAO).

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_VERTEXBUFFER_H
#define __ANIMAL3D_VERTEXBUFFER_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_BufferObject.h"
#include "a3_VertexDescriptors.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_VertexArrayDescriptor	a3_VertexArrayDescriptor;
#endif	// __cplusplus

	
//-----------------------------------------------------------------------------

	// A3: Vertex buffer object (VBO) and index buffer object (IBO) (a.k.a. 
	//		element buffer object (EBO)) are aliases for generic buffer object.
	typedef a3_BufferObject					a3_VertexBuffer;
	typedef a3_BufferObject					a3_IndexBuffer;
	

	// A3: Wrapper for vertex array object (VAO); OpenGL uses this to describe 
	//		a vertex as it appears in a vertex buffer (VBO).
	//	member handle: graphics object handle
	//	member vertexFormat: the format used to describe vertices in this array
	//	member vertexBuffer: pointer to vertex buffer (VBO) associated with 
	//		this vertex array (VAO); required
	//	member vertexCount: the number of vertices tracked by this vertex 
	//		array; only updates if buffer is updated via 'a3vertexArrayStore'
	struct a3_VertexArrayDescriptor
	{
		a3_GraphicsObjectHandle handle[1];
		a3_VertexFormatDescriptor vertexFormat[1];
		a3_VertexBuffer *vertexBuffer;
		a3ui32 vertexCount;
	};


//-----------------------------------------------------------------------------

	// A3: Interleave raw attribute data (not yet interleaved) and insert the 
	//		interleaved data into a vertex buffer.
	//	param vertexBuffer: non-null pointer to initialized vertex buffer
	//	param vertexFormat: non-null pointer to vertex format descriptor
	//	param attribRawData: non-null array of vertex attribute data; should 
	//		be aligned with the attributes described in the format descriptor
	//		NOTE: THE DATA IS NOT INTERLEAVED; THIS FUNCTION DOES THAT FOR YOU!
	//	param vertexCount: how many instances of this vertex (non-zero)
	//	param offset_out_opt: optional pointer to get this storage's offset
	//	return: size of data stored if success
	//	return: 0 if missing attribute descriptions or data, or out of space
	//	return: -1 if invalid params or improperly initialized params
	a3ret a3vertexBufferStore(a3_VertexBuffer *vertexBuffer, const a3_VertexFormatDescriptor *vertexFormat, const a3_VertexAttributeDataDescriptor *attribRawData, const a3ui32 vertexCount, a3ui32 *offset_out_opt);

	// A3: Fill index buffer with raw index data.
	//	param indexBuffer: non-null pointer to initialized index buffer
	//	param indexFormat: non-null pointer to index format descriptor
	//	param indexData: non-null array of index data; should have the same 
	//		data type as described by index descriptor
	//	param indexCount: how many indices to store (non-zero)
	//	param offset_out_opt: optional pointer to get this storage's offset
	//	param internalFormat_opt: optional pointer to a format that describes 
	//		how the data should be converted before being stored; if null, it 
	//		is assumed the data passed in (indexData) matches the type in the 
	//		indexFormat param
	//	param baseIndex: index value to add to all indices processed
	//	return: size of data stored if success
	//	return: 0 if missing description of index, or out of space
	//	return: -1 if invalid params or improperly initialized params
	a3ret a3indexBufferStore(a3_IndexBuffer *indexBuffer, const a3_IndexFormatDescriptor *indexFormat, const void *indexData, const a3ui32 indexCount, const a3ui32 baseIndex, a3ui32 *offset_out_opt, const a3_IndexFormatDescriptor *internalFormat_opt);


//-----------------------------------------------------------------------------
	
	// A3: Create vertex array object (VAO) descriptor.
	//	param vertexArray_out: non-null pointer to uninitialized vertex array
	//		object descriptor
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param vertexBuffer: non-null pointer to initialized vertex buffer
	//	param vertexFormat: non-null pointer to initialized vertex descriptor
	//	param vertexBufferOffset: byte offset in provided vertex buffer for 
	//		the starting point of this vertex array
	//	return: number of attributes included if success
	//	return: 0 if creation failed or no attributes in vertex descriptor
	//	return: -1 if invalid params, already initialized, or buffer not init
	a3ret a3vertexArrayCreateDescriptor(a3_VertexArrayDescriptor *vertexArray_out, const a3byte name_opt[32], a3_VertexBuffer *vertexBuffer, const a3_VertexFormatDescriptor *vertexFormat, const a3ui32 vertexBufferOffset);

	// A3: Get current count of vertices referenced by vertex array.
	//	param vertexArray: non-null pointer to initialized vertex array
	//	return: current vertex count
	//	return: -1 if invalid param
	a3ret a3vertexArrayGetCurrentCount(const a3_VertexArrayDescriptor *vertexArray);

	// A3: Similar to 'a3vertexBufferStore', but also keeps track of how many 
	//		vertices are being added to the buffer.
	//	param vertexArray: non-null pointer to initialized vertex array
	//	param attribRawData: non-null array of vertex attribute data; should 
	//		be aligned with the attributes described in the format descriptor
	//		NOTE: THE DATA IS NOT INTERLEAVED; THIS FUNCTION DOES THAT FOR YOU!
	//	param vertexCount: how many instances of this vertex (non-zero)
	//	param vertexBufferOffset_out_opt: optional pointer to get this 
	//		storage's offset from the start of the vertex buffer
	//	param firstVertex_out_opt: optional pointer to get the vertex count 
	//		in the vertex array before storage
	//	return: vertexCount if success
	//	return: 0 if missing attribute descriptions or data, or out of space
	//	return: -1 if invalid params or improperly initialized params
	a3ret a3vertexArrayStore(a3_VertexArrayDescriptor *vertexArray, const a3_VertexAttributeDataDescriptor *attribRawData, const a3ui32 vertexCount, a3ui32 *vertexBufferOffset_out_opt, a3ui32 *firstVertex_out_opt);

	// A3: Reference vertex array descriptor for use; call release when done.
	//	param vertexArray: non-null pointer to initialized descriptor
	//	return: reference count if success
	//	return: -1 if invalid param or not initialized
	a3ret a3vertexArrayReferenceDescriptor(a3_VertexArrayDescriptor *vertexArray);

	// A3: Release vertex array descriptor.
	//	param vertexArray: non-null pointer to initialized descriptor
	//	return: positive if removed reference
	//	return: 0 if object was permanently released; descriptor is reset
	//	return: -1 if invalid param or not initialized
	a3ret a3vertexArrayReleaseDescriptor(a3_VertexArrayDescriptor *vertexArray);

	// A3: Update vertex array release function (hotload quick-fix).
	//	param vertexArray: non-null pointer to vertex array to update
	//	return: 1 if success
	//	return: -1 if invalid params
	a3ret a3vertexArrayHandleUpdateReleaseCallback(a3_VertexArrayDescriptor *vertexArray);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_VertexBuffer.inl"


#endif	// !__ANIMAL3D_VERTEXBUFFER_H