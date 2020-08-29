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
	
	a3_app_renderer-OpenGL.c
	Implementation for OpenGL context creation for window.

	**DO NOT MODIFY THIS FILE**
*/

#if (defined _WINDOWS || defined _WIN32)

#include "animal3D/a3/a3types_integer.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/wglew.h>


//-----------------------------------------------------------------------------
// internal types

typedef HGLRC				a3_RenderingContext;
typedef WNDCLASSEXA			a3_WindowClass;
typedef a3ret(*a3_PrintFunc)(const a3byte *const format, ...);


//-----------------------------------------------------------------------------
// internal rendering context functions

// choose pixel format for window
a3i32 a3rendererInternalChooseDefaultPixelFormat(const a3i32 flag, const HDC dc)
{
	// describe built-in pixel format
	PIXELFORMATDESCRIPTOR pfd[1] = { 0 };
	pfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd->nVersion = 1;					// version
	pfd->dwFlags =
		PFD_DRAW_TO_WINDOW |			// window drawing
		PFD_SUPPORT_OPENGL |			// using opengl
		PFD_DOUBLEBUFFER |				// double buffer support
		PFD_GENERIC_ACCELERATED;		// hardware acceleration
	pfd->iPixelType = PFD_TYPE_RGBA;	// RGBA colour mode
	pfd->cColorBits = 32;				// **32 bit colour buffer
	pfd->cDepthBits = 24;				// **24 bit depth buffer
	pfd->cStencilBits = 8;				// **8 bit stencil buffer
	pfd->iLayerType = PFD_MAIN_PLANE;

	// result
	a3i32 pixelFormat = 0;
	a3i32 numFormats = 0;
	a3i32 result = 0;

	// if platform repeat flag is raised, then the default format 
	//	has been previously used; select new pixel format using 
	//	WGL to get access to new features
	if (flag > 0)
	{
		// WGL pixel format descriptor
		const a3i32 pixelAttribs[] = {
			// standard pixel format settings
			WGL_DRAW_TO_WINDOW_ARB,				GL_TRUE, 
			WGL_SUPPORT_OPENGL_ARB,				GL_TRUE, 
			WGL_DOUBLE_BUFFER_ARB,				GL_TRUE, 
			WGL_ACCELERATION_ARB,				WGL_FULL_ACCELERATION_ARB, 
			WGL_PIXEL_TYPE_ARB,					WGL_TYPE_RGBA_ARB, 
			WGL_COLOR_BITS_ARB,					040, 
			WGL_DEPTH_BITS_ARB,					030, 
			WGL_STENCIL_BITS_ARB,				010, 

			// multi-sampling (MSAA) settings
			WGL_SAMPLE_BUFFERS_ARB,				GL_FALSE, 
			WGL_SAMPLES_ARB,					000, 

			// done
			0
		};
		
		// get pixel format function and use it to select pixel format
		// this is a good way to validate the address of the function
		PFNWGLCHOOSEPIXELFORMATARBPROC a3rendererChoosePixelFormat = wglChoosePixelFormatARB;
		result = a3rendererChoosePixelFormat(dc, pixelAttribs, 0, 1, &pixelFormat, &numFormats);
	}

	// otherwise, use built-in default descriptor
	else
	{
		pixelFormat = ChoosePixelFormat(dc, pfd);
	}

	// after pixel format is selected, assign it to device context
	if (pixelFormat)
		result = SetPixelFormat(dc, pixelFormat, pfd);

	// done
	return result;
}

// rendering context initialize extensions
a3ret a3rendererInternalInitializeExtensions()
{
	static a3i32 initialized = 0;
	if (!initialized)
	{
		// modern GPUs will launch with extensions... 
		//	and initialize glew just to be safe and check
		if (glewInit() == GLEW_OK)
		{
			// done
			initialized = 1;
		}
		return initialized;
	}
	return -1;
}


//-----------------------------------------------------------------------------
// internal context management functions

// check if context is current
a3boolean a3rendererInternalContextIsCurrent(const a3_RenderingContext renderingContext)
{
	return (renderingContext == wglGetCurrentContext());
}

// set current context
void a3rendererInternalSetContext(HDC deviceContext, a3_RenderingContext renderingContext)
{
	wglMakeCurrent(deviceContext, renderingContext);
}

// set vertical sync
void a3rendererInternalSetVsync(a3i32 interval)
{
	wglSwapIntervalEXT(interval);
}


