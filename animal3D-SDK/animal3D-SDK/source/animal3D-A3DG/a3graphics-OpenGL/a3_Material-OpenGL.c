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
	
	a3_Material-OpenGL.c
	Definitions for OpenGL material management.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_Material.h"

#include "GL/glew.h"


//-----------------------------------------------------------------------------

// utility to activate material
void a3materialInternalActivate(const a3_MaterialTexture *matTex, const a3ui32 numTextures, const a3ui32 unifBuffHandle, const a3ui32 progHandle, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding)
{
	a3ui32 i;

	// activate program
	glUseProgram(progHandle);

	// activate uniform buffer
	glBindBufferBase(GL_UNIFORM_BUFFER, unifBlockBinding, unifBuffHandle);

	// bind program block
	glUniformBlockBinding(progHandle, unifBlockLocation, unifBlockBinding);

	// activate textures
	for (i = 0; i < numTextures; ++i, ++matTex)
	{
		glActiveTexture(GL_TEXTURE0 + matTex->textureUnit);
		glBindTexture(GL_TEXTURE_2D, matTex->texture->handle->handle);
	}
}


//-----------------------------------------------------------------------------
