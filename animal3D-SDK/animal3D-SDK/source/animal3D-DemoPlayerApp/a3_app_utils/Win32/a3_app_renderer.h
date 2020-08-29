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
	
	a3_app_renderer.h
	Interface for render context management.

	**DO NOT MODIFY THIS FILE**
*/

#if (defined _WINDOWS || defined _WIN32)
#ifndef __ANIMAL3D_APP_RENDERER_H
#define __ANIMAL3D_APP_RENDERER_H


#include "a3types_integer.h"

#include <Windows.h>


//-----------------------------------------------------------------------------
// structures & types

// A3: Renderer-agnostic stand-in rendering context type.
// **DO NOT MODIFY**
typedef void *				a3_RenderingContext;
typedef WNDCLASSEXA			a3_WindowClass;


// A3: User print function with the following layout: 
//	param format: format string of variadic arguments
//	return: common integer type
typedef a3ret(*a3_PrintFunc)(const a3byte *const format, ...);


//-----------------------------------------------------------------------------

// A3: Create default rendering context.
//	param context_out: pointer to context to be created
//	param windowClass: window class used to describe render window
//	return: 1 if success (created context)
//	return: 0 if fail (context not created)
//	return: -1 if invalid params (null pointers)
a3ret a3rendererCreateDefaultContext(a3_RenderingContext *context_out, a3_WindowClass *windowClass);

// A3: Allow two rendering contexts to share resources. This must be done 
//		before starting to actually use either context to create GL resources.
//	param contextA: pointer to first context
//	param contextB: pointer to second context
//	return: 1 if success
//	return: 0 if failed
//	return: -1 if invalid params (null pointers)
a3ret a3rendererShareContext(a3_RenderingContext *contextA, a3_RenderingContext *contextB);

// A3: Release rendering context properly.
//	param context: pointer to context to be released
//	return: 1 if success (released context)
//	return: 0 if fail (context not released)
//	return: -1 if invalid params (null pointers)
a3ret a3rendererReleaseContext(a3_RenderingContext *context);

// A3: Display information about renderer.
//	param context: pointer to render context
//	return: 1 if success
//	return: 0 if failed
//	return: -1 if invalid params (null pointers)
a3ret a3rendererDisplayInfo(a3_RenderingContext *context);

// A3: Display information about renderer with user-specified print function.
//	param context: pointer to render context
//	param printFunc: user-specified function pointer for printing
//	return: 1 if success
//	return: 0 if failed
//	return: -1 if invalid params (null pointers)
a3ret a3rendererDisplayInfoPrintFunc(a3_RenderingContext *context, a3_PrintFunc printFunc);


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_APP_RENDERER_H
#endif	// (defined _WINDOWS || defined _WIN32)