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
	
	a3_TextRenderer-OpenGL.c
	Basic text rendering implementation, OpenGL-specific.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_TextRenderer.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if (defined _WINDOWS || defined _WIN32)
#include <Windows.h>
#endif	// (defined _WINDOWS || defined _WIN32)

#include <GL/gl.h>


//-----------------------------------------------------------------------------

a3ret a3textInternalInitialize(a3_TextRenderer *handle_out, const a3ui32 fontSize, const a3boolean isBold, const a3boolean isItalic, const a3boolean isUnderline, const a3boolean isStrikethru)
{
	a3i32 ret = 0;

#if (defined _WINDOWS || defined _WIN32)
	HFONT tmpFont, textFont;
	HDC deviceContext = GetDC(GetActiveWindow());
	if (deviceContext)
	{
		handle_out->handle = deviceContext;
		handle_out->base = glGenLists(96);

		// create font
		tmpFont = CreateFontA(-(a3i32)fontSize, 0, 0, 0,
			(isBold ? FW_BOLD : FW_NORMAL), (isItalic ? TRUE : FALSE),
			(isUnderline ? TRUE : FALSE), (isStrikethru ? TRUE : FALSE),
			ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			(FF_DONTCARE | DEFAULT_PITCH), "Courier New");

		// select font and map to GL characters
		handle_out->object = textFont = (HFONT)SelectObject(deviceContext, tmpFont);
		wglUseFontBitmaps(deviceContext, 32, 96, handle_out->base);
		SelectObject(deviceContext, textFont);
		DeleteObject(tmpFont);
		ret = 1;
	}
#endif	// (defined _WINDOWS || defined _WIN32)

	return ret;
}

a3ret a3textInternalRelease(a3_TextRenderer *handle)
{
	a3i32 ret = 0;

#if (defined _WINDOWS || defined _WIN32)
	glDeleteLists(handle->base, 96);
	DeleteObject((HFONT)handle->object);
	handle->handle = 0;
	handle->object = 0;
	handle->base = 0;
	ret = 1;
#endif	// (defined _WINDOWS || defined _WIN32)

	return ret;
}


//-----------------------------------------------------------------------------

// draw text
a3ret a3textDraw(const a3_TextRenderer *handle,
	const a3f32 x_ndc, const a3f32 y_ndc, const a3f32 z_ndc,
	const a3f32 r, const a3f32 g, const a3f32 b, const a3f32 a,
	const a3byte *format, ...
)
{
	static a3ubyte text[128] = { 0 };

	// if text and valid handle provided, render text
	if (handle && handle->base && format)
	{
		a3i32 ret;
		va_list va;
		va_start(va, format);
		ret = _vsnprintf(text, 128, format, va);
		va_end(va);

		glColor4f(r, g, b, a);
		glRasterPos3f(x_ndc, y_ndc, z_ndc);
		glPushAttrib(GL_LIST_BIT);
		glListBase(handle->base - 32);
		glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
		*text = 0;

		// done
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------
