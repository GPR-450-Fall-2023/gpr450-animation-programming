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
	
	a3_TextureAtlas.h
	Descriptors and utilities for texture atlas. A classic application: 2D 
		sprite-based animation.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_TEXTUREATLAS_H
#define __ANIMAL3D_TEXTUREATLAS_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"
#include "animal3D/a3utility/a3_Stream.h"
#include "animal3D-A3DG/a3graphics/a3_Texture.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_TextureAtlas				a3_TextureAtlas;
	typedef struct a3_TextureAtlasCell			a3_TextureAtlasCell;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Texture atlas cell, a single frame in a texture atlas.
	//	member relativeOffset: x, y position of frame within texture atlas
	//	member relativeSize: width and height of frame within texture atlas
	//	member pixelOffset: pixel coordinate of frame in image
	//	member pixelSize: pixel dimensions of frame in image
	//	member localOffset: if cell should have an offset position in local 
	//		space when drawn, this can be used to change cell's position
	struct a3_TextureAtlasCell
	{
		a3f32 relativeOffset[2];
		a3f32 relativeSize[2];
		a3i32 pixelOffset[2];
		a3i32 pixelSize[2];
		a3i32 localOffset[2];
	};

	// A3: Texture atlas, an image divided up into cells.
	//	member texture: pointer to 2D texture/image used for texture atlas
	//	member cells: array of cells that divide the image (null if unused)
	//	member numCells: number of cells in the array (zero if unused)
	struct a3_TextureAtlas
	{
		a3_Texture *texture;
		a3_TextureAtlasCell *cells;
		a3ui32 numCells;
	};


//-----------------------------------------------------------------------------

	// A3: Set texture for atlas.
	//	param textureAtlas: non-null pointer to texture atlas to set texture for
	//	param texture: pointer to texture; pass null or uninitialized to 
	//		unset current texture and remove reference; existing texture will 
	//		be dereferenced and replaced with new
	//	return: 1 if new texture set
	//	return: 0 if texture unset
	//	return: -1 if invalid param
	a3ret a3textureAtlasSetTexture(a3_TextureAtlas *textureAtlas, a3_Texture *texture);

	// A3: Allocate a number of uninitialized cells.
	//	param textureAtlas: non-null pointer to uninitialized texture atlas
	//	param numCells: non-zero number of cells to allocate
	//	return: numCells if success
	//	return: -1 if invalid params
	a3ret a3textureAtlasAllocateCells(a3_TextureAtlas *textureAtlas, const a3ui32 numCells);

	// A3: Load cell data from ASCII file.
	//	param textureAtlas: non-null pointer to uninitialized texture atlas
	//	param filePath: non-null, non-empty cstring of ASCII format file path
	//	return: number of cells loaded if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3textureAtlasLoadCells(a3_TextureAtlas *textureAtlas, const a3byte *filePath);

	// A3: Allocate evenly divided cells; texture must be set.  Cells are 
	//		ordered by-row from the bottom-left corner: bottom-left has 
	//		index 0, bottom-right has index (rows) - 1, top-left has index 
	//		(columns)(rows - 1), and top-right has index (columns)(rows) - 1; 
	//		additionally, cells will not have any offsets.
	//	param textureAtlas: non-null pointer to uninitialized texture atlas
	//	param columns: non-zero number of horizontal divisions
	//	param rows: non-zero number of vertical divisions
	//	return: number of resulting cells if success
	//	return: -1 if invalid params
	a3ret a3textureAtlasAllocateEvenCells(a3_TextureAtlas *textureAtlas, const a3ui32 columns, const a3ui32 rows);

	// A3: Set info for a single cell; texture must be set.
	//	param textureAtlas: non-null pointer to initialized texture atlas
	//	param index: index of cell to set; must be less than count in sheet
	//	params pixelSizeW, pixelSizeH: size of cell (width and height) in 
	//		texture, measured in pixels
	//	params pixelOffsetX, pixelOffsetY: offset of bottom-left corner of 
	//		cell in the texture, measured in pixels
	//	params localOffsetX, localOffsetY: offset in local space of sprite, 
	//		measured in pixels
	a3ret a3textureAtlasSetCell(const a3_TextureAtlas *textureAtlas, const a3ui32 index, const a3i32 pixelOffsetX, const a3i32 pixelOffsetY, const a3i32 pixelSizeW, const a3i32 pixelSizeH, const a3i32 localOffsetX, const a3i32 localOffsetY);

	// A3: Send cell data to active shader program as individual uniforms.
	//	param textureAtlas: non-null pointer to initialized texture atlas
	//	param index: index of cell to send; must be less than count in sheet
	//	param relativeOffsetLocation: integer location in program of relative 
	//		offset 2D float vector; pass -1 to ignore
	//	param relativeSizeLocation: integer location in program of relative 
	//		size 2D float vector; pass -1 to ignore
	//	param pixelOffsetLocation: integer location in program of pixel offset 
	//		2D integer vector; pass -1 to ignore
	//	param pixelSizeLocation: integer location in program of pixel size 
	//		2D integer vector; pass -1 to ignore
	//	param localOffsetLocation: integer location in program of local offset 
	//		2D integer vector; pass -1 to ignore
	//	return: number of values sent if success
	//	return: 0 if failed (no valid locations provided)
	//	return: -1 if invalid params
	a3ret a3textureAtlasSendToShaderProgram(const a3_TextureAtlas *textureAtlas, const a3ui32 index, const a3i32 relativeOffsetLocation, const a3i32 relativeSizeLocation, const a3i32 pixelOffsetLocation, const a3i32 pixelSizeLocation, const a3i32 localOffsetLocation);

	// A3: Save sprite cells to binary file.
	//	param textureAtlas: non-null pointer to uninitialized texture atlas
	//	param fileStream: non-null pointer to file stream opened in write mode
	//	return: number of bytes written if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3textureAtlasSaveDataBinary(const a3_TextureAtlas *textureAtlas, const a3_FileStream *fileStream);

	// A3: Load sprite cells from binary file.
	//	param textureAtlas: non-null pointer to uninitialized texture atlas
	//	param fileStream: non-null pointer to file stream opened in read mode
	//	return: number of bytes read if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3textureAtlasLoadDataBinary(a3_TextureAtlas *textureAtlas, const a3_FileStream *fileStream);

	// A3: Copy sprite cells to string.
	//	param textureAtlas: non-null pointer to uninitialized texture atlas
	//	param str: non-null string to copy to
	//	return: number of bytes written if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3textureAtlasCopyDataToString(const a3_TextureAtlas *textureAtlas, a3byte *str);

	// A3: Read sprite cells from string.
	//	param textureAtlas: non-null pointer to uninitialized texture atlas
	//	param str: non-null string to copy from
	//	return: number of bytes read if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3textureAtlasCopyDataFromString(a3_TextureAtlas *textureAtlas, const a3byte *str);

	// A3: Get stream size for texture atlas cells.
	//	param textureAtlas: non-null pointer to initialized texture atlas
	//	return: number of bytes required for a string to store sprite cells
	//	return: -1 if failed
	a3ret a3textureAtlasGetStringSize(const a3_TextureAtlas *textureAtlas);

	// A3: Release texture atlas: remove texture reference and release cells.
	//	param textureAtlas: non-null pointer to initialized texture atlas
	//	return: 1 if success
	//	return: -1 if invalid param
	a3ret a3textureAtlasRelease(a3_TextureAtlas *textureAtlas);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_TEXTUREATLAS_H