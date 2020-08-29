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
	
	a3_Texture-OpenGL.c
	Definitions for OpenGL texture object (using DevIL for now).

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_Texture.h"

#include "GL/glew.h"

#include <stdlib.h>
#include <string.h>

#ifdef UNICODE
#define A3_UNICODE_UNDEF
#undef UNICODE
#endif	// UNICODE
#ifdef _UNICODE
#define _A3_UNICODE_UNDEF
#undef _UNICODE
#endif	// _UNICODE

#include "IL/ilut.h"

#ifdef A3_UNICODE_UNDEF
#define UNICODE
#undef A3_UNICODE_UNDEF
#endif	// A3_UNICODE_UNDEF
#ifdef _A3_UNICODE_UNDEF
#define _UNICODE
#undef _A3_UNICODE_UNDEF
#endif	// _A3_UNICODE_UNDEF


//-----------------------------------------------------------------------------

// flip the data and store in dst
void a3textureInternalFlipData(a3byte *dataDst, const a3byte *dataSrc, const a3ui32 rowSz, const a3ui32 totalSz);


// auto-release function
void a3textureInternalHandleReleaseFunc(a3i32 count, a3ui32 *handlePtr)
{
	glDeleteTextures(count, handlePtr);
}


//-----------------------------------------------------------------------------

a3ret a3textureCreatePixelFormatDescriptor(a3_TexturePixelFormatDescriptor *pixelFormat_out, const a3_TexturePixelType pixelType)
{
	static const a3_TexturePixelFormatDescriptor pixelFormats[] = {
		{ GL_RED,	GL_R8,		GL_UNSIGNED_BYTE,	1, 1 },
		{ GL_RED,	GL_R16,		GL_UNSIGNED_SHORT,	1, 2 },
		{ GL_RED,	GL_R32F,	GL_FLOAT,			1, 4 },
		{ GL_RG,	GL_RG8,		GL_UNSIGNED_BYTE,	2, 1 },
		{ GL_RG,	GL_RG16,	GL_UNSIGNED_SHORT,	2, 2 },
		{ GL_RG,	GL_RG32F,	GL_FLOAT,			2, 4 },
		{ GL_RGB,	GL_RGB8,	GL_UNSIGNED_BYTE,	3, 1 },
		{ GL_RGB,	GL_RGB16,	GL_UNSIGNED_SHORT,	3, 2 },
		{ GL_RGB,	GL_RGB32F,	GL_FLOAT,			3, 4 },
		{ GL_RGBA,	GL_RGBA8,	GL_UNSIGNED_BYTE,	4, 1 },
		{ GL_RGBA,	GL_RGBA16,	GL_UNSIGNED_SHORT,	4, 2 },
		{ GL_RGBA,	GL_RGBA32F,	GL_FLOAT,			4, 4 },
		{ GL_LUMINANCE,			GL_LUMINANCE8,			GL_UNSIGNED_BYTE,	1, 1 },
		{ GL_LUMINANCE,			GL_LUMINANCE16,			GL_UNSIGNED_SHORT,	1, 2 },
		{ GL_DEPTH_COMPONENT,	GL_DEPTH_COMPONENT16,	GL_UNSIGNED_SHORT,	1, 2 },
		{ GL_DEPTH_COMPONENT,	GL_DEPTH_COMPONENT24,	GL_UNSIGNED_INT,	1, 4 },
		{ GL_DEPTH_COMPONENT,	GL_DEPTH_COMPONENT32,	GL_UNSIGNED_INT,	1, 4 },
		{ GL_DEPTH_STENCIL,		GL_DEPTH24_STENCIL8,	GL_UNSIGNED_INT_24_8,	1, 4 },
	};

	if (pixelFormat_out)
	{
		*pixelFormat_out = pixelFormats[pixelType];
		return pixelType;
	}
	return -1;
}


//-----------------------------------------------------------------------------

a3ret a3textureInitializeImageLibrary()
{
	static a3i32 initialized = 0;
	if (!initialized)
	{
		ilInit();
		iluInit();
		ilutInit();
		ilutRenderer(ILUT_OPENGL);
	//	ilEnable(IL_FILE_OVERWRITE);
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		initialized = 1;
		return 1;
	}
	return 0;
}

