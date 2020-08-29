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
	
	a3_VertexDrawable.inl
	Inline definitions for drawable object.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_VERTEXDRAWABLE_H
#ifndef __ANIMAL3D_VERTEXDRAWABLE_INL
#define __ANIMAL3D_VERTEXDRAWABLE_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3vertexDrawableReference(a3_VertexDrawable *drawable)
{
	if (drawable && drawable->vertexArray)
	{
		a3i32 ret = a3vertexArrayReferenceDescriptor(drawable->vertexArray);
		a3bufferReference(drawable->indexBuffer);
		return ret;
	}
	return -1;
}

A3_INLINE a3ret a3vertexDrawableRelease(a3_VertexDrawable *drawable)
{
	if (drawable && drawable->vertexArray)
	{
		a3i32 ret = a3vertexArrayReleaseDescriptor(drawable->vertexArray);
		a3bufferRelease(drawable->indexBuffer);
		if (ret == 0)
		{
			drawable->vertexArray = 0;
			drawable->indexBuffer = 0;
		}
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_VERTEXDRAWABLE_INL
#endif	// __ANIMAL3D_VERTEXDRAWABLE_H