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
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../a3_Kinematics.h"


//-----------------------------------------------------------------------------

// partial FK solver
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
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

		for (a3ui32 i = firstIndex; i < firstIndex + nodeCount; i++)
		{
			a3i32 parent = hierarchyState->hierarchy->nodes[i].parentIndex;
			if (parent != -1)
			{
				//Copy parent object matrix so we don't overwrite it
				a3real4x4 parentObject;
				a3real4x4SetReal4x4(parentObject, (a3real4*)&hierarchyState->objectSpace->sPoses[parent].transform);

				//object = parentObject * local
				hierarchyState->objectSpace->sPoses[i].transform = *(a3mat4*)a3real4x4MulTransform(
					parentObject,
					(a3real4*)&hierarchyState->localSpace->sPoses[i].transform
				);
			}
			else
			{
				a3hierarchyPoseCopy(hierarchyState->objectSpace, hierarchyState->localSpace, nodeCount);
			}
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// partial IK solver
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
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
	}
	return -1;
}


//-----------------------------------------------------------------------------
