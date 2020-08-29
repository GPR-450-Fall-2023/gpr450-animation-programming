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
	
	a3_ShaderProgram.inl
	GLSL shader and program inline definitions.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_SHADERPROGRAM_H
#ifndef __ANIMAL3D_SHADERPROGRAM_INL
#define __ANIMAL3D_SHADERPROGRAM_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3shaderCreateFromSource(a3_Shader *shader_out, const a3byte name_opt[32], const a3_ShaderType type, const a3byte *source)
{
	const a3byte *sourceList[] = { source };
	return a3shaderCreateFromSourceList(shader_out, name_opt, type, sourceList, 1);
}

A3_INLINE a3ret a3shaderCreateFromFile(a3_Shader *shader_out, const a3byte name_opt[32], const a3_ShaderType type, const a3byte *filePath)
{
	const a3byte *filePathList[] = { filePath };
	return a3shaderCreateFromFileList(shader_out, name_opt, type, filePathList, 1);
}

A3_INLINE a3ret a3shaderReference(a3_Shader *shader)
{
	if (shader)
		return a3handleIncrementCount(shader->handle);
	return -1;
}

A3_INLINE a3ret a3shaderRelease(a3_Shader *shader)
{
	if (shader)
	{
		a3i32 ret = a3handleDecrementCount(shader->handle);
		if (ret == 0)
			shader->compiled = shader->attached = 0;
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3shaderProgramReference(a3_ShaderProgram *program)
{
	if (program)
		return a3handleIncrementCount(program->handle);
	return -1;
}

A3_INLINE a3ret a3shaderProgramRelease(a3_ShaderProgram *program)
{
	if (program)
	{
		a3i32 ret = a3handleDecrementCount(program->handle);
		if (ret == 0)
			program->linked = program->validated = 0;
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3shaderProgramCreateSelfContained(a3_ShaderProgram *program_out, const a3byte name_opt[32], const a3byte *filePath_vs, const a3byte *filePath_gs, const a3byte *filePath_fs)
{
	a3_Shader shaders[3] = { 0 };

	if (a3shaderProgramCreate(program_out, name_opt) > 0)
	{
		// attempt to load VS; required for this implementation
		if (a3shaderCreateFromFile(shaders + 0, "tmp_vs", a3shader_vertex, filePath_vs) > 0)
		{
			// attach VS
			a3shaderProgramAttachShader(program_out, shaders + 0);

			// attempt to load GS if one was requested
			if (a3shaderCreateFromFile(shaders + 1, "tmp_gs", a3shader_geometry, filePath_gs) > 0)
				a3shaderProgramAttachShader(program_out, shaders + 1);

			// attempt to load FS if one was requested
			if (a3shaderCreateFromFile(shaders + 2, "tmp_fs", a3shader_fragment, filePath_fs) > 0)
				a3shaderProgramAttachShader(program_out, shaders + 2);

			// link and validate
			a3shaderProgramLink(program_out);
			a3shaderProgramValidate(program_out);

			// delete all shaders
			a3shaderRelease(shaders + 0);
			a3shaderRelease(shaders + 1);
			a3shaderRelease(shaders + 2);

			// done
			return 1;
		}

		// fail
		return 0;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SHADERPROGRAM_INL
#endif	// __ANIMAL3D_SHADERPROGRAM_H