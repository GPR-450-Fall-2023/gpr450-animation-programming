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
	
	a3_GeometryData.inl
	Inline definitions for geometry data and vertex descriptors.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_GEOMETRYDATA_H
#ifndef __ANIMAL3D_GEOMETRYDATA_INL
#define __ANIMAL3D_GEOMETRYDATA_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3geometryGetVertexCount(const a3_GeometryData *geom)
{
	if (geom && geom->data)
		return geom->numVertices;
	return -1;
}

A3_INLINE a3ret a3geometryGetIndexCount(const a3_GeometryData *geom)
{
	if (geom && geom->data)
		return geom->numIndices;
	return -1;
}

A3_INLINE a3ret a3geometryGetVertexBufferSize(const a3_GeometryData *geom)
{
	if (geom && geom->data)
		return a3vertexFormatGetStorageSpaceRequired(geom->vertexFormat, geom->numVertices);
	return 0;
}

A3_INLINE a3ret a3geometryGetIndexBufferSize(const a3_GeometryData *geom)
{
	if (geom && geom->data)
		return a3indexFormatGetStorageSpaceRequired(geom->indexFormat, geom->numIndices);
	return 0;
}


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3geometryGetAddressBitangent(const void **attribDataPtr_out, const a3_GeometryData *geom)
{
	if (attribDataPtr_out && geom && geom->data)
	{
		*attribDataPtr_out = geom->attribData[a3attrib_geomTangent] ? 
			(a3f32 *)geom->attribData[a3attrib_geomTangent] + (geom->numVertices * 3) : 0;
		return (*attribDataPtr_out != 0);
	}
	return -1;
}

A3_INLINE a3ret a3geometryGetAddressBlendingInd(const void **attribDataPtr_out, const a3_GeometryData *geom)
{
	if (attribDataPtr_out && geom && geom->data)
	{
		*attribDataPtr_out = geom->attribData[a3attrib_geomBlending] ?
			(a3f32 *)geom->attribData[a3attrib_geomBlending] + (geom->numVertices * 4) : 0;
		return (*attribDataPtr_out != 0);
	}
	return -1;
}


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3geometryGenerateVertexArray(a3_VertexArrayDescriptor *vertexArray_out, const a3byte name_opt[32], const a3_GeometryData *geom, a3_VertexBuffer *vertexBuffer, const a3ui32 vertexBufferOffset)
{
	if (geom && geom->data)
		return a3vertexArrayCreateDescriptor(vertexArray_out, name_opt, vertexBuffer, geom->vertexFormat, vertexBufferOffset);
	return -1;
}


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3geometryGenerateDrawableSelfContained(a3_VertexDrawable *drawable_out, a3_VertexArrayDescriptor *dedicatedFormat_out, a3_VertexBuffer *dedicatedBuffer_out, const a3_GeometryData *geom)
{
	// validate params
	if (drawable_out && dedicatedFormat_out && dedicatedBuffer_out && geom && geom->data)
	{
		// validate init
		if (!drawable_out->vertexArray && !dedicatedFormat_out->handle->handle && !dedicatedBuffer_out->handle->handle)
		{
			// determine space requirements
			const a3ui32 vertexStorage = a3geometryGetVertexBufferSize(geom);
			const a3ui32 indexStorage = a3geometryGetIndexBufferSize(geom);
			a3i32 result;

			// create buffer and vertex format
			a3bufferCreateSplit(dedicatedBuffer_out, "tmp_vbo+ibo", a3buffer_vertex, vertexStorage, indexStorage, 0, 0);
			a3geometryGenerateVertexArray(dedicatedFormat_out, "tmp_vao", geom, dedicatedBuffer_out, 0);

			// generate drawable
			result = a3geometryGenerateDrawable(drawable_out, geom, dedicatedFormat_out, dedicatedBuffer_out, 0, 0, 0);

			// check if successful
			if (result > 0)
				return 1;
			
			// fail, release handles
			a3vertexArrayReleaseDescriptor(dedicatedFormat_out);
			a3bufferRelease(dedicatedBuffer_out);
			
			// done
			return 0;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_GEOMETRYDATA_INL
#endif	// __ANIMAL3D_GEOMETRYDATA_H