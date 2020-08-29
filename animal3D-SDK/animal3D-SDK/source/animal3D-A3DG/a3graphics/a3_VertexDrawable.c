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
	
	a3_VertexDrawable.c
	Definitions for common renderable vertex-based objects.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_VertexDrawable.h"

#include <stdio.h>


#define A3_BUFFER_OFFSET(n) ((a3byte *)(0) + (n))


//-----------------------------------------------------------------------------
// internal utility declarations

a3ui16 a3primitiveInternalFlag(const a3_VertexPrimitiveType primitiveType);


//-----------------------------------------------------------------------------

const a3_VertexDrawable *a3vertexDrawableInternalGetActive(const a3_VertexDrawable *drawable, const a3i32 set)
{
	static const a3_VertexDrawable *activeDrawable = 0;

	// if set flag is not raised, skip set
	if (set == 0)
		return activeDrawable;

	// set and return
	return (activeDrawable = drawable);
}


//-----------------------------------------------------------------------------

a3ret a3vertexDrawableCreate(a3_VertexDrawable *drawable_out, a3_VertexArrayDescriptor *vertexArray, const a3_VertexPrimitiveType primitiveType, const a3ui32 firstVertex, const a3ui32 numVertices)
{
	a3_VertexDrawable ret = { 0 };
	if (drawable_out && vertexArray && vertexArray->handle->handle && numVertices)
	{
		// make sure not already initialized
		if (!drawable_out->vertexArray)
		{
			// set properties
			ret.first = firstVertex;
			ret.count = numVertices;
			ret.primitive = a3primitiveInternalFlag(primitiveType);

			// refcount for vertex array
			ret.vertexArray = vertexArray;

			// done
			*drawable_out = ret;
			a3vertexDrawableReference(drawable_out);
			return 1;
		}
	}
	return -1;
}

a3ret a3vertexDrawableCreateIndexed(a3_VertexDrawable *drawable_out, a3_VertexArrayDescriptor *vertexArray, a3_IndexBuffer *indexBuffer, const a3_IndexFormatDescriptor *indexFormat, const a3_VertexPrimitiveType primitiveType, const a3ui32 firstIndexByteOffset, const a3ui32 numIndices)
{
	a3_VertexDrawable ret = { 0 };
	a3i32 result = 0;
	if (drawable_out && vertexArray && numIndices)
	{
		if (!drawable_out->vertexArray && vertexArray->handle->handle)
		{
			// same above, but also set indexing properties
		//	ret.first = firstIndexByteOffset;
			ret.count = numIndices;
			ret.primitive = a3primitiveInternalFlag(primitiveType);
			ret.vertexArray = vertexArray;

			if (indexBuffer && indexBuffer->handle->handle && indexFormat && indexFormat->indexType)
			{
				ret.indexType = indexFormat->indexType;
				ret.indexing = A3_BUFFER_OFFSET(firstIndexByteOffset);
				ret.indexBuffer = indexBuffer;
				result = 1;
			}
			else
				printf("\n A3 Warning: Vertex drawable created without indexing; 'first' is 0.");

			// done
			*drawable_out = ret;
			a3vertexDrawableReference(drawable_out);
			return result;
		}
	}
	return -1;
}
