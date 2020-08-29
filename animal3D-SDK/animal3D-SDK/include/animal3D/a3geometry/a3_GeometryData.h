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
	
	a3_GeometryData.h
	Helpful container for geometry data and utilities to create special vertex 
		descriptors for pre-defined geometry; limits the number of attributes 
		available.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_GEOMETRYDATA_H
#define __ANIMAL3D_GEOMETRYDATA_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"
#include "animal3D/a3utility/a3_Stream.h"
#include "animal3D-A3DG/a3graphics/a3_VertexDrawable.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_GeometryData				a3_GeometryData;
	typedef enum a3_GeometryVertexAttributeName	a3_GeometryVertexAttributeName;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Enumeration of possible attributes for pre-defined geometry.
	enum a3_GeometryVertexAttributeName
	{
		a3attrib_geomPosition,	// vertex positions, vec3, enabled by default, provided or not
		a3attrib_geomNormal,	// vertex normals, vec3
		a3attrib_geomColor,		// vertex colors, vec4
		a3attrib_geomTexcoord,	// texture coordinates, vec2
		a3attrib_geomTangent,	// tangents and bitangents, each vec3, requires texcoords and normals
		a3attrib_geomBlending,	// blend weights, vec4, and blend indices, ivec4
	};

	// A3: Geometry attribute name and attribute maximum.
	enum a3_GeometryVertexAttributeMax
	{
		a3attrib_geomNameMax = 6
	};


//-----------------------------------------------------------------------------

	// A3: General container for geometry/model data. Inherently it has no 
	//		single intended purpose; client APIs can decide what to do with it.
	//	member vertexFormat: vertex format descriptor
	//	member indexFormat: index format descriptor (may not be used)
	//	member primType: primitive type that should be used for drawing
	//	members numVertices, numIndices: vertex and index count
	//	member data: pointer to raw geometry data; should be null if unused
	//	member attribData: array of pointers to specific attribute data
	//	member indexData: pointer to indexing data (may not be used)
	struct a3_GeometryData
	{
		a3_VertexFormatDescriptor vertexFormat[1];
		a3_IndexFormatDescriptor indexFormat[1];
		a3_VertexPrimitiveType primType;
		a3ui32 numVertices, numIndices;
		void *data;
		const void *attribData[a3attrib_geomNameMax];
		const void *indexData;
	};


//-----------------------------------------------------------------------------

	// A3: Create vertex format descriptor given a list of geometry attribute 
	//		names; will automatically validate and select types.
	//	param vertexFormat_out: the vertex format to be stored
	//	param attribNameList: non-null array of geometry attribute names
	//	param attribNameCount: non-zero count of attribute names, less than max
	//	return: total number of attributes specified if success
	//	return: -1 if invalid params
	a3ret a3geometryCreateVertexFormat(a3_VertexFormatDescriptor *vertexFormat_out, const a3_GeometryVertexAttributeName *attribNameList, a3ui32 attribNameCount);

	// A3: Create index format descriptor given max size of buffer.
	//	param indexFormat_out: the index format to be stored
	//	param vertexCount: non-zero max number of vertices referred to; needed 
	//		to determine the best size for a single index (a3byte, a3i16, a3i32)
	//	return: index type selected if success
	//	return: -1 if invalid params
	a3ret a3geometryCreateIndexFormat(a3_IndexFormatDescriptor *indexFormat_out, const a3ui32 vertexCount);
	
	// A3: Get the vertex count for a shape descriptor.
	//	param geom: non-null pointer to initialized descriptor
	//	return: number of unique vertices needed to create this shape
	//	return: -1 if invald params
	a3ret a3geometryGetVertexCount(const a3_GeometryData *geom);

	// A3: Get the index count for a shape descriptor.
	//	param geom: non-null pointer to initialized descriptor
	//	return: number of indices needed to create this shape
	//	return: 0 if indices are not required for this shape
	//	return: -1 if invalid params
	a3ret a3geometryGetIndexCount(const a3_GeometryData *geom);

	// A3: Get the required vertex buffer size for a shape descriptor; also 
	//		accounts for indexing.
	//	param geom: non-null pointer to initialized descriptor
	//	return: vertex buffer storage requirements for this shape
	//	return: 0 if invalid param (so it doesn't mess up other shapes)
	a3ret a3geometryGetVertexBufferSize(const a3_GeometryData *geom);

	// A3: Get the required index buffer size for a shape descriptor.
	//	param geom: non-null pointer to initialized descriptor
	//	return: index buffer storage requirements for this shape
	//	return: 0 if index buffer is not required for this shape
	//	return: 0 if invalid param
	a3ret a3geometryGetIndexBufferSize(const a3_GeometryData *geom);


//-----------------------------------------------------------------------------

	// A3: Get the address of the implicit bitangent attribute data.
	//	param attribDataPtr_out: pointer to data pointer to capture address
	//	param geom: non-null pointer to initialized descriptor
	//	return: 1 if success
	//	return: 0 if geometry does not have bitangents (pointer is null)
	//	return: -1 if invalid params
	a3ret a3geometryGetAddressBitangent(const void **attribDataPtr_out, const a3_GeometryData *geom);

	// A3: Get the address of the implicit blend indices attribute data.
	//	param attribDataPtr_out: pointer to data pointer to capture address
	//	param geom: non-null pointer to initialized descriptor
	//	return: 1 if success
	//	return: 0 if geometry does not have bitangents (pointer is null)
	//	return: -1 if invalid params
	a3ret a3geometryGetAddressBlendingInd(const void **attribDataPtr_out, const a3_GeometryData *geom);


