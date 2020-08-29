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
	
	a3_Texture.h
	Generic wrapper for OpenGL texture (namely 2D) object.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_TEXTURE_H
#define __ANIMAL3D_TEXTURE_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_GraphicsObjectHandle.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Texture						a3_Texture;
	typedef struct a3_TexturePixelFormatDescriptor	a3_TexturePixelFormatDescriptor;
	typedef enum a3_TexturePixelType				a3_TexturePixelType;
	typedef enum a3_TextureUnit						a3_TextureUnit;
	typedef enum a3_TextureFilterOption				a3_TextureFilterOption;
	typedef enum a3_TextureRepeatOption				a3_TextureRepeatOption;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: OpenGL 2D texture descriptor.
	//	member handle: graphics object handle
	//	members width, height: the dimensions of the image
	//	member channels: the number of channels per pixel
	//	member bytes: the number of bytes per pixel
	//	member internalFormat: OpenGL texture format flag
	//	member internalType: OpenGL type flag
	struct a3_Texture
	{
		a3_GraphicsObjectHandle handle[1];
		a3ui32 width, height;
		a3ui32 channels, bytes;
		a3ui16 internalFormat, internalType;
	};

	// A3: Structure with details about a pixel format from a given type.
	//	member internalFormat: describes the channel arrangement
	//	member internalFormatBits: channel arrangement and size flag
	//	member internalDataType: OpenGL data type flag for channels
	//	member channelsPerPixel: how many color channels per pixel
	//	member bytesPerChannel: how many bytes per channel
	struct a3_TexturePixelFormatDescriptor
	{
		a3ui16 internalFormat;
		a3ui16 internalFormatBits;
		a3ui16 internalDataType;
		a3ubyte channelsPerPixel;
		a3ubyte bytesPerChannel;
	};


	// A3: Format of texture pixel.
	enum a3_TexturePixelType
	{
		// single channel
		a3tex_r8,				// 8 bits per channel (byte)
		a3tex_r16,				// 16 bits per channel (short)
		a3tex_r32F,				// 32 bits per channel (float)

		// two channels
		a3tex_rg8,
		a3tex_rg16,
		a3tex_rg32F,

		// 3-channel red-green-blue
		a3tex_rgb8,
		a3tex_rgb16,
		a3tex_rgb32F,

		// 4-channel red-green-blue-alpha
		a3tex_rgba8,
		a3tex_rgba16,
		a3tex_rgba32F,

		// luminance and depth textures
		a3tex_luminance8,		// 8-bit luminance texture
		a3tex_luminance16,		// 16-bit luminance
		a3tex_depth16,			// 16-bit depth texture
		a3tex_depth24,			// 24-bit depth
		a3tex_depth32,			// 32-bit depth (a3i32)
		a3tex_depth24_stencil8,	// 24-bit depth with 8-bit stencil
	};

	// A3: Texture unit for activating textures.
	enum a3_TextureUnit
	{
		a3tex_unit00,
		a3tex_unit01,
		a3tex_unit02,
		a3tex_unit03,
		a3tex_unit04,
		a3tex_unit05,
		a3tex_unit06,
		a3tex_unit07,
		a3tex_unit08,
		a3tex_unit09,
		a3tex_unit10,
		a3tex_unit11,
		a3tex_unit12,
		a3tex_unit13,
		a3tex_unit14,
		a3tex_unit15,
	};

	// A3: Maximum texture units available.
	enum a3_TextureUnitsMax
	{
		a3tex_unitMax = 16
	};


	// A3: Texture setting options for filter.
	enum a3_TextureFilterOption
	{
		a3tex_filterNearest,	// magnified pixels clamp to nearest neighbor
		a3tex_filterLinear,		// magnified pixels blend with neighbors
	};

	// A3: Texture setting options for repeating textures.
	enum a3_TextureRepeatOption
	{
		a3tex_repeatClamp,		// texture does not repeat; edge pixels stretch
		a3tex_repeatMirror,		// texture mirrors at edges
		a3tex_repeatNormal,		// texture repeats
	};

	
//-----------------------------------------------------------------------------

	// A3: Create a format descriptor given a type of pixel.
	//	param pixelFormat_out: non-null pointer to pixel descriptor
	//	param pixelType: high-level pixel type enum
	//	return: pixel format type if success
	//	return: -1 if invalid params
	a3ret a3textureCreatePixelFormatDescriptor(a3_TexturePixelFormatDescriptor *pixelFormat_out, const a3_TexturePixelType pixelType);

	
