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
	
	a3_TextureAtlas.c
	Texture atlas implementation.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_TextureAtlas.h"
#include "animal3D-A3DG/a3graphics/a3_ShaderProgram.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/glew.h"


//-----------------------------------------------------------------------------

inline void a3textureAtlasInternalSetCell(a3_TextureAtlasCell *cell, const a3f32 relativeOffsetX, const a3f32 relativeOffsetY, const a3f32 relativeSizeW, const a3f32 relativeSizeH, const a3i32 pixelOffsetX, const a3i32 pixelOffsetY, const a3i32 pixelSizeW, const a3i32 pixelSizeH, const a3i32 localOffsetX, const a3i32 localOffsetY)
{
	cell->relativeOffset[0] = relativeOffsetX;
	cell->relativeOffset[1] = relativeOffsetY;
	cell->relativeSize[0] = relativeSizeW;
	cell->relativeSize[1] = relativeSizeH;
	cell->pixelOffset[0] = pixelOffsetX;
	cell->pixelOffset[1] = pixelOffsetY;
	cell->pixelSize[0] = pixelSizeW;
	cell->pixelSize[1] = pixelSizeH;
	cell->localOffset[0] = localOffsetX;
	cell->localOffset[1] = localOffsetY;
}

inline void a3textureAtlasInternalCalculateRelativeSetCell(a3_TextureAtlasCell *cell, const a3ui32 textureWidth, const a3ui32 textureHeight, const a3i32 pixelOffsetX, const a3i32 pixelOffsetY, const a3i32 pixelSizeW, const a3i32 pixelSizeH, const a3i32 localOffsetX, const a3i32 localOffsetY)
{
	const a3f32 relativeX = ((a3f32)pixelOffsetX) / ((a3f32)textureWidth);
	const a3f32 relativeY = ((a3f32)pixelOffsetY) / ((a3f32)textureHeight);
	const a3f32 relativeW = ((a3f32)pixelSizeW) / ((a3f32)textureWidth);
	const a3f32 relativeH = ((a3f32)pixelSizeH) / ((a3f32)textureHeight);
	a3textureAtlasInternalSetCell(cell, relativeX, relativeY, relativeW, relativeH, pixelOffsetX, pixelOffsetY, pixelSizeW, pixelSizeH, localOffsetX, localOffsetY);
}


inline a3ui32 a3textureAtlasInternalGetCellCount(a3byte *line, const a3ui32 lineLen, FILE *fp)
{
	a3ui32 numCells = 0;
	a3byte *linePtr = fgets(line, lineLen, fp);
	while (!feof(fp))
	{
		// check first character
		if (*(linePtr++) == '@')
		{
			sscanf(linePtr, "%u", &numCells);
			break;
		}

		// next line
		linePtr = fgets(line, lineLen, fp);
	}
	return numCells;
}

inline a3ret a3textureAtlasInternalLoad(a3_TextureAtlas *textureAtlas, FILE *fp)
{
	const a3ui32 lineLen = 256;
	a3byte line[256], *linePtr;
	a3ui32 numCells = 0;
	a3ui32 i = 0;
	a3i32 j = 0;

	a3_TextureAtlasCell *cell;
	a3i32 offsetX, offsetY, sizeW, sizeH, localX, localY;

	// get number of cells
	numCells = a3textureAtlasInternalGetCellCount(line, lineLen, fp);

	// validate
	if (numCells)
	{
		const a3ui32 dataSize = sizeof(a3_TextureAtlasCell) * numCells;
		textureAtlas->cells = (a3_TextureAtlasCell *)malloc(dataSize);
		memset(textureAtlas->cells, 0, dataSize);
		textureAtlas->numCells = numCells;

		// read values
		i = 0;
		linePtr = fgets(line, lineLen, fp);
		while (!feof(fp) && i < numCells)
		{
			if (*(linePtr++) == '@')
			{
				j = sscanf(linePtr, "%d %d %d %d %d %d", &offsetX, &offsetY, &sizeW, &sizeH, &localX, &localY);
				cell = textureAtlas->cells + (i++);
				a3textureAtlasInternalCalculateRelativeSetCell(cell, textureAtlas->texture->width, textureAtlas->texture->height, offsetX, offsetY, sizeW, sizeH, localX, localY);
			}
			linePtr = fgets(line, lineLen, fp);
		}

		if (i != numCells)
			printf("\n A3 ERROR: Sprite cells file has incomplete data definitions.");
	}
	return numCells;
}


//-----------------------------------------------------------------------------