a3ret a3textureCreateFromFile(a3_Texture *texture_out, const a3byte name_opt[32], const a3byte *filePath)
{
	a3_Texture ret = { 0 };
	if (texture_out)
	{
		if (!texture_out->handle->handle)
		{
			// multi-platform-friendly way of doing this: 
			//	no platform-specific calls
			// start by initializing image library in case it hasn't been
			a3i32 result = a3textureInitializeImageLibrary();
			a3i32 convertFormat, convertType, textureFormat, textureFormatInternal;
			a3ui32 glHandle = 0;
			a3ui32 ilHandle = 0;
			a3ui32 width, height, channels, bytes;

			// generate IL handle
			ilHandle = ilGenImage();
			if (ilHandle)
			{
				ilBindImage(ilHandle);
				result = ilLoadImage(filePath);
				if (result)
				{
					result = 0;
					width = ilGetInteger(IL_IMAGE_WIDTH);
					height = ilGetInteger(IL_IMAGE_HEIGHT);
					channels = ilGetInteger(IL_IMAGE_CHANNELS);
					bytes = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL) / channels;

					if (width && height && channels && bytes)
					{
						// determine format: want all images to be one of 4 types: 
						//	rgb8, rgb16, rgba8 or rgba16
						channels = channels >= 3 ? channels <= 4 ? channels : 4 : 3;
						bytes = bytes >= 1 ? bytes <= 2 ? bytes : 2 : 1;

						// select descriptors and convert image
						if (channels == 3)
							textureFormat = bytes == 1 ? GL_RGB8 : GL_RGB16;
						else
							textureFormat = bytes == 1 ? GL_RGBA8 : GL_RGBA16;
						convertFormat = textureFormatInternal = channels == 3 ? IL_RGB : IL_RGBA;
						convertType = bytes == 1 ? IL_UNSIGNED_BYTE : IL_UNSIGNED_SHORT;
						ilConvertImage(convertFormat, convertType);

						// create GL texture with default settings
						glGenTextures(1, &glHandle);
						if (glHandle)
						{
							glBindTexture(GL_TEXTURE_2D, glHandle);
							glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormatInternal, convertType, ilGetData());
							a3textureDefaultSettings();
							glBindTexture(GL_TEXTURE_2D, 0);

							// configure the output
							a3handleCreateHandle(ret.handle, a3textureInternalHandleReleaseFunc, name_opt, glHandle, 1);
							ret.width = width;
							ret.height = height;
							ret.channels = channels;
							ret.bytes = bytes;
							ret.internalFormat = textureFormatInternal;
							ret.internalType = convertType;

							// done
							*texture_out = ret;
							a3textureReference(texture_out);
							result = 1;
						}
					}
				}

				// delete IL image
				ilDeleteImage(ilHandle);
			}

			// done
			return result;
		}
	}
	return -1;
}

