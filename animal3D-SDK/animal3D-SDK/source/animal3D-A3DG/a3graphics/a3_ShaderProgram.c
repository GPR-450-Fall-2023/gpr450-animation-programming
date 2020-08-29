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
	
	a3_ShaderProgram.c
	Definitions for common shader and program utility functions.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_ShaderProgram.h"

#include "animal3D/a3utility/a3_Stream.h"

#include <stdio.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// internal utility declarations

void a3shaderInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr);
void a3shaderProgramInternalReleaseFunc(a3i32 count, a3ui32 *handlePtr);


//-----------------------------------------------------------------------------

// get shader type as string
const a3byte *a3shaderInternalGetTypeStr(const a3_ShaderType type)
{
	// shader type strings
	static const a3byte shaderTypeStr[6][10] = {
		"vertex", "tess ctrl", "tess eval", "geometry", "fragment", "compute",
	};
	return (shaderTypeStr[type]);
}


//-----------------------------------------------------------------------------
// shader

a3ret a3shaderCreateFromFileList(a3_Shader *shader_out, const a3byte name_opt[32], const a3_ShaderType type, const a3byte **filePathList, const a3ui32 count)
{
	a3ui32 newCount, i;
	a3i32 result;
	a3_Stream fs[1] = { 0 };
	const a3byte **itr, **valid;

	if (shader_out && filePathList && count)
	{
		// check for blank file paths, decode file contents as necessary
		valid = (a3byte **)malloc(count * sizeof(a3byte *));
		for (i = newCount = 0, itr = filePathList; i < count; ++i, ++itr)
		{
			if (a3streamLoadContents(fs, *itr) > 0)
			{
				valid[newCount++] = fs->contents;
				fs->contents = 0;
			}
		}

		// use sources to load shader
		result = a3shaderCreateFromSourceList(shader_out, name_opt, type, valid, newCount);

		// release file contents
		for (i = 0, itr = valid; i < newCount; ++i, ++itr)
		{
			fs->contents = *itr;
			a3streamReleaseContents(fs);
		}

		// done
		// release source list
		free((void *)valid);
		return result;
	}
	return -1;
}


a3ret a3shaderHandleUpdateReleaseCallback(a3_Shader *shader)
{
	if (shader)
		return a3handleSetReleaseFunc(shader->handle, a3shaderInternalReleaseFunc);
	return -1;
}


//-----------------------------------------------------------------------------
// shader program

a3ret a3shaderProgramHandleUpdateReleaseCallback(a3_ShaderProgram *program)
{
	if (program)
		return a3handleSetReleaseFunc(program->handle, a3shaderProgramInternalReleaseFunc);
	return -1;
}


//-----------------------------------------------------------------------------
