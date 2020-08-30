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
	animal3D SDK: Minimal 3D Animation Framework Extended (A3DX)
	By Daniel S. Buckstein
	
	a3_Hierarchy.inl
	Inline definitions for hierarchy.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_A3DX_HIERARCHY_H
#ifndef __ANIMAL3D_A3DX_HIERARCHY_INL
#define __ANIMAL3D_A3DX_HIERARCHY_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3hierarchyIsParentNode(const a3_Hierarchy *hierarchy, const a3ui32 parentIndex, const a3ui32 otherIndex)
{
	if (hierarchy && hierarchy->nodes && otherIndex < hierarchy->numNodes && parentIndex < hierarchy->numNodes)
		return (hierarchy->nodes[otherIndex].parentIndex == parentIndex);
	return -1;
}

A3_INLINE a3ret a3hierarchyIsChildNode(const a3_Hierarchy *hierarchy, const a3ui32 childIndex, const a3ui32 otherIndex)
{
	return a3hierarchyIsParentNode(hierarchy, otherIndex, childIndex);
}

A3_INLINE a3ret a3hierarchyIsSiblingNode(const a3_Hierarchy *hierarchy, const a3ui32 siblingIndex, const a3ui32 otherIndex)
{
	if (hierarchy && hierarchy->nodes && otherIndex < hierarchy->numNodes && siblingIndex < hierarchy->numNodes)
		return (hierarchy->nodes[otherIndex].parentIndex == hierarchy->nodes[siblingIndex].parentIndex);
	return -1;
}

A3_INLINE a3ret a3hierarchyIsAncestorNode(const a3_Hierarchy *hierarchy, const a3ui32 ancestorIndex, const a3ui32 otherIndex)
{
	a3ui32 i = otherIndex;
	if (hierarchy && hierarchy->nodes && otherIndex < hierarchy->numNodes && ancestorIndex < hierarchy->numNodes)
	{
		while (i > ancestorIndex)
			i = hierarchy->nodes[i].parentIndex;
		return (i == ancestorIndex);
	}
	return -1;
}

A3_INLINE a3ret a3hierarchyIsDescendantNode(const a3_Hierarchy *hierarchy, const a3ui32 descendantIndex, const a3ui32 otherIndex)
{
	return a3hierarchyIsAncestorNode(hierarchy, otherIndex, descendantIndex);
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_A3DX_HIERARCHY_INL
#endif	// __ANIMAL3D_A3DX_HIERARCHY_H