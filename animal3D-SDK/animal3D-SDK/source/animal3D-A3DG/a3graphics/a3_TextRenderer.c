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
	
	a3_TextRenderer.c
	Basic text rendering implementation, common functions.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_TextRenderer.h"


//-----------------------------------------------------------------------------
// internal utility declarations

a3ret a3textInternalInitialize(a3_TextRenderer *handle_out, const a3ui32 fontSize, const a3boolean isBold, const a3boolean isItalic, const a3boolean isUnderline, const a3boolean isStrikethru);
a3ret a3textInternalRelease(a3_TextRenderer *handle);


//-----------------------------------------------------------------------------

// init
a3ret a3textInitialize(a3_TextRenderer *handle_out, const a3ui32 fontSize, const a3boolean isBold, const a3boolean isItalic, const a3boolean isUnderline, const a3boolean isStrikethru)
{
	if (handle_out)
	{
		if (!handle_out->base)
		{
			// need to initialize and release or it does not reload for some reason
			a3textInternalInitialize(handle_out, fontSize, 0, 0, 0, 0);
			a3textInternalRelease(handle_out);
			return a3textInternalInitialize(handle_out, fontSize, isBold, isItalic, isUnderline, isStrikethru);
		}
		return 0;
	}
	return -1;
}

// release
a3ret a3textRelease(a3_TextRenderer *handle)
{
	if (handle)
	{
		if (handle->base)
		{
			// release
			return a3textInternalRelease(handle);
		}
		return 0;
	}
	return -1;
}

// check
a3ret a3textIsInitialized(const a3_TextRenderer *handle)
{
	if (handle)
		return (handle->base != 0);
	return -1;
}


//-----------------------------------------------------------------------------
