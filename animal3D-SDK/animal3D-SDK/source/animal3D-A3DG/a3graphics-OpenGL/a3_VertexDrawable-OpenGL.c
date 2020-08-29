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
	
	a3_VertexDrawable-OpenGL.c
	Definitions for OpenGL renderable vertex-based objects.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_VertexDrawable.h"

#include "GL/glew.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// internal utility declarations

const a3_VertexDrawable *a3vertexDrawableInternalGetActive(const a3_VertexDrawable *drawable, const a3i32 set);


//-----------------------------------------------------------------------------

a3ui16 a3primitiveInternalFlag(const a3_VertexPrimitiveType primitiveType)
{
	static const a3ui16 primitiveInternal[] = { GL_POINTS, GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, };
	return primitiveInternal[primitiveType];
}


//-----------------------------------------------------------------------------

a3ret a3vertexDrawableActivate(const a3_VertexDrawable *drawable)
{
	if (drawable && drawable->vertexArray)
	{
		// activate
		a3vertexDrawableInternalGetActive(drawable, 1);
		glBindVertexArray(drawable->vertexArray->handle->handle);
		if (drawable->indexType)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable->indexBuffer->handle->handle);
		return 0;
	}

	// deactivate
	a3vertexDrawableInternalGetActive(0, 1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return 0;
}

a3ret a3vertexDrawableDeactivate()
{
	a3vertexDrawableInternalGetActive(0, 1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return 0;
}

a3ret a3vertexDrawableRenderActive()
{
	const a3_VertexDrawable *drawable = a3vertexDrawableInternalGetActive(0, 0);
	if (drawable && drawable->vertexArray)
	{
		if (drawable->indexType)
			glDrawElements(drawable->primitive, drawable->count, drawable->indexType, drawable->indexing);
		else
			glDrawArrays(drawable->primitive, drawable->first, drawable->count);
		return 1;
	}
	return 0;
}

a3ret a3vertexDrawableRenderActiveInstanced(const a3ui32 instanceCount)
{
	const a3_VertexDrawable *drawable = a3vertexDrawableInternalGetActive(0, 0);
	if (drawable && drawable->vertexArray)
	{
		if (drawable->indexType)
			glDrawElementsInstanced(drawable->primitive, drawable->count, drawable->indexType, drawable->indexing, instanceCount);
		else
			glDrawArraysInstanced(drawable->primitive, drawable->first, drawable->count, instanceCount);
		return 1;
	}
	return 0;
}

a3ret a3vertexDrawableActivateAndRender(const a3_VertexDrawable *drawable)
{
	if (a3vertexDrawableInternalGetActive(drawable, 1) && drawable->vertexArray)
	{
		glBindVertexArray(drawable->vertexArray->handle->handle);
		if (drawable->indexType)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable->indexBuffer->handle->handle);
			glDrawElements(drawable->primitive, drawable->count, drawable->indexType, drawable->indexing);
		}
		else
			glDrawArrays(drawable->primitive, drawable->first, drawable->count);
		return 1;
	}

	// deactivate
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return 0;
}

a3ret a3vertexDrawableActivateAndRenderInstanced(const a3_VertexDrawable *drawable, const a3ui32 instanceCount)
{
	if (a3vertexDrawableInternalGetActive(drawable, 1) && drawable->vertexArray)
	{
		glBindVertexArray(drawable->vertexArray->handle->handle);
		if (drawable->indexType)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable->indexBuffer->handle->handle);
			glDrawElementsInstanced(drawable->primitive, drawable->count, drawable->indexType, drawable->indexing, instanceCount);
		}
		else
			glDrawArraysInstanced(drawable->primitive, drawable->first, drawable->count, instanceCount);
		return 1;
	}
	
	// deactivate
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return 0;
}
