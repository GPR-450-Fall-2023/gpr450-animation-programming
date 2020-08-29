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
	
	a3_Thread.h
	Basic thread starter interface.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_THREAD_H
#define __ANIMAL3D_THREAD_H


#include "animal3D/a3/a3types_integer.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Thread		a3_Thread;
	typedef struct a3_MutexHandle	a3_MutexHandle;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Thread function alias; any function declared with this format may 
	//	be used to launch a thread: 
	//	-> returns integer value
	//	-> single pointer parameter (strict)
	typedef a3ret(*a3_threadfunc)(void *);


//-----------------------------------------------------------------------------

	// A3: Simple structure for thread descriptor.
	//	member threadID: thread ID number that appears in debugger; non-zero 
	//		when initialized; resets to zero when thread terminates
	//	member result: value returned when thread exits
	//	member inThreadFunc: flag that is raised immediately before threadFunc 
	//		is called and lowered immediately after
	//	member handle: internal thread handle(s), not platform-specific
	//	member threadName: optional name of thread, renamed on launch
	//	member threadArgs: pointer to pass to threadFunc on launch
	//	member threadFunc: function to launch when thread starts
	struct a3_Thread
	{
		void *handle[2];
		a3ui32 threadID;
		a3ret result;
		a3boolean inThreadFunc;
		a3_threadfunc threadFunc;
		void *threadArgs;
		a3byte *threadName;
	};


	// A3: Simple descriptor for mutual exclusion (mutex) handle.
	//	member threadID: ID of thread that is holding the handle
	struct a3_MutexHandle
	{
		a3ui32 threadID;
	};


//-----------------------------------------------------------------------------

	// A3: Launch thread.
	//	param thread_out: pointer to unused thread descriptor
	//	param func: non-null pointer to function to launch when thread starts
	//	param args_opt: optional pointer to function arguments
	//	param name_opt: optional cstring to rename thread; default name if null
	//	return: 1 if success (thread started)
	//	return: 0 if fail (already initialized)
	//	return: -1 if invalid params (null pointers)
	a3ret a3threadLaunch(a3_Thread *thread_out, a3_threadfunc func, void *args_opt, a3byte *name_opt);

	// A3: Terminate a running thread immediately.
	//	param thread: container of thread to terminate
	//	return: 1 if success (thread terminated)
	//	return: 0 if fail
	//	return: -1 if invalid params (null pointers)
	a3ret a3threadTerminate(a3_Thread *thread);

	// A3: Wait indefinitely for a running thread to finish.
	//	param thread: container of thread to wait for
	//	return: 1 if success (thread ended)
	//	return: 0 if fail
	//	return: -1 if invalid params (null pointers)
	a3ret a3threadWait(a3_Thread *thread);

	// A3: Check if a thread is still running.
	//	param thread: container of thread to check
	//	return: thread status (1 for running, 0 for not running)
	//	return: -1 if invalid params (null pointers)
	a3ret a3threadIsRunning(const a3_Thread *thread);


//-----------------------------------------------------------------------------

	// A3: Lock mutex handle; will wait forever until handle can be locked.
	//	param mutex: non-null pointer to handle
	//	return: 1 if locked
	//	return: 0 if owner (already locked)
	//	return: -1 if invalid param
	a3ret a3mutexLock(a3_MutexHandle *mutex);

	// A3: Unlock mutex handle if calling thread is holding it.
	//	param mutex: non-null pointer to handle
	//	return: 1 if unlocked
	//	return: 0 if not owner (cannot unlock)
	//	return: -1 if invalid param
	a3ret a3mutexUnlock(a3_MutexHandle *mutex);

	// A3: Check if mutex handle is locked.
	//	param mutex: non-null pointer to handle
	//	return: 1 if locked
	//	return: 0 if unlocked
	//	return: -1 if invalid param
	a3ret a3mutexIsLocked(a3_MutexHandle *mutex);

	// A3: Check if mutex handle is locked by calling thread.
	//	param mutex: non-null pointer to handle
	//	return: 1 if locked by calling thread
	//	return: 0 if not locked by calling thread
	//	return: -1 if invalid param
	a3ret a3mutexIsLockedByCaller(a3_MutexHandle *mutex);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_Thread.inl"


#endif	// __ANIMAL3D_THREAD_H