//-----------------------------------------------------------------------------

	// A3: Initialize image library (DevIL).
	//	return: 1 if initialized
	//	return: 0 if already initialized
	a3ret a3textureInitializeImageLibrary();

	// A3: Load texture from image file.
	//	param texture_out: non-null pointer to uninitialized texture
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param filePath: non-null, valid cstring of file path to load form
	//	return: 1 if successful load
	//	return: 0 if load failed
	//	return: -1 if invalid params or already initialized
	a3ret a3textureCreateFromFile(a3_Texture *texture_out, const a3byte name_opt[32], const a3byte *filePath);

	// A3: Create texture from raw data.
	//	param texture_out: non-null pointer to uninitialized texture
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param pixelFormat: pixel format to use for texture
	//	params width, height: positive dimensions of texture
	//	param data_opt: optional pointer to raw texture data
	//	param dataFlipped: flag that says whether the data passed is flipped
	//	return: 1 if successful creation
	//	return: 0 if creation failed
	//	return: -1 if invalid params or already initialized
	a3ret a3textureCreateFromData(a3_Texture *texture_out, const a3byte name_opt[32], const a3_TexturePixelFormatDescriptor *pixelFormat, const a3ui32 width, const a3ui32 height, const void *data_opt, a3boolean dataFlipped);

	// A3: Replace part of a texture with new data.
	//	param texture: non-null pointer to initialized texture
	//	params offsetWidth, offsetHeight: positive data start offset in image
	//	params replaceWidth, replaceHeight: positive dimensions of the new data
	//	param data_opt: optional pointer to replacement data; pass null to 
	//		erase current data in that part of the image
	//	param dataFlipped: flag that says whether the data passed is flipped
	//	return: 1 if successful
	//	return: 0 if failed
	//	return: -1 if invalid params or not initialized
	a3ret a3textureReplaceData(const a3_Texture *texture, const a3ui32 offsetWidth, const a3ui32 offsetHeight, const a3ui32 replaceWidth, const a3ui32 replaceHeight, const void *data_opt, a3boolean dataFlipped);

	// A3: Use a texture for rendering.
	//	param texture: pointer to initialized texture; pass null or 
	//		uninitialized texture to deactivate
	//	param unit: texture unit or "slot" to bind texture to
	//	return: 1 if activated
	//	return: 0 if deactivated
	a3ret a3textureActivate(const a3_Texture *texture, const a3_TextureUnit unit);

	// A3: Deactivate texture on specified slot.
	//	param unit: texture unit to unbind texture from
	//	return: 0
	a3ret a3textureDeactivate(const a3_TextureUnit unit);

	// A3: Reference texture.
	//	param texture: non-null pointer to initialized texture
	//	return: new reference count if success
	//	return: -1 if invalid params or not initialized
	a3ret a3textureReference(a3_Texture *texture);

	// A3: Release texture.
	//	param texture: non-null pointer to initialized texture
	//	return: non-negative if success
	//	return: 0 if object was permanently released; descriptor is reset
	//	return: -1 if invalid params or not initialized
	a3ret a3textureRelease(a3_Texture *texture);

	// A3: Update texture release function (hotload quick-fix).
	//	param texture: non-null pointer to texture to update
	//	return: 1 if success
	//	return: -1 if invalid params
	a3ret a3textureHandleUpdateReleaseCallback(a3_Texture *texture);

	
//-----------------------------------------------------------------------------

	// A3: Change settings for active texture to default.
	//	return: 0
	a3ret a3textureDefaultSettings();

	// A3: Change filter settings for active texture.
	//	param filterOption: which filter mode to use
	//	return: 1
	a3ret a3textureChangeFilterMode(const a3_TextureFilterOption filterOption);

	// A3: Change repeat settings for active texture.
	//	param repeatOptionHoriz: which repeat mode to use for horiz repeat
	//	param repeatOptionVert: which repeat mode to use for vert repeat
	//	return: 1
	a3ret a3textureChangeRepeatMode(const a3_TextureRepeatOption repeatOptionHoriz, const a3_TextureRepeatOption repeatOptionVert);

	
//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_Texture.inl"


#endif	// !__ANIMAL3D_TEXTURE_H