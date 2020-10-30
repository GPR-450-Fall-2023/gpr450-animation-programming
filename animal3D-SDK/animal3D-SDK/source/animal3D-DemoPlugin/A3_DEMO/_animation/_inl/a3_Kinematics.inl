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
	
	a3_Kinematics.inl
	Implementation of kinematics solvers.
*/


#ifdef __ANIMAL3D_KINEMATICS_H
#ifndef __ANIMAL3D_KINEMATICS_INL
#define __ANIMAL3D_KINEMATICS_INL


//-----------------------------------------------------------------------------

// partial FK solver
inline a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// ****TO-DO: implement forward kinematics algorithm
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- object matrix = parent object matrix * local matrix
		//		- else
		//			- copy local matrix to object matrix
		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3real4x4Product(hierarchyState->objectSpace->pose[itr->index].transformMat.m,
					hierarchyState->objectSpace->pose[itr->parentIndex].transformMat.m,
					hierarchyState->localSpace->pose[itr->index].transformMat.m);
			else
				hierarchyState->objectSpace->pose[itr->index] = hierarchyState->localSpace->pose[itr->index];
		}
		return (a3i32)(end - itr);
	}
	return -1;
}


//-----------------------------------------------------------------------------

// partial IK solver
inline a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// ****TO-DO: implement inverse kinematics algorithm
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- local matrix = inverse parent object matrix * object matrix
		//		- else
		//			- copy object matrix to local matrix
		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3real4x4Product(hierarchyState->localSpace->pose[itr->index].transformMat.m,
					hierarchyState->objectSpaceInv->pose[itr->parentIndex].transformMat.m,
					hierarchyState->objectSpace->pose[itr->index].transformMat.m);
			else
				hierarchyState->localSpace->pose[itr->index] = hierarchyState->objectSpace->pose[itr->index];
		}
		return (a3i32)(end - itr);
	}
	return -1;
}


//-----------------------------------------------------------------------------

// FK solver
inline a3i32 a3kinematicsSolveForward(const a3_HierarchyState *hierarchyState)
{
	if (hierarchyState && hierarchyState->hierarchy)
		return a3kinematicsSolveForwardPartial(hierarchyState, 0, hierarchyState->hierarchy->numNodes);
	return -1;
}


//-----------------------------------------------------------------------------

// IK solver
inline a3i32 a3kinematicsSolveInverse(const a3_HierarchyState *hierarchyState)
{
	if (hierarchyState && hierarchyState->hierarchy)
		return a3kinematicsSolveInversePartial(hierarchyState, 0, hierarchyState->hierarchy->numNodes);
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KINEMATICS_INL
#endif	// __ANIMAL3D_KINEMATICS_H