//-----------------------------------------------------------------------------
// application-usable functions

// print info about renderer using custom function
a3ret a3rendererDisplayInfoPrintFunc(a3_RenderingContext *context, a3_PrintFunc printFunc)
{
	if (context && *context)
	{
		// get strings
		const a3ubyte *versionStr = glGetString(GL_VERSION);
		const a3ubyte *shadingStr = glGetString(GL_SHADING_LANGUAGE_VERSION);
		const a3ubyte *rendererStr = glGetString(GL_RENDERER);
		const a3ubyte *vendorStr = glGetString(GL_VENDOR);
		const a3boolean status = (versionStr != 0);

		// display strings
		printFunc("\n----------------------------------------------------------------------------");
		printFunc("\n----------------------------------------------------------------------------\n");
		if (status)
			printFunc("\n GL version:   %s\n GLSL version: %s\n GPU renderer: %s\n GPU vendor:   %s\n", versionStr, shadingStr, rendererStr, vendorStr);
		else
			printFunc("\n GL: Unable to determine info; ensure context is active and valid.\n");
		printFunc("\n----------------------------------------------------------------------------");
		printFunc("\n----------------------------------------------------------------------------\n");
		printFunc("\n");

		// done
		return status;
	}
	return -1;
}

// print info about renderer to console
a3ret a3rendererDisplayInfo(a3_RenderingContext *context)
{
	return a3rendererDisplayInfoPrintFunc(context, printf);
}

// rendering context release
a3ret a3rendererReleaseContext(a3_RenderingContext *context)
{
	if (context && *context)
	{
		wglMakeCurrent(0, 0);
		wglDeleteContext(*context);
		*context = 0;
		return 1;
	}
	return -1;
}

// rendering context share
a3ret a3rendererShareContext(a3_RenderingContext *contextA, a3_RenderingContext *contextB)
{
	if (contextA && contextB && *contextA && *contextB)
	{
		wglShareLists(*contextA, *contextB);
		return 1;
	}
	return -1;
}

// rendering context create
a3ret a3rendererCreateDefaultContext(a3_RenderingContext *context_out, a3_WindowClass *windowClass)
{
	if (context_out && !*context_out && windowClass && windowClass->hInstance)
	{
		// **DO NOT MODIFY THIS STRUCTURE**
		typedef struct tagA3RENDERINGPLATFORMWGL
		{
			HWND hwnd;
			HDC dc;
			HGLRC rc;
			a3i32 flag;
		} a3_RenderingPlatformWGL;

		// dummy context for checking pixel type and actual context to use
		a3_RenderingPlatformWGL falseContext = { 0 }, trueContext = { 0 };
		a3i32 result = 0;

		// create a dummy window only to create a render context
		CreateWindowA(windowClass->lpszClassName,
			0, 0, 0, 0, 0, 0, 0, 0, windowClass->hInstance,
			&falseContext);

		// check if pixel format was selected for dummy window
		result = falseContext.flag;
		if (result)
		{
			// create and activate OpenGL rendering context
			falseContext.rc = wglCreateContext(falseContext.dc);
			result = wglMakeCurrent(falseContext.dc, falseContext.rc);
			if (result)
			{
				// now, a "false" render context has been created 
				//	and the platform descriptor has its repeat flag 
				//	raised; create another dummy window to initialize 
				//	the actual context
				trueContext.flag = 1;

				// initialize extensions
				a3rendererInternalInitializeExtensions();

				// create window again
				CreateWindowA(windowClass->lpszClassName,
					0, 0, 0, 0, 0, 0, 0, 0, windowClass->hInstance,
					&trueContext);

				// check if new pixel format was selected
				result = trueContext.flag;
				if (result)
				{
					// create and configure final context
					trueContext.rc = wglCreateContext(trueContext.dc);
					result = wglMakeCurrent(trueContext.dc, trueContext.rc);
					if (result)
					{
						// move reference to RC so it doesn't 
						//	get deleted with the dummy window
						*context_out = trueContext.rc;
						trueContext.rc = 0;

						// release false context (also disables context)
						a3rendererReleaseContext(&falseContext.rc);
					}
				}
			}
		}

		// kill dummy windows
		DestroyWindow(falseContext.hwnd);
		DestroyWindow(trueContext.hwnd);

		// done
		return result;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// (defined _WINDOWS || defined _WIN32)