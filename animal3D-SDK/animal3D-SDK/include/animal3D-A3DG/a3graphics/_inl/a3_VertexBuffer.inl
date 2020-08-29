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
	
	a3_VertexBuffer.inl
	Inline definitions for vertex buffer utilities.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_VERTEXBUFFER_H
#ifndef __ANIMAL3D_VERTEXBUFFER_INL
#define __ANIMAL3D_VERTEXBUFFER_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3vertexArrayGetCurrentCount(const a3_VertexArrayDescriptor *vertexArray)
{
	if (vertexArray && vertexArray->handle->handle)
		return vertexArray->vertexCount;
	return -1;
}

A3_INLINE a3ret a3vertexArrayStore(a3_VertexArrayDescriptor *vertexArray, const a3_VertexAttributeDataDescriptor *attribRawData, const a3ui32 vertexCount, a3ui32 *vertexBufferOffset_out_opt, a3ui32 *firstVertex_out_opt)
{
	if (vertexArray && vertexArray->handle->handle)
	{
		a3i32 ret = a3vertexBufferStore(vertexArray->vertexBuffer, vertexArray->vertexFormat, attribRawData, vertexCount, vertexBufferOffset_out_opt);
		if (ret > 0)
		{
			if (firstVertex_out_opt)
				*firstVertex_out_opt = vertexArray->vertexCount;
			vertexArray->vertexCount += vertexCount;
		}
		return ret;
	}
	return -1;
}

A3_INLINE a3ret a3vertexArrayReferenceDescriptor(a3_VertexArrayDescriptor *vertexArray)
{
	if (vertexArray)
	{
		if (a3bufferReference(vertexArray->vertexBuffer) > 0)
			return a3handleIncrementCount(vertexArray->handle);
	}
	return -1;
}

A3_INLINE a3ret a3vertexArrayReleaseDescriptor(a3_VertexArrayDescriptor *vertexArray)
{
	if (vertexArray)
	{
		a3i32 ret;
		if (a3bufferRelease(vertexArray->vertexBuffer) == 0)
			vertexArray->vertexBuffer = 0;
		ret = a3handleDecrementCount(vertexArray->handle);
		if (ret == 0)
		{
			vertexArray->vertexBuffer = 0;
			vertexArray->vertexCount = 0;
		}
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_VERTEXBUFFER_INL
#endif	// __ANIMAL3D_VERTEXBUFFER_H