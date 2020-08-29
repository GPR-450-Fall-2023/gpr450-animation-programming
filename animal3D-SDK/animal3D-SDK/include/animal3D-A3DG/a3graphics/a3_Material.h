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
	
	a3_Material.h
	Helper structures to describe and manage materials.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_MATERIAL_H
#define __ANIMAL3D_MATERIAL_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_Texture.h"
#include "a3_UniformBuffer.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Material				a3_Material;
	typedef struct a3_MaterialTexture		a3_MaterialTexture;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Helper struct to align a texture with a texture unit.
	//	member texture: pointer to texture
	//	member textureUnit: which texture this unit should be activated on
	struct a3_MaterialTexture
	{
		a3_Texture *texture;
		a3_TextureUnit textureUnit;
	};


	// A3: Material helper structure.
	//	member uniformBuffer: uniform buffer associated with this material, 
	//		required member
	//	member data: pointer to raw material data, e.g. a struct, excluding 
	//		texture units
	//	member dataSize: size of raw material data in bytes
	//	member numTextures: number of textures referenced by material
	//	member textureList: list of paired textures and texture units
	struct a3_Material
	{
		a3_UniformBuffer *uniformBuffer;
		const void *data;
		a3ui16 dataSize;
		a3ui16 numTextures;
		a3_MaterialTexture textureList[a3tex_unitMax];
	};


//-----------------------------------------------------------------------------

	// A3: Create material; does not upload any data.
	//	param material_out: non-null pointer to material object to initialize
	//	param uniformBuffer: non-null pointer to initialized uniform buffer
	//	param dataSize: non-zero intended size of data, or size of buffer
	//	param data_opt: optional pointer to material data (e.g. a struct); 
	//		passing null will clear the material data (zero)
	//	param numTextures_opt: optional texture count associated with material; 
	//		has no effect if textureList is null or empty
	//	param textureList_opt: optional array of material texture descriptors; 
	//		has no effect if numTextures is zero; passed textures must be 
	//		initialized or they will be ignored, duplicates are also ignored
	//	return: 1 if success
	//	return: -1 if invalid params or already initialized
	a3ret a3materialCreate(a3_Material *material_out, a3_UniformBuffer *uniformBuffer, const a3ui16 dataSize, const void *data_opt, const a3ui16 numTextures_opt, const a3_MaterialTexture *textureList_opt);

	// A3: Re-upload material data.
	//	param material: non-null pointer to initialized material
	//	return: data size uploaded if success
	//	return: -1 if invalid params or not initialized
	a3ret a3materialUpload(const a3_Material *material);

	// A3: Re-upload a section of the material data.
	//	param material: non-null pointer to initialized material
	//	param start: byte index in data to start
	//	param size: size of data to re-upload; must not exceed end of 
	//		existing data
	//	return: data size uploaded if success
	//	return: 0 if failed
	//	return: -1 if invalid params or not initialized
	a3ret a3materialUploadSection(const a3_Material *material, const a3ui16 start, const a3ui16 size);

	// A3: Re-assign and upload material data.
	//	param material: non-null pointer to initialized material
	//	param data_opt: optional pointer to new data; pass null to clear
	//	param dataSize: non-zero size in bytes of new data
	//	return: data size uploaded if success
	//	return: -1 if invalid params or not initialized
	a3ret a3materialUploadNew(a3_Material *material, const void *data_opt, const a3ui16 dataSize);

	// A3: Activate material for rendering; binds uniform buffer and textures.
	//	param material: non-null pointer to initialized material
	//	param shaderProgram: non-null pointer to initialized shader program
	//	param unifBlockLocation: location in current program of uniform block
	//	param unifBlockBinding: binding slot for uniform buffer
	//	return: 1 if success
	//	return: -1 if invalid params or not initialized
	a3ret a3materialActivate(const a3_Material *material, const a3_ShaderProgram *shaderProgram, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding);

	// A3: Activate material and also upload new data.
	//	param material: non-null pointer to initialized material
	//	param shaderProgram: non-null pointer to initialized shader program
	//	param unifBlockLocation: location in current program of uniform block
	//	param unifBlockBinding: binding slot for uniform buffer
	//	param data_opt: optional pointer to new data; pass null to clear
	//	param dataSize: non-zero size in bytes of new data
	//	return: 1 if success
	//	return: -1 if invalid params or not initialized
	a3ret a3materialActivateUploadNew(a3_Material *material, const a3_ShaderProgram *shaderProgram, const a3i32 unifBlockLocation, const a3ui32 unifBlockBinding, const void *data_opt, const a3ui16 dataSize);

	// A3: Reference material.
	//	param material: non-null pointer to initialized material to reference
	//	return: new reference count of uniform buffer if success
	//	return: -1 if invalid params or not initialized
	a3ret a3materialReference(a3_Material *material);

	// A3: Release material.
	//	param material: non-null pointer to initialized material to release
	//	return: new reference count of uniform buffer
	//	return: 0 if permanently released (uniform buffer released)
	//	return: -1 if invalid params or not initialized
	a3ret a3materialRelease(a3_Material *material);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_Material.inl"


#endif	// !__ANIMAL3D_MATERIAL_H