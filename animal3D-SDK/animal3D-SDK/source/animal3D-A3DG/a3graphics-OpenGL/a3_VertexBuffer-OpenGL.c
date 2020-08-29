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
	
	a3_VertexBuffer-OpenGL.c
	Definitions for OpenGL vertex buffer utilities.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_VertexBuffer.h"

#include "GL/glew.h"

#include <stdio.h>


// A3: buffer pointer offset utility: offset a pointer by 'n' bytes
#define A3_BUFFER_OFFSET(n) ((a3byte *)(0) + (n))


//-----------------------------------------------------------------------------

// get attribute internal types
a3ui16 a3vertexInternalGetType(const a3_VertexAttributeType type)
{
	static const a3ui16 internalType[] = { 0, GL_INT, GL_INT, GL_INT, GL_INT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_DOUBLE, GL_DOUBLE, GL_DOUBLE, GL_DOUBLE };
	return internalType[type];
}

// get index internal types
a3ui16 a3indexInternalGetType(const a3_IndexType type)
{
	static const a3ui16 internalType[] = { 0, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT };
	return internalType[type];
}


void a3vertexArrayInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr)
{
	glDeleteVertexArrays(count, handlePtr);
}


//-----------------------------------------------------------------------------

a3ret a3vertexArrayCreateDescriptor(a3_VertexArrayDescriptor *vertexArray_out, const a3byte name_opt[32], a3_VertexBuffer *vertexBuffer, const a3_VertexFormatDescriptor *vertexFormat, const a3ui32 vertexBufferOffset)	//, a3_IndexBuffer *indexBuffer_opt)
{
	a3_VertexArrayDescriptor ret = { 0 };
	a3ui32 handle, i;

	// validate params
	if (vertexArray_out && vertexBuffer && vertexFormat)
	{
		// make sure not already init, buffer is init
		if (!vertexArray_out->handle->handle && vertexBuffer->handle->handle)
		{
			// validate count
			if (vertexFormat->vertexNumAttribs)
			{
				// create and validate handle
				glGenVertexArrays(1, &handle);
				if (handle)
				{
					// bind buffers to be associated
					glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->handle->handle);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					// OLD: IBO no longer reference by VAO, see drawable
				//	if (indexBuffer_opt)
				//		if (indexBuffer_opt->handle->handle)
				//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_opt->handle->handle);
				//		else
				//			printf("\n A3 Warning: Uninitialized index buffer passed to vertex array.");

					// bind VAO to configure
					glBindVertexArray(handle);

					// iterate through attributes and store info, enabling and 
					//	disabling GL attributes as needed
					for (i = 0; i < a3attrib_nameMax; ++i)
					{
						// enable and configure attribute in VAO
						switch (vertexFormat->attribType[i])
						{
						case GL_FLOAT:
							glEnableVertexAttribArray(i);
							glVertexAttribPointer(i,				// attrib index
								vertexFormat->attribElements[i],	// number of components
								GL_FLOAT,							// internal data type
								GL_FALSE,							// no normalization
								vertexFormat->vertexSize,			// stride = vertex size
								A3_BUFFER_OFFSET(					// byte offset from start of vertex buffer
									vertexFormat->attribOffset[i] + vertexBufferOffset
								)
							);
							break;
						case GL_INT:
							glEnableVertexAttribArray(i);
							glVertexAttribIPointer(i,
								vertexFormat->attribElements[i],
								GL_INT,
								vertexFormat->vertexSize,
								A3_BUFFER_OFFSET(
									vertexFormat->attribOffset[i] + vertexBufferOffset
								)
							);
							break;
						case GL_DOUBLE:
							glEnableVertexAttribArray(i);
							glVertexAttribLPointer(i,
								vertexFormat->attribElements[i],
								GL_DOUBLE,
								vertexFormat->vertexSize,
								A3_BUFFER_OFFSET(
									vertexFormat->attribOffset[i] + vertexBufferOffset
								)
							);
							break;
						default:
							// attribute is not used
							glDisableVertexAttribArray(i);
							break;
						}
					}

					// done, disable handles and copy result
					glBindVertexArray(0);
					a3handleCreateHandle(ret.handle, a3vertexArrayInternalReleaseFunc, name_opt, handle, 1);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					ret.vertexBuffer = vertexBuffer;

					// copy format
					*ret.vertexFormat = *vertexFormat;

					// done
					*vertexArray_out = ret;
					a3vertexArrayReferenceDescriptor(vertexArray_out);
					return (vertexFormat->vertexNumAttribs);
				}
				else
					printf("\n A3 ERROR (VAO \'%s\'): \n\t Invalid handle; VAO not created.", name_opt);
			}
			else
				printf("\n A3 ERROR (VAO \'%s\'): \n\t Vertex format with no attributes passed; VAO not created.", name_opt);

			// fail
			return 0;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------
// removed from header

/*
// A3: Deactivate vertex array descriptor.
//	return: 0
a3ret a3vertexArrayDeactivate();
*/


//-----------------------------------------------------------------------------
// removed from inline

/*
		if (a3bufferRelease(vertexArray->indexBuffer) == 0)
			vertexArray->indexBuffer = 0;
*/


//-----------------------------------------------------------------------------
// removed from source

/*
	if (indexBuffer_opt && indexBuffer_opt->handle->handle)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		ret.indexBuffer = indexBuffer_opt;
		a3handleIncrementCount(indexBuffer_opt->handle);
	}
*/
/*
a3ret a3vertexArrayDeactivate()
{
	glBindVertexArray(0);
	return 0;
}
*/
