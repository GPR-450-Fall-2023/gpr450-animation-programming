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
	
	a3_VertexDescriptors.c
	Definitions for vertex descriptors.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_VertexDescriptors.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// internal utility declarations

a3ui16 a3vertexInternalGetType(const a3_VertexAttributeType type);
a3ui16 a3indexInternalGetType(const a3_IndexType type);


//-----------------------------------------------------------------------------

a3ret a3vertexFormatCreateDescriptor(a3_VertexFormatDescriptor *vertexFormat_out, const a3_VertexAttributeDescriptor *attribList, a3ui32 attribCount)
{
	a3_VertexFormatDescriptor ret = { 0 };
	const a3_VertexAttributeDescriptor *attribsOrdered[a3attrib_nameMax] = { 0 }, *itr;
	a3ui32 i;

	// validate params
	if (vertexFormat_out && attribList)
	{
		// validate count and handle creation
		if (attribCount)
		{
			// first, make sure all attribute descriptors are sorted
			attribCount = attribCount <= a3attrib_nameMax ? attribCount : a3attrib_nameMax;
			for (i = 0, itr = attribList; i < attribCount; ++i, ++itr)
			{
				// make sure attribute is not disabled
				if (itr->type)
				{
					// make sure we haven't already stored this attrib
					if (!attribsOrdered[itr->name])
					{
						attribsOrdered[itr->name] = itr;
						++ret.vertexNumAttribs;
					}
					else
						printf("\n A3 Warning: Duplicate attribute (%u) passed to vertex descriptor; ignoring.", itr->name);
				}
				else
					printf("\n A3 Warning: Disabled attribute (%u) passed to vertex descriptor; ignoring.", itr->name);
			}

			// verify count again
			if (ret.vertexNumAttribs)
			{
				// iterate through all attribs, 
				//	counting offset for those enabled
				for (i = 0; i < a3attrib_nameMax; ++i)
				{
					itr = attribsOrdered[i];
					if (itr)
					{
						// copy info for ordered attribute
						ret.attribType[i] = a3vertexInternalGetType(itr->type);
						ret.attribOffset[i] = ret.vertexSize;
						ret.attribElements[i] = a3vertexAttribGetElementsPerAttrib(itr->type);
						ret.attribSize[i] = ret.attribElements[i] * a3vertexAttribGetBytesPerElement(itr->type);

						// increase size (which is offset for next attrib)
						ret.vertexSize += ret.attribSize[i];
					}
				}

				// done, copy result
				*vertexFormat_out = ret;
				return ret.vertexNumAttribs;
			}
		}

		// fail
		return 0;
	}
	return -1;
}

a3ret a3indexFormatCreateDescriptor(a3_IndexFormatDescriptor *indexFormat_out, const a3_IndexType indexType)
{
	if (indexFormat_out)
	{
		indexFormat_out->indexType = a3indexInternalGetType(indexType);
		indexFormat_out->indexSize = a3indexGetBytesPerIndex(indexType);
		return indexType;
	}
	return -1;
}


//-----------------------------------------------------------------------------
// removed from header

/*
	// A3: Set data for an attribute in an array of pointers.
	//	param dataArray: non-null array of vertex attribute data pointers
	//	param attribName: which attribute slot to store data for
	//	param attribData: pointer to raw attribute data (pass null to disable)
	//	return: attribName if success
	//	return: -1 if invalid param
	a3ret a3vertexAttribSetData(a3_VertexAttributeData dataArray, const a3_VertexAttributeName attribName, const void *attribData);
*/

/*
	// A3: Get the number of bytes per instance of an attribute type.
	//	param attribType: data type used to describe attribute
	//	return: number of bytes contained in an instance of this attribute
	a3ret a3vertexAttribGetBytesPerAttrib(const a3_VertexAttributeType attribType);
*/


//-----------------------------------------------------------------------------
// removed from inline

/*
inline a3ret a3vertexAttribSetData(a3_VertexAttributeData dataArray, const a3_VertexAttributeName attribName, const void *attribData)
{
	if (dataArray)
	{
		dataArray[attribName] = attribData;
		return attribName;
	}
	return -1;
}
*/

/*
inline a3ret a3vertexAttribGetBytesPerAttrib(const a3_VertexAttributeType attribType)
{
	return (a3vertexAttribGetElementsPerAttrib(attribType) * a3vertexAttribGetBytesPerElement(attribType));
}
*/
