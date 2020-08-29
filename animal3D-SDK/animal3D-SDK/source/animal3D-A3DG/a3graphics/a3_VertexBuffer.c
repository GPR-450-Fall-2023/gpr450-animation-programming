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
	
	a3_VertexBuffer.c
	Definitions for common vertex buffer utilities.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_VertexBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//-----------------------------------------------------------------------------
// internal utility declarations

void a3vertexArrayInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr);


//-----------------------------------------------------------------------------

inline a3ret a3indexInternalStore(a3_IndexBuffer *indexBuffer, const a3ui32 storage, const void *indexData, a3ui32 *offset_out_opt, const a3ui32 indexCount)
{
	a3i32 ret = a3bufferAppend(indexBuffer, 1, storage, indexData, offset_out_opt);
	if (ret <= 0)
		printf("\n A3 ERROR (IBO %u \'%s\'): \n\t Index data not stored in buffer.", indexBuffer->handle->handle, indexBuffer->handle->name);
	return ret;
}


//-----------------------------------------------------------------------------

a3ret a3vertexBufferStore(a3_VertexBuffer *vertexBuffer, const a3_VertexFormatDescriptor *vertexFormat, const a3_VertexAttributeDataDescriptor *attribRawData, const a3ui32 vertexCount, a3ui32 *offset_out_opt)
{
	a3i32 ret = 0;
	a3ui32 storage;
	a3ui32 attribIndex, vertexIndex;
	a3byte *interleaved, *interleavedPtr;
	const a3byte *attribData[a3attrib_nameMax] = { 0 }, *attribDataPtr;
	a3ui32 attribSize;

	// ALGORITHM: 
	//	- generate CPU-side array to hold the data before shipping to GPU
	//	- for each attribute...
	//		- for each element in this attribute...
	//			- insert into our giant array
	//			***e.g. if element is a vector, insert 2/3/4 items (we are assuming 3)
	//	- stuff GPU-side VBO with raw data in our array!
	//	- free our CPU-side memory allocation

	// PURPOSE: 
	// these are not definitely the attributes in-use, but we'll say 
	//	we are using positions, normals, texcoords: 
	//	positions(cpu) = |pos0|pos1|pos2|pos3|...|
	//	normals(cpu)   = |nrm0|nrm1|nrm2|nrm3|...|
	//	texcoords(cpu) = |tex0|tex1|tex2|tex3|...|
	// 
	// take this data and interleave it so attributes with the same number
	//	are side-by-side: 
	//	interleavedData(cpu) = |pos0|nrm0|tex0|pos1|nrm1|tex1|pos2|nrm2|tex2|pos3|nrm3|tex3|...|...|...|...|
	// contiguous memory means faster access for drawing!

	// validate params
	if (vertexBuffer && vertexFormat && attribData && vertexCount)
	{
		// validate initialized
		if (vertexBuffer->handle->handle)
		{
			// validate other params
			if (vertexFormat->vertexNumAttribs)
			{
				// copy attribute data passed in to internal container
				// use vertex descriptor to dictate how many there are
				for (attribIndex = 0; attribIndex < vertexFormat->vertexNumAttribs; ++attribIndex)
					attribData[attribRawData[attribIndex].name] = (a3byte *)attribRawData[attribIndex].data;

				// confirm that all attributes have matching data
				for (attribIndex = 0; attribIndex < a3attrib_nameMax; ++attribIndex)
					if (vertexFormat->attribType[attribIndex] && !attribData[attribIndex])
						break;

				// if loop completed, all attributes have data
				if (attribIndex == a3attrib_nameMax)
				{
					// check if interleaved data will fit in buffer
					storage = a3vertexFormatGetStorageSpaceRequired(vertexFormat, vertexCount);
					if (a3bufferValidateBlockSize(vertexBuffer, 0, storage) > 0)
					{
						// allocate interleaved data array
						interleaved = (a3byte *)malloc(storage);

						// for each attribute, copy instances in steps
						for (attribIndex = 0; attribIndex < a3attrib_nameMax; ++attribIndex)
						{
							// vertex is used
							if (vertexFormat->attribType[attribIndex])
							{
								interleavedPtr = interleaved + vertexFormat->attribOffset[attribIndex];
								attribDataPtr = attribData[attribIndex];
								attribSize = vertexFormat->attribSize[attribIndex];
								for (vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
								{
									memcpy(interleavedPtr, attribDataPtr, attribSize);
									interleavedPtr += vertexFormat->vertexSize;
									attribDataPtr += attribSize;
								}
							}
						}

						// fill buffer (should definitely return positive 
						//	after all the checks done previously)
						ret = a3bufferAppend(vertexBuffer, 0, storage, interleaved, offset_out_opt);
						if (ret <= 0)
							printf("\n A3 ERROR (VBO %u \'%s\'): \n\t Vertex data not stored in buffer.", vertexBuffer->handle->handle, vertexBuffer->handle->name);

						// release interleaved data
						free(interleaved);
					}
					else
						printf("\n A3 ERROR (VBO %u \'%s\'): \n\t Vertex data will not fit in buffer.", vertexBuffer->handle->handle, vertexBuffer->handle->name);
				}
				else
					printf("\n A3 ERROR (VBO %u \'%s\'): \n\t Vertex attribute (%u) has no data.", vertexBuffer->handle->handle, vertexBuffer->handle->name, attribIndex);
			}
			else
				printf("\n A3 ERROR (VBO %u \'%s\'): \n\t Vertex descriptor has no attributes.", vertexBuffer->handle->handle, vertexBuffer->handle->name);

			// done
			return ret;
		}
	}
	return -1;
}

a3ret a3indexBufferStore(a3_IndexBuffer *indexBuffer, const a3_IndexFormatDescriptor *indexFormat, const void *indexData, const a3ui32 indexCount, const a3ui32 baseIndex, a3ui32 *offset_out_opt, const a3_IndexFormatDescriptor *internalFormat_opt)
{
	a3i32 ret = 0;
	a3ui32 storage = 0;
	a3ui32 i;

	// data pointers
	const a3ubyte *provided1 = indexData;
	const a3ui16 *provided2 = indexData;
	const a3ui32 *provided4 = indexData;
	a3ubyte *internal1;
	a3ui16 *internal2;
	a3ui32 *internal4;
	void *newData;

	// validate
	if (indexBuffer && indexFormat && indexData && indexCount)
	{
		if (indexBuffer->handle->handle)
		{
			storage = a3indexFormatGetStorageSpaceRequired(indexFormat, indexCount);

			// check if common format is being used; faster if not
			if (!internalFormat_opt || internalFormat_opt->indexSize == indexFormat->indexSize)
			{
				if (baseIndex)
				{
					// add base offset
					newData = malloc(storage);
					if (indexFormat->indexSize == 1)
						for (i = 0, internal1 = (a3ubyte *)newData; i < indexCount; ++i)
							*(internal1++) = *(provided1++) + baseIndex;
					else if (indexFormat->indexSize == 2)
						for (i = 0, internal2 = (a3ui16 *)newData; i < indexCount; ++i)
							*(internal2++) = *(provided2++) + baseIndex;
					else
						for (i = 0, internal4 = (a3ui32 *)newData; i < indexCount; ++i)
							*(internal4++) = *(provided4++) + baseIndex;

					// store
					ret = a3indexInternalStore(indexBuffer, storage, newData, offset_out_opt, indexCount);
					free(newData);
				}
				else
				{
					// the fastest option, direct store
					ret = a3indexInternalStore(indexBuffer, storage, indexData, offset_out_opt, indexCount);
				}
			}

			// common format is used and size is different
			else
			{
				storage = a3indexFormatGetStorageSpaceRequired(internalFormat_opt, indexCount);
				newData = malloc(storage);

				// need to know combo for correct interleaving
				// incoming is a3byte
				if (indexFormat->indexSize == 1)
					if (internalFormat_opt->indexSize == 2)
						// convert to a3i16
						for (i = 0, internal2 = (a3ui16 *)newData; i < indexCount; ++i)
							*(internal2++) = (a3ui16)(*(provided1++)) + baseIndex;
					else
						// convert to a3i32
						for (i = 0, internal4 = (a3ui32 *)newData; i < indexCount; ++i)
							*(internal4++) = (a3ui32)(*(provided1++)) + baseIndex;
				// incoming is a3i16
				else if (indexFormat->indexSize == 2)
					if (internalFormat_opt->indexSize == 1)
						// convert to a3byte
						for (i = 0, internal1 = (a3ubyte *)newData; i < indexCount; ++i)
							*(internal1++) = (a3ubyte)(*(provided2++)) + baseIndex;
					else
						// convert to a3i32
						for (i = 0, internal4 = (a3ui32 *)newData; i < indexCount; ++i)
							*(internal4++) = (a3ui32)(*(provided2++)) + baseIndex;
				// incoming is a3i32
				else
					if (internalFormat_opt->indexSize == 1)
						// convert to a3byte
						for (i = 0, internal1 = (a3ubyte *)newData; i < indexCount; ++i)
							*(internal1++) = (a3ubyte)(*(provided4++)) + baseIndex;
					else
						// convert to a3i16
						for (i = 0, internal2 = (a3ui16 *)newData; i < indexCount; ++i)
							*(internal2++) = (a3ui16)(*(provided4++)) + baseIndex;

				// done interleaving; store, free and done
				ret = a3indexInternalStore(indexBuffer, storage, newData, offset_out_opt, indexCount);
				free(newData);
			}

			// done
			return ret;
		}
	}
	return -1;
}


a3ret a3vertexArrayHandleUpdateReleaseCallback(a3_VertexArrayDescriptor *vertexArray)
{
	if (vertexArray)
		return a3handleSetReleaseFunc(vertexArray->handle, a3vertexArrayInternalReleaseFunc);
	return -1;
}


//-----------------------------------------------------------------------------
