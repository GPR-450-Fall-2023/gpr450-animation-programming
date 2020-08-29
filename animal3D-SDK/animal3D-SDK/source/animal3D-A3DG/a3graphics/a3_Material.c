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
	
	a3_Material.c
	Definitions for common material management functions.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_Material.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// internal utility declarations

void a3bufferInternalFillSub(a3i32 binding, a3ui32 start, a3ui32 size, const void *data);

void a3materialInternalActivate(const a3_MaterialTexture *matTex, const a3ui32 numTextures, const a3ui32 unifBuffHandle, const a3ui32 progHandle, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding);


//-----------------------------------------------------------------------------

a3ret a3materialCreate(a3_Material *material_out, a3_UniformBuffer *uniformBuffer, const a3ui16 dataSize, const void *data_opt, const a3ui16 numTextures_opt, const a3_MaterialTexture *textureList_opt)
{
	a3_Material ret = { 0 };
	const a3_MaterialTexture *matTexOrdered[a3tex_unitMax] = { 0 }, *matTex;
	a3ui16 i, j;

	if (material_out && uniformBuffer && uniformBuffer->handle->handle && dataSize)
	{
		if (!material_out->uniformBuffer)
		{
			// set what we can to start
			ret.uniformBuffer = uniformBuffer;
			ret.data = data_opt;
			ret.dataSize = dataSize;

			// copy textures and units, but ignore duplicate units
			if (numTextures_opt && textureList_opt)
			{
				matTex = textureList_opt;
				for (i = 0; i < numTextures_opt; ++i, ++matTex)
				{
					// check if texture is valid and initialized
					if (matTex->texture)
					{
						if (matTex->texture->handle->handle)
						{
							// avoid duplicates
							if (!matTexOrdered[matTex->textureUnit])
							{
								// store texture
								matTexOrdered[matTex->textureUnit] = matTex;
								++ret.numTextures;
							}
							else
								printf("\n A3 Warning: Duplicate texture unit (%u) passed to material; ignoring", matTex->textureUnit);
						}
						else
							printf("\n A3 Warning: Uninitialized texture (%u) passed to material; ignoring.", matTex->textureUnit);
					}
					else
						printf("\n A3 Warning: Disabled texture (%u) passed to material; ignoring.", matTex->textureUnit);
				}

				// verify textures should be stored
				if (ret.numTextures)
				{
					// copy from ordered into return
					for (i = j = 0; j < ret.numTextures; ++i)
					{
						matTex = matTexOrdered[i];
						if (matTex)
							ret.textureList[j++] = *matTex;
					}
				}
			}

			// done
			*material_out = ret;
			a3materialReference(material_out);
			return 1;
		}
	}
	return -1;
}

a3ret a3materialActivate(const a3_Material *material, const a3_ShaderProgram *shaderProgram, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding)
{
	a3ui32 pHandle;
	if (material && material->uniformBuffer && shaderProgram && unifBlockLocation >= 0)
	{
		pHandle = shaderProgram->handle->handle;
		if (pHandle)
		{
			// activate and done
			a3materialInternalActivate(material->textureList, material->numTextures, material->uniformBuffer->handle->handle, pHandle, unifBlockLocation, unifBlockBinding);
			return 1;
		}
	}
	return -1;
}

a3ret a3materialActivateUploadNew(a3_Material *material, const a3_ShaderProgram *shaderProgram, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding, const void *data_opt, const a3ui16 dataSize)
{
	a3ui32 pHandle;
	if (material && material->uniformBuffer && shaderProgram && unifBlockLocation >= 0)
	{
		pHandle = shaderProgram->handle->handle;
		if (pHandle)
		{
			// activate
			a3materialInternalActivate(material->textureList, material->numTextures, material->uniformBuffer->handle->handle, pHandle, unifBlockLocation, unifBlockBinding);

			// upload if size is valid
			if (dataSize <= material->uniformBuffer->size)
			{
				a3bufferInternalFillSub(material->uniformBuffer->internalBinding, 0, dataSize, data_opt);
				material->data = data_opt;
				material->dataSize = dataSize;
			}

			// done
			return 1;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------