a3ret a3textureAtlasSetTexture(a3_TextureAtlas *textureAtlas, a3_Texture *texture)
{
	if (textureAtlas)
	{
		// release current texture
		a3textureRelease(textureAtlas->texture);
		if (texture && texture->handle->handle)
		{
			// set new texture
			textureAtlas->texture = texture;
			a3textureReference(texture);
			return 1;
		}

		// remove pointer
		textureAtlas->texture = 0;
		return 0;
	}
	return -1;
}

a3ret a3textureAtlasAllocateCells(a3_TextureAtlas *textureAtlas, const a3ui32 numCells)
{
	if (textureAtlas && numCells)
	{
		if (!textureAtlas->cells)
		{
			const a3ui32 dataSize = sizeof(a3_TextureAtlasCell) * numCells;
			textureAtlas->cells = (a3_TextureAtlasCell *)malloc(dataSize);
			memset(textureAtlas->cells, 0, dataSize);
			textureAtlas->numCells = numCells;
			return numCells;
		}
	}
	return -1;
}

a3ret a3textureAtlasLoadCells(a3_TextureAtlas *textureAtlas, const a3byte *filePath)
{
	FILE *fp;
	a3i32 ret = 0;
	if (textureAtlas && filePath)
	{
		if (!textureAtlas->cells && textureAtlas->texture && textureAtlas->texture->handle->handle && *filePath)
		{
			fp = fopen(filePath, "r");
			if (fp)
			{
				ret = a3textureAtlasInternalLoad(textureAtlas, fp);
				fclose(fp);
			}
			return ret;
		}
	}
	return -1;
}

a3ret a3textureAtlasAllocateEvenCells(a3_TextureAtlas *textureAtlas, const a3ui32 columns, const a3ui32 rows)
{
	if (textureAtlas && columns && rows)
	{
		if (!textureAtlas->cells && textureAtlas->texture && textureAtlas->texture->handle->handle)
		{
			a3ui32 r, c;
			a3_TextureAtlasCell *cell;

			const a3f32 relativeW = 1.0f / ((a3f32)columns), relativeH = 1.0f / ((a3f32)rows);
			const a3ui32 pixelW = textureAtlas->texture->width / columns, pixelH = textureAtlas->texture->height / rows;
			a3f32 relativeX, relativeY;
			a3ui32 pixelX, pixelY;

			// allocate
			textureAtlas->numCells = rows * columns;
			textureAtlas->cells = (a3_TextureAtlasCell *)malloc(sizeof(a3_TextureAtlasCell) * textureAtlas->numCells);

			// set
			for (cell = textureAtlas->cells, r = 0; r < rows; ++r)
			{
				relativeY = ((a3f32)r) / ((a3f32)rows);
				pixelY = textureAtlas->texture->height * r / rows;
				for (c = 0; c < columns; ++c, ++cell)
				{
					relativeX = ((a3f32)c) / ((a3f32)columns);
					pixelX = textureAtlas->texture->width * c / columns;
					a3textureAtlasInternalSetCell(cell, relativeX, relativeY, relativeW, relativeH, pixelX, pixelY, pixelW, pixelH, 0, 0);
				}
			}

			// done
			return (textureAtlas->numCells);
		}
	}
	return -1;
}

a3ret a3textureAtlasSetCell(const a3_TextureAtlas *textureAtlas, const a3ui32 index, const a3i32 pixelOffsetX, const a3i32 pixelOffsetY, const a3i32 pixelSizeW, const a3i32 pixelSizeH, const a3i32 localOffsetX, const a3i32 localOffsetY)
{
	if (textureAtlas)
	{
		if (textureAtlas->cells && textureAtlas->texture && textureAtlas->texture->handle->handle && index < textureAtlas->numCells)
		{
			a3_TextureAtlasCell *cell = textureAtlas->cells + index;
			a3textureAtlasInternalCalculateRelativeSetCell(cell, textureAtlas->texture->width, textureAtlas->texture->height, pixelOffsetX, pixelOffsetY, pixelSizeW, pixelSizeH, localOffsetX, localOffsetY);
			return 1;
		}
	}
	return -1;
}