a3ret a3textureCreateFromData(a3_Texture *texture_out, const a3byte name_opt[32], const a3_TexturePixelFormatDescriptor *pixelFormat, const a3ui32 width, const a3ui32 height, const void *data_opt, a3boolean dataFlipped)
{
	a3_Texture ret = { 0 };
	a3ui32 handle;

	// validate
	if (texture_out)
	{
		// not in use
		if (!texture_out->handle->handle)
		{
			// validate size
			if (width && height)
			{
				// generate texture
				glGenTextures(1, &handle);
				if (handle)
				{
					a3byte *tmpDataPtr;

					// if data is flipped, need to re-order it row-by-row
					// do so in a temporarily-allocated image
					if (data_opt && dataFlipped)
					{
						const a3ui32 rowSz = width * pixelFormat->channelsPerPixel * pixelFormat->bytesPerChannel;
						const a3ui32 totalSz = height * rowSz;
						tmpDataPtr = (a3byte *)malloc(totalSz);
						a3textureInternalFlipData(tmpDataPtr, (const a3byte *)data_opt, rowSz, totalSz);
						data_opt = tmpDataPtr;
					}

					// bind texture and fill with data
					glBindTexture(GL_TEXTURE_2D, handle);
					glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat->internalFormatBits, width, height, 0, pixelFormat->internalFormat, pixelFormat->internalDataType, data_opt);
					a3textureDefaultSettings();
					glBindTexture(GL_TEXTURE_2D, 0);

					if (data_opt && dataFlipped)
						free(tmpDataPtr);

					// done, configure output
					a3handleCreateHandle(ret.handle, a3textureInternalHandleReleaseFunc, name_opt, handle, 1);
					ret.width = width;
					ret.height = height;
					ret.channels = pixelFormat->channelsPerPixel;
					ret.bytes = pixelFormat->bytesPerChannel;
					ret.internalFormat = pixelFormat->internalFormat;
					ret.internalType = pixelFormat->internalDataType;

					// set output
					*texture_out = ret;
					a3textureReference(texture_out);
					return 1;
				}
				else
					printf("\n A3 ERROR (TEX \'%s\'): \n\t Invalid handle; texture not created.", name_opt);
			}
			else
				printf("\n A3 ERROR (TEX \'%s\'): \n\t Invalid dimensions; texture not created.", name_opt);
			
			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3textureReplaceData(const a3_Texture *texture, const a3ui32 offsetWidth, const a3ui32 offsetHeight, const a3ui32 replaceWidth, const a3ui32 replaceHeight, const void *data_opt, a3boolean dataFlipped)
{
	if (texture)
	{
		if (texture->handle->handle)
		{
			const a3ui32 endWidth = offsetWidth + replaceWidth, endHeight = offsetHeight + replaceHeight;
			if (replaceWidth && replaceHeight && endWidth <= texture->width && endHeight <= texture->height)
			{
				// same flip algo as above
				a3byte *tmpDataPtr;
				if (data_opt && dataFlipped)
				{
					const a3ui32 rowSz = replaceWidth * texture->channels * texture->bytes;
					const a3ui32 totalSz = replaceHeight * rowSz;
					tmpDataPtr = (a3byte *)malloc(totalSz);
					a3textureInternalFlipData(tmpDataPtr, (const a3byte *)data_opt, rowSz, totalSz);
					data_opt = tmpDataPtr;
				}

				// bind texture and fill with data
				glBindTexture(GL_TEXTURE_2D, texture->handle->handle);
				glTexSubImage2D(GL_TEXTURE_2D, 0, offsetWidth, offsetHeight, replaceWidth, replaceHeight, texture->internalFormat, texture->internalType, data_opt);
				glBindTexture(GL_TEXTURE_2D, 0);

				if (data_opt && dataFlipped)
					free(tmpDataPtr);

				// done
				return 1;
			}
			else
				printf("\n A3 ERROR (TEX %u \'%s\'): \n\t Invalid params for replacing texture data.", texture->handle->handle, texture->handle->name);

			// fail
			return 0;
		}
	}
	return -1;
}

a3ret a3textureActivate(const a3_Texture *texture, const a3_TextureUnit unit)
{
	// switch unit
	glActiveTexture(GL_TEXTURE0 + unit);

	// if valid texture, activate
	if (texture && texture->handle->handle)
	{
		glBindTexture(GL_TEXTURE_2D, texture->handle->handle);
		return 1;
	}

	// deactivate
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}

a3ret a3textureDeactivate(const a3_TextureUnit unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}


//-----------------------------------------------------------------------------

a3ret a3textureDefaultSettings()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return 1;
}

a3ret a3textureChangeFilterMode(const a3_TextureFilterOption filterOption)
{
	static const a3ui16 filter[] = { GL_NEAREST, GL_LINEAR, };
	const a3ui16 option = filter[filterOption];
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, option);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, option);
	return 1;
}

a3ret a3textureChangeRepeatMode(const a3_TextureRepeatOption repeatOptionHoriz, const a3_TextureRepeatOption repeatOptionVert)
{
	static const a3ui16 repeat[] = { GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT };
	const a3ui16 optionH = repeat[repeatOptionHoriz];
	const a3ui16 optionV = repeat[repeatOptionVert];
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, optionH);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, optionV);
	return 1;
}


//-----------------------------------------------------------------------------
// removed from header

/*
	// A3: Save texture to image file.
	//	param texture: non-null pointer to initialized texture
	//	param filePath: non-null, valid cstring of file path to save to
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params or not initialized
	a3ret a3textureSaveToFile(const a3_Texture *texture, const a3byte *filePath);
*/

//-----------------------------------------------------------------------------
// removed source

/*
a3ret a3textureSaveToFile(const a3_Texture *texture, const a3byte *filePath)
{
	if (texture)
	{
		if (texture->handle->handle)
		{
			a3i32 result = a3textureInitializeImageLibrary();
			a3ui32 ilHandle = ilGenImage();
			if (ilHandle)
			{
				result = 0;

				ilBindImage(ilHandle);
				ilSetInteger(IL_IMAGE_WIDTH, texture->width);
				ilSetInteger(IL_IMAGE_HEIGHT, texture->height);
				ilSetInteger(IL_IMAGE_CHANNELS, texture->channels);
				ilSetInteger(IL_IMAGE_BYTES_PER_PIXEL, texture->bytes);

				// get data

				// save image
				result = ilSaveImage(filePath);

				ilDeleteImage(ilHandle);
			}

			// done
			return result;
		}
	}
	return -1;
}
*/
