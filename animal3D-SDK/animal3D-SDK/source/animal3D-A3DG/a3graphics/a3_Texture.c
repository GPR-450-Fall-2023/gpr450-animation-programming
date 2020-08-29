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
	
	a3_Texture.c
	Definitions for common texture object functions.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_Texture.h"

#include <string.h>


//-----------------------------------------------------------------------------
// internal utility declarations

void a3textureInternalHandleReleaseFunc(a3i32 count, a3ui32 *handlePtr);


//-----------------------------------------------------------------------------

void a3textureInternalFlipData(a3byte *dataDst, const a3byte *dataSrc, const a3ui32 rowSz, const a3ui32 totalSz)
{
	const a3byte *dataSrcPtr = dataSrc + totalSz - rowSz;
	while (dataSrcPtr >= dataSrc)
	{
		memcpy(dataDst, dataSrcPtr, rowSz);
		dataDst += rowSz;
		dataSrcPtr -= rowSz;
	}
}


a3ret a3textureHandleUpdateReleaseCallback(a3_Texture *texture)
{
	if (texture)
		return a3handleSetReleaseFunc(texture->handle, a3textureInternalHandleReleaseFunc);
	return -1;
}


//-----------------------------------------------------------------------------