a3ret a3textureAtlasSendToShaderProgram(const a3_TextureAtlas *textureAtlas, const a3ui32 index, const a3i32 relativeOffsetLocation, const a3i32 relativeSizeLocation, const a3i32 pixelOffsetLocation, const a3i32 pixelSizeLocation, const a3i32 localOffsetLocation)
{
	if (textureAtlas)
	{
		if (textureAtlas->cells && index < textureAtlas->numCells)
		{
			a3i32 ret = 0;
			const a3_TextureAtlasCell *cell = textureAtlas->cells + index;
			ret += a3shaderUniformSendFloat(a3unif_vec2, relativeOffsetLocation, 1, cell->relativeOffset) > 0;
			ret += a3shaderUniformSendFloat(a3unif_vec2, relativeSizeLocation, 1, cell->relativeSize) > 0;
			ret += a3shaderUniformSendInt(a3unif_vec2, pixelOffsetLocation, 1, cell->pixelOffset) > 0;
			ret += a3shaderUniformSendInt(a3unif_vec2, pixelSizeLocation, 1, cell->pixelSize) > 0;
			ret += a3shaderUniformSendInt(a3unif_vec2, localOffsetLocation, 1, cell->localOffset) > 0;
			return ret;
		}
	}
	return -1;
}

a3ret a3textureAtlasSaveDataBinary(const a3_TextureAtlas *textureAtlas, const a3_FileStream *fileStream)
{
	FILE *fp;
	a3ui32 ret = 0;
	if (textureAtlas && fileStream)
	{
		if (textureAtlas->cells)
		{
			fp = fileStream->stream;
			if (fp)
			{
				ret += (a3ui32)fwrite(&textureAtlas->numCells, 1, sizeof(a3ui32), fp);
				ret += (a3ui32)fwrite(textureAtlas->cells, 1, sizeof(a3_TextureAtlasCell) * textureAtlas->numCells, fp);
			}
			return ret;
		}
	}
	return -1;
}

a3ret a3textureAtlasLoadDataBinary(a3_TextureAtlas *textureAtlas, const a3_FileStream *fileStream)
{
	FILE *fp;
	a3ui32 ret = 0;
	if (textureAtlas && fileStream)
	{
		if (!textureAtlas->cells)
		{
			fp = fileStream->stream;
			if (fp)
			{
				ret += (a3ui32)fread(&textureAtlas->numCells, 1, sizeof(a3ui32), fp);

				textureAtlas->cells = (a3_TextureAtlasCell *)malloc(sizeof(a3_TextureAtlasCell) * textureAtlas->numCells);
				ret += (a3ui32)fread(textureAtlas->cells, 1, sizeof(a3_TextureAtlasCell) * textureAtlas->numCells, fp);
			}
			return ret;
		}
	}
	return -1;
}

a3ret a3textureAtlasCopyDataToString(const a3_TextureAtlas *textureAtlas, a3byte *str)
{
	const a3byte *const start = str;
	if (textureAtlas)
	{
		if (textureAtlas->cells)
		{
			// copy count
			str = (a3byte *)((a3ui32 *)memcpy(str, &textureAtlas->numCells, sizeof(a3ui32)) + 1);

			str = (a3byte *)((a3_TextureAtlasCell *)memcpy(str, textureAtlas->cells, sizeof(a3_TextureAtlasCell) * textureAtlas->numCells) + textureAtlas->numCells);

			return (a3i32)(str - start);
		}
	}
	return -1;
}

a3ret a3textureAtlasCopyDataFromString(a3_TextureAtlas *textureAtlas, const a3byte *str)
{
	const a3byte *const start = str;
	a3ui32 dataSize;
	if (textureAtlas)
	{
		if (!textureAtlas->cells)
		{
			memcpy(&textureAtlas->numCells, str, sizeof(a3ui32));
			str += sizeof(a3ui32);

			dataSize = sizeof(a3_TextureAtlasCell) * textureAtlas->numCells;
			textureAtlas->cells = (a3_TextureAtlasCell *)malloc(dataSize);
			memcpy(textureAtlas->cells, str, dataSize);
			str += dataSize;

			return (a3i32)(str - start);
		}
	}
	return -1;
}

a3ret a3textureAtlasGetStringSize(const a3_TextureAtlas *textureAtlas)
{
	if (textureAtlas)
	{
		if (textureAtlas->cells)
		{
			const a3ui32 dataSize
				= sizeof(a3ui32)
				+ sizeof(a3_TextureAtlasCell) * textureAtlas->numCells;
			return dataSize;
		}
	}
	return -1;
}

a3ret a3textureAtlasRelease(a3_TextureAtlas *textureAtlas)
{
	if (textureAtlas)
	{
		a3textureRelease(textureAtlas->texture);
		free(textureAtlas->cells);
		textureAtlas->texture = 0;
		textureAtlas->cells = 0;
		textureAtlas->numCells = 0;
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------
