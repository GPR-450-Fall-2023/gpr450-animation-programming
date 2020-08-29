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
	
	a3_TextRenderer.h
	Basic text rendering interface.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_TEXTRENDERER_H
#define __ANIMAL3D_TEXTRENDERER_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_TextRenderer a3_TextRenderer;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Container for text rendering handles.
	//	members handle, base: internal data used by OpenGL for text drawing
	struct a3_TextRenderer
	{
		// internal handles
		void *handle;
		void *object;
		a3ui32 base;
	};

	
//-----------------------------------------------------------------------------

	// A3: Initialize text drawing.
	//	param handle_out: uninitialized text descriptor
	//	param fontSize: the point size of the font (courier new)
	//	param isBold: use boldface text
	//	param isItalic: use italic text
	//	param isUnderline: use underlined text
	//	param isStrikethru: use strike-through text
	//	return: 1 if success
	//	return: 0 if fail (already initialized, did not initialize)
	//	return: -1 if invalid param (null pointer)
	a3ret a3textInitialize(a3_TextRenderer *handle_out, const a3ui32 fontSize, const a3boolean isBold, const a3boolean isItalic, const a3boolean isUnderline, const a3boolean isStrikethru);

	// A3: Release text drawing.
	//	param handle: text renderer object to release
	//	param handle: text 
	//	return: 1 if success
	//	return: 0 if fail (not initialized)
	//	return: -1 if invalid param (null pointer)
	a3ret a3textRelease(a3_TextRenderer *handle);

	// A3: Check if text is initialized
	//	param handle: text renderer object to check
	//	return: status of rendered (1 for initialized, 0 for not)
	//	return: -1 if invalid param (null pointer)
	a3ret a3textIsInitialized(const a3_TextRenderer *handle);


	// A3: Render text to window.
	//	param handle: text renderer object to use for drawing
	//	params x_ndc, y_ndc, z_ndc: the text's on-screen position in 
	//		normalized device coordinates (NDC), all in range [-1, 1]
	//	params r, g, b, a: 4-channel color of text, all in range [0, 1]
	//	param format: formatting string like that of 'printf', followed by 
	//		optional variadic parameter list of format arguments
	//	return: number of characters drawn
	//	return: 0 if uninitialized or no characters drawn
	//	return: -1 if invalid params (null pointers)
	a3ret a3textDraw(const a3_TextRenderer *handle,
		const a3f32 x_ndc, const a3f32 y_ndc, const a3f32 z_ndc, 
		const a3f32 r, const a3f32 g, const a3f32 b, const a3f32 a, 
		const a3byte *format, ...
	);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_TEXTRENDERER_H