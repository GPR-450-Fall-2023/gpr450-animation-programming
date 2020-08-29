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
	
	a3_app_window.h
	Interface for window creation and event loop.

	**DO NOT MODIFY THIS FILE**
*/

#if (defined _WINDOWS || defined _WIN32)
#ifndef __ANIMAL3D_APP_WINDOW_H
#define __ANIMAL3D_APP_WINDOW_H


#include "a3_app_renderer.h"
#include "a3_app_utils/a3_app_load.h"

#include <Windows.h>


//-----------------------------------------------------------------------------
// structures

// A3: Reusable liases for existing structures.
// **DO NOT MODIFY**
typedef HANDLE				a3_ThreadHandle;
typedef HWND				a3_WindowHandle;
typedef HDC					a3_DeviceContext;
typedef HMENU				a3_WindowMenuHandle;
typedef TRACKMOUSEEVENT		a3_MouseTracker[1];


// A3: Platform environment variables.
//**DO NOT MODIFY THIS STRUCTURE**
typedef struct tagA3_PLATFORMENVIRONMENT
{
	const a3byte *launchPathStr;
	const a3byte *slnPathStr;
	const a3byte *sdkDirStr;
	const a3byte *cfgDirStr;
} a3_PlatformEnvironment;


// A3: Window interface.
// **DO NOT MODIFY THIS STRUCTURE**
typedef struct tagA3_WINDOWINTERFACE
{
	// platform-specifics
	a3_WindowHandle windowHandle;
	a3_DeviceContext deviceContext;
	a3_RenderingContext renderingContext;
	const a3_PlatformEnvironment *env;

	// flags
	a3i32 windowID;
	a3boolean rcAlways;
	a3boolean isStandalone;

	// platform-specific demo features
	a3_WindowMenuHandle windowMenuHandle, fileMenuHandle;
	a3_MouseTracker mouseTracker;

	// shared
	a3_DemoWindowInterface demo[1];
} a3_WindowInterface;


//-----------------------------------------------------------------------------
// functions

// A3: Initialize platform environment variable structure.
//	param env_out: pointer to environment structure to set
//	param launchPathStr: non-null path to VS launcher (devenv)
//	param slnPathStr: non-null path to solution
//	param sdkDirStr: non-null path to A3 SDK directory
//	param cfgDirStr: non-null relative path for build config
//	return: 1 if success (set all variables)
//	return: -1 if invalid params (null pointers or strings)
a3ret a3windowInitPlatformEnvironment(a3_PlatformEnvironment* env_out, const a3byte* launchPathStr, const a3byte* slnPathStr, const a3byte* sdkDirStr, const a3byte* cfgDirStr);

// A3: Create window class for application rendering window.
//	param windowClass_out: pointer to window class object to be created
//	param appInstance: handle to application
//	param windowClassName: name of application's window class
//	param extraBytes: extra space to reserve
//	param iconID: resource ID for a taskbar icon; pass -1 to ignore
//	param smallIconID: resource ID for small window icon; pass -1 to ignore
//	return: 1 if success (window class created)
//	return: 0 if fail (window class not created)
//	return: -1 if invalid params (null pointers)
a3ret a3windowCreateDefaultRenderingClass(a3_WindowClass *windowClass_out, HINSTANCE appInstance, const a3byte *windowClassName, const a3ui32 extraBytes, const a3i32 iconID, const a3i32 smallIconID);

// A3: Release application window class.
//	param windowClass: pointer to window class object to be released
//	param appInstance: handle to application
//	return: 1 if success (window class released)
//	return: 0 if fail (window class not released)
//	return: -1 if invalid params (null pointers)
a3ret a3windowReleaseClass(a3_WindowClass *windowClass, HINSTANCE appInstance);

// A3: Create window.
//	param windowHandle_out: pointer to hold resulting window structure
//	param windowClass: pointer to window class used to describe window
//	param env: pointer to platform environment variables
//	param renderContext_opt: optional pointer to rendering context to use for 
//		this window
//	param windowName: name of window (appears at the top of the window)
//	params winWidth, winHeight: the size of the window in pixels (not 
//		including border & decorations)
//	param alwaysRender: if rendering, the context will remain active when 
//		the window loses focus (e.g. clicked on another window)
//	param standalone: create standalone, single-demo window without menu
//	return: 1 if success (window created)
//	return: 0 if fail (not created)
//	return: -1 if invalid params (null pointers)
a3ret a3windowCreate(a3_WindowInterface *window_out, a3_WindowClass *windowClass, a3_PlatformEnvironment *env, a3_RenderingContext *renderContext_opt, const a3byte *windowName, const a3ui32 winWidth, const a3ui32 winHeight, const a3boolean alwaysRender, const a3boolean standalone);

// A3: Release window.
//	param windowHandle: handle of window to be released
//	return: 1 if success (window released)
//	return: 0 if fail (not released)
//	return: -1 if invalid params (null pointer)
a3ret a3windowRelease(a3_WindowInterface *window);

// A3: Begin processing window messages.
//	param window: pointer to window object to begin processing messages for
//	return: return code from window when it exits
a3ret a3windowBeginMainLoop(a3_WindowInterface *window);

// A3: Begin processing window messages on a new thread.
//	param window: pointer to window object to begin processing messages for
//	param handle: pointer to thread handle
//	return: thread ID if successful
//	return: 0 if thread handle already used
//	return: -1 if invalid params (null pointers)
a3ret a3windowBeginMainLoopThreaded(a3_WindowInterface *window, a3_ThreadHandle *thread);

// A3: Get return value from thread.
//	param handle: pointer to thread handle.
//	return: return value from thread (do not return -1)
//	return: -1 if invalid param or thread is still running
a3ret a3windowGetThreadReturnValue(a3_ThreadHandle *handle);


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_APP_WINDOW_H
#endif	// (defined _WINDOWS || defined _WIN32)