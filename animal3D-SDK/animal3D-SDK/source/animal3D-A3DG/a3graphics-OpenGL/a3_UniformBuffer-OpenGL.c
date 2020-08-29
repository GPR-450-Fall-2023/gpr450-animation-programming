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
	
	a3_UniformBuffer.c
	Definitions for GLSL uniform buffer utility.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_UniformBuffer.h"

#include "GL/glew.h"


//-----------------------------------------------------------------------------

a3ret a3shaderUniformBufferActivate(const a3_UniformBuffer *buffer, const a3ui32 unifBlockBinding)
{
	a3ui32 handle;
	if (buffer)
	{
		handle = buffer->handle->handle;
		if (handle)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, unifBlockBinding, handle);
			return 1;
		}
	}
	return -1;
}

a3ret a3shaderUniformBlockBind(const a3_ShaderProgram *program, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding)
{
	a3ui32 pHandle;
	if (program && unifBlockLocation >= 0)
	{
		pHandle = program->handle->handle;
		if (pHandle)
		{
			glUniformBlockBinding(pHandle, unifBlockLocation, unifBlockBinding);
			return 1;
		}
	}
	return -1;
}

a3ret a3shaderUniformBlockGetLocation(const a3_ShaderProgram *program, const a3byte *unifBlockName)
{
	if (program && unifBlockName)
		return glGetUniformBlockIndex(program->handle->handle, unifBlockName);
	return -1;
}

a3ret a3shaderUniformBufferMaxBindings()
{
	a3i32 ret[1];
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, ret);
	return *ret;
}

a3ret a3shaderUniformBlockMaxSize()
{
	a3i32 ret[1];
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, ret);
	return *ret;
}

a3ret a3shaderUniformBlockMaxCount(const a3_ShaderType shaderType)
{
	static const a3ui16 shaderTypeQuery[] = { GL_MAX_VERTEX_UNIFORM_BLOCKS, GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS, GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS, GL_MAX_GEOMETRY_UNIFORM_BLOCKS, GL_MAX_FRAGMENT_UNIFORM_BLOCKS, GL_MAX_COMPUTE_UNIFORM_BLOCKS };
	a3i32 ret[1];
	glGetIntegerv(shaderTypeQuery[shaderType], ret);
	return *ret;
}


//-----------------------------------------------------------------------------
