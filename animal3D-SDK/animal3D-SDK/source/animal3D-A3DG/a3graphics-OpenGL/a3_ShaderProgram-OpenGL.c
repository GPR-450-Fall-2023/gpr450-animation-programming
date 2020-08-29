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
	
	a3_ShaderProgram-OpenGL.c
	Definitions for GLSL shader and program utilities.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_ShaderProgram.h"

#include "animal3D/a3utility/a3_Stream.h"

#include "GL/glew.h"

#include <stdio.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------

const a3byte *a3shaderInternalGetTypeStr(const a3_ShaderType type);


//-----------------------------------------------------------------------------

// print shader or program log
void a3shaderInternalPrintLog(const a3boolean isProgram, const a3boolean isLink, const a3ui32 handle, const a3byte *name)
{
	static a3byte log[1024];
	a3i32 logLength;

	if (isProgram)
	{
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			glGetProgramInfoLog(handle, logLength, &logLength, log);
			printf("\n A3 ERROR (PROG %u \'%s\'): \n\t Program %s failed; log: \n%s", handle, name, (isLink ? "link" : "validate"), log);
		}
	}
	else
	{
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			glGetShaderInfoLog(handle, logLength, &logLength, log);
			printf("\n A3 ERROR (SHDR \'%s\'): \n\t Compile failed; shader not created; log: \n%s", name, log);
		}
	}
}


//-----------------------------------------------------------------------------

void a3shaderInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr)
{
	glDeleteShader(*handlePtr);
}

void a3shaderProgramInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr)
{
	glDeleteProgram(*handlePtr);
}


//-----------------------------------------------------------------------------

