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
	
	a3_app_application.h
	Interface for some application management utilities.

	**DO NOT MODIFY THIS FILE**
*/

#if (defined _WINDOWS || defined _WIN32)
#ifndef __ANIMAL3D_APP_APPLICATION_H
#define __ANIMAL3D_APP_APPLICATION_H


#include "a3types_integer.h"

#include <Windows.h>


//-----------------------------------------------------------------------------

// A3: Using this function at the beginning of winmain will prevent multiple 
// instances of the application from running by automatically switching to 
// the original instance.
//	param windowClassName: name of application's window class
//	param windowName: title of window created
//	return: 1 if success (initial instance created)
//	return: -1 if invalid params (no valid pointer)
a3ret a3appStartSingleInstanceSwitchExisting(const a3byte *windowClassName, const a3byte *windowName);

// A3: Same as above, but does not switch to original instance.
//	param handle_out: pointer to handle to mutex object used to lock the window resource
//	param instanceName: name of application instance
//	return: 1 if success (initial instance created)
//	return: 0 if fail (already initialized)
//	return: -1 if invalid params (null pointers)
a3ret a3appStartSingleInstance(HANDLE *handle_out, const a3byte *instanceName);

// A3: Similar to above with multiple instances allowed.
//	param handle_out: pointer to handle to semaphor object used to track resources
//	param instanceName: name of application instance
//	param limit: number of instances allowed
//	return: 1 if success (instance created)
//	return: 0 if fail (already initialized)
//	return: -1 if invalid params (null pointers)
a3ret a3appStartMultipleInstance(HANDLE *handle_out, const a3byte *instanceName, const a3ui32 limit);

// A3: Stop single instance using mutex.
//	param handle: pointer to mutex handle acquired from starting single instance
//	return: 1 if success (handle released)
//	return: 0 if fail (not initialized)
//	return: -1 if invalid params
a3ret a3appStopSingleInstance(HANDLE *handle);

// A3: Stop instance using semaphor.
//	param handle: pointer to semaphor handle acquired from starting first instance
//	return: 1 if success (handle released)
//	return: 0 if fail (not initialized)
//	return: -1 if invalid params
a3ret a3appStopMultipleInstance(HANDLE *handle);


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_APP_APPLICATION_H
#endif	// (defined _WINDOWS || defined _WIN32)