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
	
	a3_VertexDescriptors.h
	Vertex descriptors and utility functions for things such as single vertex 
		attributes, whole vertices and indices.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_VERTEXDESCRIPTORS_H
#define __ANIMAL3D_VERTEXDESCRIPTORS_H


#include "animal3D/a3/a3types_integer.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_VertexAttributeDescriptor		a3_VertexAttributeDescriptor;
	typedef struct a3_VertexAttributeDataDescriptor	a3_VertexAttributeDataDescriptor;
	typedef struct a3_VertexFormatDescriptor		a3_VertexFormatDescriptor;
	typedef struct a3_IndexFormatDescriptor			a3_IndexFormatDescriptor;
	typedef enum a3_VertexAttributeName				a3_VertexAttributeName;
	typedef enum a3_VertexAttributeType				a3_VertexAttributeType;
	typedef enum a3_IndexType						a3_IndexType;
#endif	// __cplusplus


//-----------------------------------------------------------------------------
	
	// A3: Attribute names (what they represent in shader land): these are 
	//		organized in this order because they align with OpenGL's default 
	//		attribute assignments on hardware.
	enum a3_VertexAttributeName
	{
		// defaults, matching OpenGL's specification
		a3attrib_position,
		a3attrib_blendWeights,
		a3attrib_normal,
		a3attrib_color,
		a3attrib_colorSecondary,
		a3attrib_fogCoord,
		a3attrib_pointSize,
		a3attrib_blendIndices,
		a3attrib_texcoord0,
		a3attrib_texcoord1,
		a3attrib_texcoord2,
		a3attrib_texcoord3,
		a3attrib_texcoord4,
		a3attrib_texcoord5,
		a3attrib_texcoord6,
		a3attrib_texcoord7,

		// alternative names
		a3attrib_texcoord = a3attrib_texcoord0,
		a3attrib_texcoordSecondary,
		a3attrib_tangent,
		a3attrib_bitangent,
		a3attrib_positionSecondary,
		a3attrib_normalSecondary,
		a3attrib_tangentSecondary,
		a3attrib_bitangentSecondary,

		// user alternatives
		a3attrib_user00 = a3attrib_position,
		a3attrib_user01,
		a3attrib_user02,
		a3attrib_user03,
		a3attrib_user04,
		a3attrib_user05,
		a3attrib_user06,
		a3attrib_user07,
		a3attrib_user08,
		a3attrib_user09,
		a3attrib_user10,
		a3attrib_user11,
		a3attrib_user12,
		a3attrib_user13,
		a3attrib_user14,
		a3attrib_user15,
	};

	// A3: Attribute max.
	enum a3_VertexAttributeNamesMax
	{
		a3attrib_nameMax = 16
	};


	// A3: Enumeration of attribute data types.
	enum a3_VertexAttributeType
	{
		a3attrib_disable,	// attribute is not used
		a3attrib_int,		// integer
		a3attrib_ivec2,		// 2D integer vector
		a3attrib_ivec3,		// 3D integer vector
		a3attrib_ivec4,		// 4D integer vector
		a3attrib_float,		// float
		a3attrib_vec2,		// 2D float vector
		a3attrib_vec3,		// 3D float vector
		a3attrib_vec4,		// 4D float vector
		a3attrib_double,	// double
		a3attrib_dvec2,		// 2D double vector
		a3attrib_dvec3,		// 3D double vector
		a3attrib_dvec4,		// 4D double vector
	};


	// A3: Enumeration of index types.
	enum a3_IndexType
	{
		a3index_disable,	// indexing is not used
		a3index_byte,		// 8-bit index, max 255
		a3index_short,		// 16-bit index, max 65535, 
		a3index_int,		// 32-bit index, max over 4B
	};

	// A3: Index size max counts.
	enum a3_IndexCountMax
	{
		a3index_countMaxDisabled,
		a3index_countMaxByte = 256,
		a3index_countMaxShort = a3index_countMaxByte*a3index_countMaxByte,
	};


	// A3: Vertex attribute descriptor container.
	//	member name: attribute being described
	//	member type: data type used to describe attribute
	struct a3_VertexAttributeDescriptor
	{
		a3_VertexAttributeName name;
		a3_VertexAttributeType type;
	};


	// A3: Vertex attribute data container; this is separate from the above 
	//		descriptor because a set of attribute descriptions are used to 
	//		create a reusable vertex description which is data-agnostic, 
	//		whereas the data itself is only used to create a buffer.
	//	member name: attribute to which data belongs
	//	member data: pointer to raw attribute data
	struct a3_VertexAttributeDataDescriptor
	{
		a3_VertexAttributeName name;
		const void *data;
	};


	// A3: Description of a collection of attributes to form a vertex.
	//	member attribType: OpenGL's type flag for each attribute
	//	member attribOffset: number of bytes each attribute is offset from the 
	//		start of a vertex
	//	member attribElements: how many elements for each attribute
	//	member attribSize: size of attribute in bytes
	//	member vertexSize: the total size of a vertex in bytes
	//	member vertexNumAttribs: number of attributes described in this format
	struct a3_VertexFormatDescriptor
	{
		a3ui16 attribType[a3attrib_nameMax];
		a3ui16 attribOffset[a3attrib_nameMax];
		a3ubyte attribElements[a3attrib_nameMax];
		a3ubyte attribSize[a3attrib_nameMax];
		a3ui16 vertexSize;
		a3ui16 vertexNumAttribs;
	};


	// A3: Description of a single index used for indexed rendering.
	//	member indexType: OpenGL's type flag for this type of index
	//	member indexSize: size of an index entry in bytes
	struct a3_IndexFormatDescriptor
	{
		a3ui16 indexType;
		a3ui16 indexSize;
	};


