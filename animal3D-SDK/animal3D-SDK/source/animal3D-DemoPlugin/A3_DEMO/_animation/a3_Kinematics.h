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
	
	a3_Kinematics.h
	Hierarchical kinematics solvers.
*/

#ifndef __ANIMAL3D_KINEMATICS_H
#define __ANIMAL3D_KINEMATICS_H


#include "a3_HierarchyState.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus


//-----------------------------------------------------------------------------

// general forward kinematics: 
// given local transforms for hierarchy nodes, calculate object-space: 
//		if not root, 
//			object-space node = object-space parent * local-space node
//		else
//			object-space node = local-space node

// forward kinematics solver given an initialized hierarchy state
a3i32 a3kinematicsSolveForward(const a3_HierarchyState *hierarchyState);

// forward kinematics solver starting at a specified joint
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount);


//-----------------------------------------------------------------------------

// general inverse kinematics: 
// given object transforms for hierarchy nodes, calculate local-space: 
//		if not root, 
//			local-space node = inverse object-space parent * object-space node
//		else
//			local-space node = object-space node

// inverse kinematics solver given an initialized hierarchy state
a3i32 a3kinematicsSolveInverse(const a3_HierarchyState *hierarchyState);

// inverse kinematics solver starting at a specified joint
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_Kinematics.inl"


#endif	// !__ANIMAL3D_KINEMATICS_H