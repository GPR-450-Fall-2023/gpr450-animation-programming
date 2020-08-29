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
	
	a3_UniformBuffer.h
	Uniform buffer object (UBO) wrapper, an add-on to the GLSL utilities.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_UNIFORMBUFFER_H
#define __ANIMAL3D_UNIFORMBUFFER_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_BufferObject.h"
#include "a3_ShaderProgram.h"


#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus

	
//-----------------------------------------------------------------------------

	// A3: Uniform buffer object (UBO) is an alias for generic buffer object.
	typedef a3_BufferObject					a3_UniformBuffer;


//-----------------------------------------------------------------------------

	// A3: Bind uniform buffer to specified binding slot (programs share this).
	//	param buffer: non-null pointer to initialized buffer
	//	param unifBlockBinding: program block binding index for this buffer
	//	return: 1 if success
	//	return: -1 if invalid params or buffer not initialized
	a3ret a3shaderUniformBufferActivate(const a3_UniformBuffer *buffer, const a3ui32 unifBlockBinding);

	// A3: Bind uniform block to specified binding slot in a program.
	//	param program: non-null pointer to initialized program
	//	param unifBlockLocation: non-negative location of uniform block in 
	//		specified program
	//	param unifBlockBinding: program binding index for this uniform block; 
	//		this should match the binding used in 'UniformBufferActivate' for 
	//		the buffer that holds the data for this block
	//	return: 1 if success
	//	return: -1 if invalid params or params not initialized
	a3ret a3shaderUniformBlockBind(const a3_ShaderProgram *program, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding);

	// A3: Get uniform block location in a shader program (i.e. a structure 
	//		of uniforms as opposed to a single uniform).
	//	param program: non-null pointer to initialized program
	//	param unifBlockName: non-null, non-empty cstring of block name
	//	return: location of uniform block in program if success
	//	return: -1 if invalid params or uniform block was not found
	a3ret a3shaderUniformBlockGetLocation(const a3_ShaderProgram *program, const a3byte *unifBlockName);

	// A3: Get the maximum number of uniform buffer binding slots.
	//	return: maximum number of uniform buffer slots
	a3ret a3shaderUniformBufferMaxBindings();

	// A3: Get the maximum uniform block size.
	//	return: maximum uniform block size
	a3ret a3shaderUniformBlockMaxSize();

	// A3: Get the maximum number of uniform blocks per shader type.
	//	param shaderType: shader type index from enum
	//	return: maximum number of uniform blocks for the specified shader type
	a3ret a3shaderUniformBlockMaxCount(const a3_ShaderType shaderType);

	
//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_UNIFORMBUFFER_H