//-----------------------------------------------------------------------------

	// A3: Get the number of elements for each instance of an attribute type.
	//	param attribType: data type used to describe attribute
	//	return: number of elements contained in an instance of this attribute
	a3ret a3vertexAttribGetElementsPerAttrib(const a3_VertexAttributeType attribType);

	// A3: Get the number of bytes per element for an attribute type.
	//	param attribType: data type used to describe attribute
	//	return: number of bytes for a single element of this attribute
	a3ret a3vertexAttribGetBytesPerElement(const a3_VertexAttributeType attribType);

	// A3: Get the number of bytes per index of specified type.
	//	param indexType: data type used to describe index
	//	return: number of bytes contained in an index
	a3ret a3indexGetBytesPerIndex(const a3_IndexType indexType);

	// A3: Get the space in a buffer required to store a set of the provided 
	//		vertex format.
	//	param vertex: non-null pointer to vertex descriptor to be stored
	//	param count: how many vertices
	//	return: storage space requirements in bytes
	//	return: -1 if invalid param
	a3ret a3vertexFormatGetStorageSpaceRequired(const a3_VertexFormatDescriptor *vertexFormat, const a3ui32 count);

	// A3: Get the space in a buffer required to store a set of the provided 
	//		index format.
	//	param index: non-null pointer to index descriptor to be stored
	//	param count: how many indices
	//	return: storage space requirements in bytes
	//	return: -1 if invalid param
	a3ret a3indexFormatGetStorageSpaceRequired(const a3_IndexFormatDescriptor *indexFormat, const a3ui32 count);


//-----------------------------------------------------------------------------

	// A3: Fill a vertex attribute descriptor with info given a name and type.
	//	param attrib_out: non-null pointer to attribute descriptor to fill
	//	param attribName: which attribute slot is being described
	//	param attribType: data type used to describe attribute
	//	return: attribType if success
	//	return: -1 if invalid params
	a3ret a3vertexAttribCreateDescriptor(a3_VertexAttributeDescriptor *attrib_out, const a3_VertexAttributeName attribName, const a3_VertexAttributeType attribType);

	// A3: Fill a vertex attribute data descriptor with pointer.
	//	param attrib_out: non-null pointer to attribute data descriptor
	//	param attribName: which attribute slot is getting data
	//	param attribData: pointer to raw data
	//	return: 1 if success
	//	return: -1 if invalid params
	a3ret a3vertexAttribDataCreateDescriptor(a3_VertexAttributeDataDescriptor *attrib_out, const a3_VertexAttributeName attribName, const void *attribData);

	// A3: Create vertex descriptor from list of attributes.
	//	param vertexFormat_out: non-null pointer to vertex format descriptor
	//	param attribList: non-null array of vertex attribute descriptors; if 
	//		an attribute type appears more than once in the list, only the 
	//		first one will be considered
	//	param attribCount: how many attributes, greater than zero, capped at 16
	//	return: number of attributes if success
	//	return: 0 if no valid attributes provided
	//	return: -1 if invalid params
	a3ret a3vertexFormatCreateDescriptor(a3_VertexFormatDescriptor *vertexFormat_out, const a3_VertexAttributeDescriptor *attribList, a3ui32 attribCount);

	// A3: Create index descriptor given data type.
	//	param indexFormat_out: non-null pointer to index format descriptor
	//	param indexType: type description of index
	//	return: indexType if success
	//	return: -1 if invalid params
	a3ret a3indexFormatCreateDescriptor(a3_IndexFormatDescriptor *indexFormat_out, const a3_IndexType indexType);

	// A3: Check if vertex format descriptor includes a named attribute.
	//	param vertexFormat: non-null pointer to vertex format descriptor
	//	param attribName: which attribute slot to check
	//	return: positive if vertex includes attribute
	//	return: 0 if vertex does not include attribute
	//	return: -1 if invalid param
	a3ret a3vertexFormatHasAttribute(const a3_VertexFormatDescriptor *vertexFormat, const a3_VertexAttributeName attribName);

	// A3: Check if index format descriptor describes an active index format.
	//	param indexFormat: non-null pointer to index format descriptor
	//	return: positive if index format is active
	//	return: 0 if index format is inactive
	//	return: -1 if invalid param
	a3ret a3indexFormatIsActive(const a3_IndexFormatDescriptor *indexFormat);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_VertexDescriptors.inl"


#endif	// !__ANIMAL3D_VERTEXDESCRIPTORS_H