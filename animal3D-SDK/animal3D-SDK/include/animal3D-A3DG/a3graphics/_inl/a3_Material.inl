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
	
	a3_Material.inl
	Inline definitions for material.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_MATERIAL_H
#ifndef __ANIMAL3D_MATERIAL_INL
#define __ANIMAL3D_MATERIAL_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3materialUpload(const a3_Material *material)
{
	if (material)
		return a3bufferAppendOffset(material->uniformBuffer, 0, 0, material->dataSize, material->data, 0);
	return -1;
}

A3_INLINE a3ret a3materialUploadSection(const a3_Material *material, const a3ui16 start, const a3ui16 size)
{
	if (material)
		return a3bufferAppendOffset(material->uniformBuffer, 0, start, size, material->data, 0);
	return -1;
}

A3_INLINE a3ret a3materialUploadNew(a3_Material *material, const void *data_opt, const a3ui16 dataSize)
{
	if (material && material->uniformBuffer && dataSize)
	{
		a3i32 ret = a3bufferAppendOffset(material->uniformBuffer, 0, 0, dataSize, data_opt, 0);
		if (ret > 0)
		{
			material->data = data_opt;
			material->dataSize = dataSize;
		}
		return ret;
	}
	return -1;
}

A3_INLINE a3ret a3materialReference(a3_Material *material)
{
	if (material)
	{
		a3ui16 i;
		a3i32 ret = a3bufferReference(material->uniformBuffer);
		a3_MaterialTexture *matTex = material->textureList;
		for (i = 0; i < material->numTextures; ++i, ++matTex)
			a3textureReference(matTex->texture);
		return ret;
	}
	return -1;
}

A3_INLINE a3ret a3materialRelease(a3_Material *material)
{
	if (material)
	{
		a3ui16 i;
		a3i32 ret = a3bufferRelease(material->uniformBuffer);
		a3_MaterialTexture *matTex = material->textureList, dummy = { 0 };
		for (i = 0; i < material->numTextures; ++i, ++matTex)
			a3textureRelease(matTex->texture);
		if (ret == 0)
		{
			material->uniformBuffer = 0;
			matTex = material->textureList;
			for (i = 0; i < material->numTextures; ++i, ++matTex)
				*matTex = dummy;
		}
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_MATERIAL_INL
#endif	// __ANIMAL3D_MATERIAL_H