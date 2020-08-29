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
	
	a3_app_load.h
	Interface for demo library loading/unloading/hot build/load and demo setup.

	**DO NOT MODIFY THIS FILE**
*/


#ifndef __ANIMAL3D_APP_LOAD_H
#define __ANIMAL3D_APP_LOAD_H


#include "a3types_integer.h"


//-----------------------------------------------------------------------------
// callback types and wrappers

// **DO NOT MODIFY**
#define A3RECORDSTRSZ 32
#define A3NUMCALLBACKS 24


// A3: Callback function type aliases.
// **DO NOT MODIFY**
typedef void(*a3_voidFunc_ptr)(void *);
typedef void(*a3_voidFunc_ptrInt)(void *, a3i32);
typedef void(*a3_voidFunc_ptrIntInt)(void *, a3i32, a3i32);
typedef void(*a3_voidFunc_ptrIntIntInt)(void *, a3i32, a3i32, a3i32);
typedef void *(*a3_ptrFunc_ptrInt)(void *, a3i32);
typedef a3i32(*a3_intFunc_ptr)(void *);


// A3: Callback container structure.
// **DO NOT MODIFY THIS STRUCTURE**
typedef union tagA3_CALLBACKFUNCTIONS
{
	struct {
		a3_ptrFunc_ptrInt callback_load, callback_unload;
		a3_intFunc_ptr callback_display, callback_idle;
		a3_voidFunc_ptr callback_windowActivate, callback_windowDeactivate;
		a3_voidFunc_ptrIntInt callback_windowMove, callback_windowResize;
		a3_voidFunc_ptrInt callback_keyPress, callback_keyHold, callback_keyRelease, callback_keyCharPress, callback_keyCharHold;
		a3_voidFunc_ptrIntIntInt callback_mouseClick, callback_mouseDoubleClick, callback_mouseRelease;
		a3_voidFunc_ptrIntIntInt callback_mouseWheel;
		a3_voidFunc_ptrIntInt callback_mouseMove;
		a3_voidFunc_ptr callback_mouseLeave;
	};
	void *callback[A3NUMCALLBACKS];
} a3_CallbackFunctions;


// A3: Demo info descriptor.
// **DO NOT MODIFY THIS STRUCTURE**
typedef struct tagA3_DEMODESCRIPTOR
{
	a3byte name[A3RECORDSTRSZ];
	a3byte lib[A3RECORDSTRSZ];
	a3byte callbackHook[A3NUMCALLBACKS][A3RECORDSTRSZ];
	a3byte callbackName[A3NUMCALLBACKS][A3RECORDSTRSZ];
	a3ui32 numCallbacks;
} a3_DemoDescriptor;


// A3: Demo window info used by windowing interface.
// **DO NOT MODIFY THIS STRUCTURE**
typedef struct tagA3_DEMOWINDOWINTERFACE
{
	// callbacks
	a3_CallbackFunctions callbacks[1];

	// demo info
	a3_DemoDescriptor *records;
	a3ui32 numRecords;

	// persistent data
	a3i32 winPosX, winPosY, winSzX, winSzY;
	a3i32 id;
	void *data;
	void *handle;
} a3_DemoWindowInterface;


//-----------------------------------------------------------------------------
// functions

// A3: Set all callbacks to internally-defined dummy functions.
//	param callbacks: non-null pointer to callback container
//	return: 1 if success
//	return: -1 if invalid params
a3ret a3appSetDummyCallbacks(a3_CallbackFunctions *callbacks);


// A3: Allocate demo info structures.
//	param records_out: non-null pointer to array of demo info records
//	param count: how many instances to allocate, positive number (non-zero)
//	return: number of info records allocated
//	return: -1 if invalid params (null pointers, count is zero)
a3ret a3appAllocDemoInfo(a3_DemoDescriptor **records_out, const a3ui32 count);

// A3: Load demo information.
//	param records_out: pointer to array of demo info records to be stored
//	param filePath: path to records file
//	param numRecordsDefined: number of known records defined in the file; 
//		if zero, will read this value first and allocate space, otherwise 
//		will ignore and only read and store the data in the provided array
//	return: number of records loaded, records_out points to address if success
//	return: 0 if failed (file open failed)
//	return: -1 if invalid params (null pointers)
a3ret a3appLoadDemoInfo(a3_DemoDescriptor **records_out, const a3byte *filePath, a3ui32 numRecordsDefined);

// A3: Release demo record information.
//	param records: array of demo info records to be freed
//	return: 1 if success, records points to null pointer
//	return: -1 if invalid params (null pointers)
a3ret a3appReleaseDemoInfo(a3_DemoDescriptor **records);


// A3: Load and link callbacks from library, assign to demo interface.
//	param demoWindow: pointer to demo window interface with callback container
//	param record: pointer to descriptor
//	return: number of callbacks loaded if success
//	return: 0 if fail (library not loaded)
//	return: -1 if invalid params (null pointers, already initialized)
a3ret a3appLoadCallbacks(a3_DemoWindowInterface *demoWindow, const a3_DemoDescriptor *record);

// A3: Release and unlink callbacks.
//	param demoWindow: pointer to demo interface
//	return: 1 if success
//	return: 0 if fail (library not unloaded)
//	return: -1 if invalid params (null pointers, not initialized)
a3ret a3appReleaseCallbacks(a3_DemoWindowInterface *demoWindow);


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_APP_LOAD_H