//-----------------------------------------------------------------------------

	// A3: Create a vertex array descriptor given a shape descriptor; multiple 
	//		shapes with the same attribute flags can share a vertex array.
	//	param vertexArray_out: non-null pointer to uninitialized vertex array
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param geom: non-null pointer to initialized shape descriptor
	//	param vertexBuffer: non-null pointer to initialized vertex buffer
	//	param vertexBufferOffset: byte offset in provided vertex buffer for 
	//		the starting point of this vertex array's format
	//	return: number of attributes included if success
	//	return: 0 if fail
	//	return: -1 if invalid params
	a3ret a3geometryGenerateVertexArray(a3_VertexArrayDescriptor *vertexArray_out, const a3byte name_opt[32], const a3_GeometryData *geom, a3_VertexBuffer *vertexBuffer, const a3ui32 vertexBufferOffset);

	// A3: Generate procedural geometry drawable from a descriptor.
	//	param drawable_out: non-null pointer to uninitialized drawable
	//	param geom: non-null pointer to initialized descriptor
	//	param vertexArray: non-null pointer to initialized vertex array
	//		(required)
	//	param indexBuffer: non-null pointer to initialized index buffer; this 
	//		may be used depending on whether the geometry has indexing, if it 
	//		is needed and an index buffer is not passed, returns invalid
	//	param commonIndexFormat_opt: optional pointer to specify if generation 
	//		should follow a common index format
	//	param vertexBufferOffset_out_opt: optional pointer to catch vertex 
	//		buffer offset of data stored
	//	param indexBufferOffset_out_opt: optional pointer to catch index 
	//		buffer offset of data stored
	//	return: size of vertex data stored if succeeded
	//	return: -1 if invalid params
	a3ret a3geometryGenerateDrawable(a3_VertexDrawable *drawable_out, const a3_GeometryData *geom, a3_VertexArrayDescriptor *vertexArray, a3_IndexBuffer *indexBuffer, const a3_IndexFormatDescriptor *commonIndexFormat_opt, a3ui32 *vertexBufferOffset_out_opt, a3ui32 *indexBufferOffset_out_opt);


//-----------------------------------------------------------------------------

	// A3: Save geometry data to file.
	//	param geom: non-null pointer to initialized data container
	//	param fileStream: non-null pointer to file stream opened in write mode
	//	return: number of bytes written if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3geometrySaveDataBinary(const a3_GeometryData *geom, const a3_FileStream *fileStream);

	// A3: Load geometry data from file.
	//	param geom_out: non-null pointer to unused data container
	//	param fileStream: non-null pointer to file stream opened in read mode
	//	return: number of bytes read if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3geometryLoadDataBinary(a3_GeometryData *geom_out, const a3_FileStream *fileStream);

	// A3: Copy geometry data to string.
	//	param geom: non-null pointer to initialized data container
	//	param str: non-null pointer to storage string
	//		NOTE: must have enough space to hold geometry data
	//	return: number of bytes stored if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3geometryCopyDataToString(const a3_GeometryData *geom, a3byte *str);

	// A3: Copy geometry data from string.
	//	param geom_out: non-null pointer to unused data container
	//	param str: non-null pointer to storage string
	//	return: number of bytes read if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3geometryCopyStringToData(a3_GeometryData *geom_out, const a3byte *str);

	// A3: Get the string size for the specified geometry.
	//	param geom: non-null pointer to initialized data container
	//	return: stream size if success
	//	return: -1 if invalid param
	a3ret a3geometryGetStringSize(const a3_GeometryData *geom);

	// A3: Release geometry data container.
	//	param geom: non-null pointer to initialized data container
	//	return: 1 if success; object passed is reset
	//	return: -1 if invalid param or uninitialized
	a3ret a3geometryReleaseData(a3_GeometryData *geom);


//-----------------------------------------------------------------------------

	// A3: Create self-contained drawable given pre-generated geometry. 
	//		Creates its own buffers and vertex format. Use output drawable 
	//		like any other drawable: activate, release, etc. 
	//		How to use: 
	//			...for loaded model: load model, pass resulting geometry here.
	//			...for procedural model: make descriptor, generate geometry, 
	//				pass resulting geometry here.
	//	param drawable_out: non-null pointer to uninitialized drawable
	//	param dedicatedFormat_out: non-null pointer to uninitialized VAO 
	//		to be used exclusively by this drawable
	//	param dedicatedBuffer_out: non-null pointer to uninitialized VBO/IBO
	//		to be used exclusively by this drawable
	//	param geom: non-null pointer to initialized data container
	//	return: size of vertex data stored if succeeded
	//	return: -1 if invalid params
	a3ret a3geometryGenerateDrawableSelfContained(a3_VertexDrawable *drawable_out, a3_VertexArrayDescriptor *dedicatedFormat_out, a3_VertexBuffer *dedicatedBuffer_out, const a3_GeometryData *geom);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_GeometryData.inl"


#endif	// !__ANIMAL3D_GEOMETRYDATA_H