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
	
	a3_VertexDrawable.h
	Interface for a renderable object made of vertices.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_VERTEXDRAWABLE_H
#define __ANIMAL3D_VERTEXDRAWABLE_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_VertexBuffer.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_VertexDrawable		a3_VertexDrawable;
	typedef enum a3_VertexPrimitiveType		a3_VertexPrimitiveType;
#endif	// __cplusplus


//-----------------------------------------------------------------------------
	
	// A3: Enumeration of primitive shape types to be drawn.
	enum a3_VertexPrimitiveType
	{
		a3prim_points,
		a3prim_lines,
		a3prim_lineLoop,
		a3prim_lineStrip,
		a3prim_triangles,
		a3prim_triangleStrip,
		a3prim_triangleFan,
	};


	// A3: Descriptor for drawable object.
	//	member first: the first vertex/index in the buffer section described 
	//		by the VAO
	//	member count: number of consecutive vertices/indices to draw with
	//	member primitive: internal flag for the primitive type to be drawn
	//	member indexType: if rendering with indices, the internal index type
	//	member indexing: if rendering with indices, the index buffer offset
	//	member indexBuffer: pointer to index buffer (IBO) associated with 
	//		this renderable object; optional
	//	member vertexArray: VAO used to draw vertices; required
	struct a3_VertexDrawable
	{
		a3ui32 first;
		a3ui32 count;
		a3ui16 primitive;
		a3ui16 indexType;
		const void *indexing;
		a3_IndexBuffer *indexBuffer;
		a3_VertexArrayDescriptor *vertexArray;
	};


//-----------------------------------------------------------------------------

	// A3: Create drawable.
	//	param drawable_out: non-null pointer to uninitialized drawable
	//	param vertexArray: non-null pointer to initialized vertex array
	//	param primitiveType: type of primitive to be drawn
	//	param firstVertex: offset to first vertex in associated buffer
	//		(measured as vertex count from start of buffer, not bytes)
	//	param numVertices: non-zero vertex count in drawable
	//	return: 1 if success
	//	return: -1 if invalid params or improperly initialized params
	a3ret a3vertexDrawableCreate(a3_VertexDrawable *drawable_out, a3_VertexArrayDescriptor *vertexArray, const a3_VertexPrimitiveType primitiveType, const a3ui32 firstVertex, const a3ui32 numVertices);

	// A3: Create drawable with index buffer.
	//	param drawable_out: non-null pointer to uninitialized drawable
	//	param vertexArray: non-null pointer to initialized vertex array
	//	param indexBuffer: non-null pointer to initialized index buffer
	//	param index: non-null description of indices
	//	param primitiveType: type of primitive to be drawn
	//	param firstIndexByteOffset: byte offset to first index in buffer
	//	param numIndices: non-zero index count in drawable
	//	return: 1 if success
	//	return: 0 if created without indexing
	//	return: -1 if invalid params or improperly initialized params
	a3ret a3vertexDrawableCreateIndexed(a3_VertexDrawable *drawable_out, a3_VertexArrayDescriptor *vertexArray, a3_IndexBuffer *indexBuffer, const a3_IndexFormatDescriptor *indexFormat, const a3_VertexPrimitiveType primitiveType, const a3ui32 firstIndexByteOffset, const a3ui32 numIndices);

	// A3: Activate/deactivate drawable for rendering.
	//	param drawable: pointer to drawable to activate; pass null or 
	//		uninitialized drawable to deactivate
	//	return: 1 if activated
	//	return: 0 if deactivated
	a3ret a3vertexDrawableActivate(const a3_VertexDrawable *drawable);

	// A3: Deactivate any active drawable.
	//	return: 0
	a3ret a3vertexDrawableDeactivate();

	// A3: Render active drawable.
	//	return: 1 if rendered
	//	return: 0 if no drawable active
	a3ret a3vertexDrawableRenderActive();

	// A3: Render multiple "instanced" copies active drawable; this is faster 
	//		than having separate single draw calls.
	//	param instanceCount: how many copies of the drawable to render
	//	return: 1 if rendered
	//	return: 0 if no drawable active or count is zero
	a3ret a3vertexDrawableRenderActiveInstanced(const a3ui32 instanceCount);

	// A3: Activate and immediately render drawable.
	//	param drawable: pointer to drawable; pass null or uninitialized to 
	//		deactivate all drawables
	//	return: 1 if activated and rendered
	//	return: 0 if deactivated
	a3ret a3vertexDrawableActivateAndRender(const a3_VertexDrawable *drawable);

	// A3: Activate and immediately render drawable instanced.
	//	param drawable: pointer to drawable; pass null or uninitialized to 
	//		deactivate all drawables
	//	param instanceCount: how many copies of the drawable to render
	//	return: 1 if activated and rendered
	//	return: 0 if deactivated
	a3ret a3vertexDrawableActivateAndRenderInstanced(const a3_VertexDrawable *drawable, const a3ui32 instanceCount);

	// A3: Reference drawable.
	//	param drawable: non-null pointer to initialized drawable to reference
	//	return: new reference count if success
	//	return: -1 if invalid param or not initialized
	a3ret a3vertexDrawableReference(a3_VertexDrawable *drawable);

	// A3: Release drawable.
	//	param drawable: non-null pointer to initialized drawable to release
	//	return: new reference count of vertex array if success
	//	return: 0 if permanently released; occurs when vertex array is released
	//	return: -1 if invalid param or not initialized
	a3ret a3vertexDrawableRelease(a3_VertexDrawable *drawable);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_VertexDrawable.inl"


#endif	// !__ANIMAL3D_VERTEXDRAWABLE_H