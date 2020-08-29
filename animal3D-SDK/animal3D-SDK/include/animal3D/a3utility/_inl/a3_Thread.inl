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
	
	a3_Thread.inl
	Inline definitions for thread interface.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_THREAD_H
#ifndef __ANIMAL3D_THREAD_INL
#define __ANIMAL3D_THREAD_INL


//-----------------------------------------------------------------------------

// internal get ID utility
a3ret a3threadInternalGetID();

// lock
A3_INLINE a3ret a3mutexLock(a3_MutexHandle *mutex)
{
	if (mutex)
	{
		// check if not already owner
		const a3ret id = a3threadInternalGetID();
		if (mutex->threadID != id)
		{
			// wait for lock to be released, then set it
			while (mutex->threadID > 0);
			if (mutex->threadID == 0)
			{
				mutex->threadID = id;
				return 1;
			}
		}
		return 0;
	}
	return -1;
}

// unlock
A3_INLINE a3ret a3mutexUnlock(a3_MutexHandle *mutex)
{
	if (mutex)
	{
		if (mutex->threadID == a3threadInternalGetID())
		{
			mutex->threadID = 0;
			return 1;
		}
		return 0;
	}
	return -1;
}

// check if locked
A3_INLINE a3ret a3mutexIsLocked(a3_MutexHandle *mutex)
{
	if (mutex)
	{
		return (mutex->threadID != 0);
	}
	return -1;
}

// check if locked by caller
A3_INLINE a3ret a3mutexIsLockedByCaller(a3_MutexHandle *mutex)
{
	if (mutex)
	{
		return (mutex->threadID == a3threadInternalGetID());
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_THREAD_INL
#endif	// __ANIMAL3D_THREAD_H