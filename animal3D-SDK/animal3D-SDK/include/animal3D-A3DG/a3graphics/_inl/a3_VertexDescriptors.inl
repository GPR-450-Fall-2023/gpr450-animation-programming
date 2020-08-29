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
	
	a3_VertexDescriptors.inl
	Inline definitions for vertex descriptors.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_VERTEXDESCRIPTORS_H
#ifndef __ANIMAL3D_VERTEXDESCRIPTORS_INL
#define __ANIMAL3D_VERTEXDESCRIPTORS_INL


//-----------------------------------------------------------------------------
// internal utility to help pad

#define a3pad(n, p)	( (n) + ( (p) - (n) % (p) ) % (p) )
#define a3pad4(n)	a3pad(n, 4)


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3vertexAttribGetElementsPerAttrib(const a3_VertexAttributeType attribType)
{
	static const a3byte elementsPerAttrib[] = { 0, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4 };
	return elementsPerAttrib[attribType];
}

A3_INLINE a3ret a3vertexAttribGetBytesPerElement(const a3_VertexAttributeType attribType)
{
	static const a3byte bytesPerElement[] = { 0, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 8, 8 };
	return bytesPerElement[attribType];
}

A3_INLINE a3ret a3indexGetBytesPerIndex(const a3_IndexType indexType)
{
	static const a3byte bytesPerIndex[] = { 0, 1, 2, 4 };
	return bytesPerIndex[indexType];
}

A3_INLINE a3ret a3vertexFormatGetStorageSpaceRequired(const a3_VertexFormatDescriptor *vertexFormat, const a3ui32 count)
{
	if (vertexFormat)
	{
		// padding may be required
		const a3ui32 bytes = (vertexFormat->vertexSize * count);
		return a3pad4(bytes);
	}
	return -1;
}

A3_INLINE a3ret a3indexFormatGetStorageSpaceRequired(const a3_IndexFormatDescriptor *indexFormat, const a3ui32 count)
{
	if (indexFormat)
	{
		// padding may be required
		const a3ui32 bytes = (indexFormat->indexSize * count);
		return a3pad4(bytes);
	}
	return -1;
}


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3vertexAttribCreateDescriptor(a3_VertexAttributeDescriptor *attrib_out, const a3_VertexAttributeName attribName, const a3_VertexAttributeType attribType)
{
	if (attrib_out)
	{
		attrib_out->name = attribName;
		attrib_out->type = attribType;
		return attribType;
	}
	return -1;
}

A3_INLINE a3ret a3vertexAttribDataCreateDescriptor(a3_VertexAttributeDataDescriptor *attrib_out, const a3_VertexAttributeName attribName, const void *attribData)
{
	if (attrib_out)
	{
		attrib_out->name = attribName;
		attrib_out->data = attribData;
		return 1;
	}
	return -1;
}

A3_INLINE a3ret a3vertexFormatHasAttribute(const a3_VertexFormatDescriptor *vertexFormat, const a3_VertexAttributeName attribName)
{
	if (vertexFormat)
		return (vertexFormat->attribType[attribName]);
	return -1;
}

A3_INLINE a3ret a3indexFormatIsActive(const a3_IndexFormatDescriptor *indexFormat)
{
	if (indexFormat)
		return (indexFormat->indexType);
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_VERTEXDESCRIPTORS_INL
#endif	// __ANIMAL3D_VERTEXDESCRIPTORS_H