a3ret a3shaderCreateFromSourceList(a3_Shader *shader_out, const a3byte name_opt[32], const a3_ShaderType type, const a3byte **sourceList, const a3ui32 count)
{
	static const a3ui16 internalShaderType[] = { GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER };

	a3_Shader ret = { 0 };
	a3ui32 handle;
	a3ui32 newCount, i;
	a3i32 status, result = 0;
	const a3byte **itr, **valid;

	// validate params
	if (shader_out && sourceList && count)
	{
		// not already used
		if (!shader_out->handle->handle)
		{
			// sort source list, searching for empties
			valid = (a3byte **)malloc(count * sizeof(a3byte *));
			for (i = newCount = 0, itr = sourceList; i < count; ++i, ++itr)
			{
				// if source is valid
				// set current valid string pointer and increment count
				if (*itr && **itr)
					valid[newCount++] = *itr;
			}

			// if there are valid source strings
			if (newCount)
			{
				// ALGORITHM: 
				//	- create shader object on GPU
				//	- send shader source to GPU
				//	- compile source
				//	- if compile succeeds, return shader handle
				//	- else, print error message

				// generate handle
				handle = glCreateShader(internalShaderType[type]);
				if (handle)
				{
					// attach sources, compile
					glShaderSource(handle, newCount, valid, 0);
					glCompileShader(handle);
					glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
					if (status)
					{
						// finished
						a3handleCreateHandle(ret.handle, a3shaderInternalReleaseFunc, name_opt, handle, 0);
						ret.compiled = result = newCount;
						ret.type = type;
						*shader_out = ret;
						a3shaderReference(shader_out);
					}
					else
					{
						// failed, get log
						a3shaderInternalPrintLog(0, 0, handle, name_opt);

						// reset shader and exit 
						glDeleteShader(handle);
					}
				}
				else
					printf("\n A3 ERROR (SHDR \'%s\'): \n\t Invalid handle; shader not created.", name_opt);
			}
			free((void *)valid);
			return result;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

a3ret a3shaderProgramCreate(a3_ShaderProgram *program_out, const a3byte name_opt[32])
{
	a3_ShaderProgram ret = { 0 };
	a3ui32 handle;

	// validate
	if (program_out)
	{
		// not already used
		if (!program_out->handle->handle)
		{
			// create program object
			handle = glCreateProgram();
			if (handle)
			{
				// done
				a3handleCreateHandle(ret.handle, a3shaderProgramInternalReleaseFunc, name_opt, handle, 0);
				*program_out = ret;
				a3shaderProgramReference(program_out);
				return 1;
			}
			else
				printf("\n A3 ERROR (PROG \'%s\'): \n\t Invalid handle; program not created.", name_opt);

			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3shaderProgramAttachShader(a3_ShaderProgram *program, a3_Shader *shader)
{
	// like building our own custom pipeline: 
	//			   ________ ________ ________ ________ ________
	// attribs -> |________|FIXEDTCS|FIXEDTES|FIXED GS|________| -> frame
	//				^ ^	^								^ ^	^
	// EXAMPLE: attaching a vertex or fragment shader: 
	//				^ ^ ^								^ ^	^
	//	install vertex shader here		...and/or fragment shader here
	//			   _^ ^ ^ _							   _^ ^	^ _
	//			  |_NEW VS_|						  |_NEW FS_|
	//	
	a3ui16 *shaderAttach;
	a3ui32 pHandle, sHandle;

	// validate
	if (program && shader)
	{
		// valid handles
		pHandle = program->handle->handle;
		sHandle = shader->handle->handle;
		if (pHandle && sHandle)
		{
			// check if already linked
			if (!program->linked)
			{
				// first check if shader of this type is already attached
				shaderAttach = program->shadersAttached + shader->type;
				if (!*shaderAttach)
				{
					// if this is a compute shader, no other shader can be attached
					// otherwise, a compute shader cannot be attached
					// (compute shaders have a program of their own)
					if (shader->type != a3shader_compute)
					{
						if (!program->shadersAttached[a3shader_compute])
						{
							// attach
							glAttachShader(pHandle, sHandle);
							*shaderAttach = sHandle;
							++shader->attached;
							return 1;
						}
						else
							printf("\n A3 ERROR (PROG %u \'%s\'): \n\t Cannot attach non-compute shader (%u \'%s\') to compute program.", pHandle, program->handle->name, sHandle, shader->handle->name);
					}
					else
					{
						if (!program->shadersAttached[a3shader_vertex] &&
							!program->shadersAttached[a3shader_tessellationControl] &&
							!program->shadersAttached[a3shader_tessellationEvaluation] &&
							!program->shadersAttached[a3shader_geometry] &&
							!program->shadersAttached[a3shader_fragment])
						{
							// attach
							glAttachShader(pHandle, sHandle);
							*shaderAttach = sHandle;
							++shader->attached;
							return 1;
						}
						else
							printf("\n A3 ERROR (PROG %u \'%s\'): \n\t Cannot attach compute shader (%u \'%s\') to non-compute program.", pHandle, program->handle->name, sHandle, shader->handle->name);
					}
				}
				else
					printf("\n A3 ERROR (PROG %u \'%s\'): \n\t Program already has %s shader; cannot attach shader (%u \'%s\').", pHandle, program->handle->name, a3shaderInternalGetTypeStr(shader->type), sHandle, shader->handle->name);
			}
			else
				printf("\n A3 ERROR (PROG %u \'%s\'): \n\t Program already linked; cannot attach shader (%u \'%s\').", pHandle, program->handle->name, sHandle, shader->handle->name);

			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3shaderProgramDetachShaderType(a3_ShaderProgram *program, const a3_ShaderType type)
{
	a3ui16 *shaderDetach;
	a3ui32 pHandle;
	
	if (program)
	{
		// handle exists
		pHandle = program->handle->handle;
		if (pHandle)
		{
			if (!program->linked)
			{
				shaderDetach = program->shadersAttached + type;
				if (*shaderDetach)
				{
					glDetachShader(pHandle, *shaderDetach);
					*shaderDetach = 0;
					return 1;
				}
				else
					printf("\n A3 WARNING (PROG %u \'%s\'): \n\t Program does not have %s shader attached; no shader detached.", pHandle, program->handle->name, a3shaderInternalGetTypeStr(type));
			}
			else
				printf("\n A3 ERROR (PROG %u \'%s\'): \n\t Program already linked, cannot detach %s shader.", pHandle, program->handle->name, a3shaderInternalGetTypeStr(type));

			return 0;
		}
	}
	return -1;
}

a3ret a3shaderProgramLink(a3_ShaderProgram *program)
{
	a3ui32 pHandle;
	a3i32 status;

	if (program)
	{
		pHandle = program->handle->handle;
		if (pHandle)
		{
			// not already linked
			if (!program->linked)
			{
				// ALGORITHM: 
				//	- link program (consolidate pipeline)
				//	- if link succeeds, return some indication of success
				//	- else, print error message
				//	***DO NOT DELETE PROGRAM!!! we may have a contingency plan!

				glLinkProgram(pHandle);
				glGetProgramiv(pHandle, GL_LINK_STATUS, &status);
				if (status)
				{
					// good
					program->linked = 1;
					return 1;
				}

				// failed, get log
				a3shaderInternalPrintLog(1, 1, pHandle, program->handle->name);
			}
			else
				printf("\n A3 WARNING (PROG %u \'%s\'): \n\t Program already linked; program not re-linked.", pHandle, program->handle->name);

			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3shaderProgramValidate(a3_ShaderProgram *program)
{
	a3ui32 pHandle;
	a3i32 status;

	if (program)
	{
		pHandle = program->handle->handle;
		if (pHandle)
		{
			// not already validated
			if (!program->validated)
			{
				glValidateProgram(pHandle);
				glGetProgramiv(pHandle, GL_VALIDATE_STATUS, &status);
				if (status)
				{
					// good
					program->validated = 1;
					return 1;
				}

				// failed, get log
				a3shaderInternalPrintLog(1, 0, pHandle, program->handle->name);
			}
			else
				printf("\n A3 WARNING (PROG %u \'%s\'): \n\t Program already validated; program not re-validated.", pHandle, program->handle->name);

			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3shaderProgramSaveBinary(const a3_ShaderProgram *program, const a3byte *filePath)
{
	a3_Stream fs[1] = { 0 };
	a3i32 result;
	a3ui32 buffLen, streamLen;
	a3ui32 pHandle;
	a3ui32 *buff;

	if (program && program->handle && program->linked)
	{
		pHandle = program->handle->handle;
		if (pHandle)
		{
			// determine space requirement
			glGetProgramiv(pHandle, GL_PROGRAM_BINARY_LENGTH, &buffLen);

			// allocate buffer to store format and program
			streamLen = buffLen + sizeof(a3ui32);
			buff = (a3ui32 *)malloc(streamLen);

			// get program
			glGetProgramBinary(pHandle, buffLen, &buffLen, buff, buff + 1);

			// save to file
			fs->contents = (a3byte *)buff;
			fs->length = streamLen;
			result = a3streamSaveContents(fs, filePath);

			// done
			free(buff);
			return (result > 0);
		}
	}
	return -1;
}

a3ret a3shaderProgramLoadBinary(a3_ShaderProgram *program, const a3byte *filePath)
{
	a3_Stream fs[1] = { 0 };
	a3i32 result;
	a3ui32 buffLen, streamLen;
	a3ui32 pHandle;
	const a3ui32 *buff;

	if (program && program->handle && !program->linked)
	{
		pHandle = program->handle->handle;
		if (pHandle)
		{
			// load file
			result = a3streamLoadContents(fs, filePath);
			if (result > 0)
			{
				streamLen = fs->length;
				buffLen = streamLen - sizeof(a3ui32);
				buff = (a3ui32 *)(fs->contents);

				// load program and get link status
				glProgramBinary(pHandle, *buff, buff + 1, buffLen);
				glGetProgramiv(pHandle, GL_LINK_STATUS, &result);
				a3streamReleaseContents(fs);

				// check if linked; if not, print log
				if (result == GL_TRUE)
					program->linked = 1;
				else
					a3shaderInternalPrintLog(1, 1, pHandle, program->handle->name);

				// done
				return (result > 0);
			}

			// failed
			return result;
		}
	}
	return -1;
}

a3ret a3shaderProgramActivate(const a3_ShaderProgram *program)
{
	// if pointer and handle are valid
	if (program && program->handle->handle)
	{
		glUseProgram(program->handle->handle);
		return 1;
	}

	// deactivate
	glUseProgram(0);
	return 0;
}

a3ret a3shaderProgramDeactivate()
{
	glUseProgram(0);
	return 0;
}


//-----------------------------------------------------------------------------

a3ret a3shaderUniformGetLocation(const a3_ShaderProgram *program, const a3byte *uniformName)
{
	if (program && uniformName)
		return glGetUniformLocation(program->handle->handle, uniformName);
	return -1;
}

a3ret a3shaderUniformSendInt(const a3_UniformType uniformType, const a3i32 uniformLocation, const a3ui32 count, const a3i32 *values)
{
	if (uniformLocation >= 0 && count && values)
	{
		switch (uniformType)
		{
		case a3unif_single:
			glUniform1iv(uniformLocation, count, values);
			break;
		case a3unif_vec2:
			glUniform2iv(uniformLocation, count, values);
			break;
		case a3unif_vec3:
			glUniform3iv(uniformLocation, count, values);
			break;
		case a3unif_vec4:
			glUniform4iv(uniformLocation, count, values);
			break;
		}
		return count;
	}
	return -1;
}

a3ret a3shaderUniformSendFloat(const a3_UniformType uniformType, const a3i32 uniformLocation, const a3ui32 count, const a3f32 *values)
{
	if (uniformLocation >= 0 && count && values)
	{
		switch (uniformType)
		{
		case a3unif_single:
			glUniform1fv(uniformLocation, count, values);
			break;
		case a3unif_vec2:
			glUniform2fv(uniformLocation, count, values);
			break;
		case a3unif_vec3:
			glUniform3fv(uniformLocation, count, values);
			break;
		case a3unif_vec4:
			glUniform4fv(uniformLocation, count, values);
			break;
		}
		return count;
	}
	return -1;
}

a3ret a3shaderUniformSendDouble(const a3_UniformType uniformType, const a3i32 uniformLocation, const a3ui32 count, const a3f64 *values)
{
	if (uniformLocation >= 0 && count && values)
	{
		switch (uniformType)
		{
		case a3unif_single:
			glUniform1dv(uniformLocation, count, values);
			break;
		case a3unif_vec2:
			glUniform2dv(uniformLocation, count, values);
			break;
		case a3unif_vec3:
			glUniform3dv(uniformLocation, count, values);
			break;
		case a3unif_vec4:
			glUniform4dv(uniformLocation, count, values);
			break;
		}
		return count;
	}
	return -1;
}

a3ret a3shaderUniformSendFloatMat(const a3_UniformMatType uniformType, const a3boolean transpose, const a3i32 uniformLocation, const a3ui32 count, const a3f32 *values)
{
	if (uniformLocation >= 0 && count && values)
	{
		switch (uniformType)
		{
		case a3unif_mat2:
			glUniformMatrix2fv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat3:
			glUniformMatrix3fv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat4:
			glUniformMatrix4fv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat2x3:
			glUniformMatrix2x3fv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat2x4:
			glUniformMatrix2x4fv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat3x4:
			glUniformMatrix3x4fv(uniformLocation, count, transpose, values);
			break;
		}
		return count;
	}
	return -1;
}

a3ret a3shaderUniformSendDoubleMat(const a3_UniformMatType uniformType, const a3boolean transpose, const a3i32 uniformLocation, const a3ui32 count, const a3f64 *values)
{
	if (uniformLocation >= 0 && count && values)
	{
		switch (uniformType)
		{
		case a3unif_mat2:
			glUniformMatrix2dv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat3:
			glUniformMatrix3dv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat4:
			glUniformMatrix4dv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat2x3:
			glUniformMatrix2x3dv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat2x4:
			glUniformMatrix2x4dv(uniformLocation, count, transpose, values);
			break;
		case a3unif_mat3x4:
			glUniformMatrix3x4dv(uniformLocation, count, transpose, values);
			break;
		}
		return count;
	}
	return -1;
}

a3ret a3shaderUniformMaxLocations()
{
	a3i32 ret[1];
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, ret);
	return *ret;
}


//-----------------------------------------------------------------------------
