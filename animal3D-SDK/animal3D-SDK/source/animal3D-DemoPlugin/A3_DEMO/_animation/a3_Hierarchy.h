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
	
	a3_Hierarchy.h
	Node tree structure forming a hierarchy. Nodes belonging to the same 
		hierarchy know the index of their parent node, but not child nodes.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_A3DX_HIERARCHY_H
#define __ANIMAL3D_A3DX_HIERARCHY_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3utility/a3_Stream.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_Hierarchy				a3_Hierarchy;
typedef struct a3_HierarchyNode			a3_HierarchyNode;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// A3: Node name string max length (including null terminator).
enum a3_HierarchyNodeNameSize
{
	a3node_nameSize = 32
};


// A3: Hierarchy node, a single link in a hierarchy tree.
//	member name: name of node (defaults to a3node_[index])
//	member index: index of node in hierarchy
//	member parentIndex: index of parent in hierarchy (-1 if root)
struct a3_HierarchyNode
{
	a3byte name[a3node_nameSize];
	a3i32 index;
	a3i32 parentIndex;
};


// A3: Hierarchy node container, the hierarchy itself.
//	member nodes: array of nodes (null if unused)
//	member numNodes: maximum number of nodes in hierarchy (zero if unused)
struct a3_Hierarchy
{
	a3_HierarchyNode *nodes;
	a3ui32 numNodes;
};


//-----------------------------------------------------------------------------

// A3: Allocate hierarchy with maximum node count, names optional.
//	param hierarchy_out: non-null pointer to uninitialized hierarchy
//	param numNodes: non-zero node count to initialize
//	param names_opt: optional pointer to a list of names to set immediately
//	return: numNodes if success
//	return: -1 if invalid params
a3ret a3hierarchyCreate(a3_Hierarchy *hierarchy_out, const a3ui32 numNodes, const a3byte **names_opt);

// A3: Set a hierarchy node's info; overwrites existing node at index.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param index: non-negative index of node in hierarchy
//	param parentIndex: index of parent node in hierarchy; -1 if this node 
//		is a root node; *parent index is LESS THAN node index!!!*
//	param name: name of node
//	return: index if success
//	return: -1 if invalid params
a3ret a3hierarchySetNode(const a3_Hierarchy *hierarchy, const a3ui32 index, const a3i32 parentIndex, const a3byte name[a3node_nameSize]);

// A3: Get node index by name.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param name: name to search for in hierarchy
//	return: index if success
//	return: -1 if invalid params or node not found
a3ret a3hierarchyGetNodeIndex(const a3_Hierarchy *hierarchy, const a3byte name[a3node_nameSize]);

// A3: Utility to get all node names.
//	param nameList_out: non-null pointer to pre-allocated cstring pointers 
//		to hold node names; list should have at least as many pointers as 
//		there are nodes in the hierarchy
//	param hierarchy: non-null pointer to initialized hierarchy
//	return: number of nodes if success
//	return: -1 if invalid params
a3ret a3hierarchyGetNodeNames(const a3byte *nameList_out[], const a3_Hierarchy *hierarchy);

// A3: Utility to prefix all node names with some fixed string.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param prefix: string to prepend to each node's name
//	return: index if success
//	return: -1 if invalid params or node not found
a3i32 a3hierarchyPrefixNodeNames(a3_Hierarchy const* hierarchy, a3byte const prefix[a3node_nameSize]);

// A3: Check if node is a parent of another.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param parentIndex: non-negative possible parent node index
//	param otherIndex: non-negative index of node to check for relationship
//	return: boolean, 1 if the node is a parent of the other; 0 if not
//	return: -1 if invalid params
a3ret a3hierarchyIsParentNode(const a3_Hierarchy *hierarchy, const a3ui32 parentIndex, const a3ui32 otherIndex);

// A3: Check if a node is a child of another.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param childIndex: non-negative possible child node index
//	param otherIndex: non-negative index of node to check for relationship
//	return: boolean, 1 if the node is a child of the other; 0 if not
//	return: -1 if invalid params
a3ret a3hierarchyIsChildNode(const a3_Hierarchy *hierarchy, const a3ui32 childIndex, const a3ui32 otherIndex);

// A3: Check if a node is a sibling of another.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param siblingIndex: non-negative possible sibling node index
//	param otherIndex: non-negative index of node to check for relationship
//	return: boolean, 1 if the node is a parent of the other; 0 if not
//	return: -1 if invalid params
a3ret a3hierarchyIsSiblingNode(const a3_Hierarchy *hierarchy, const a3ui32 siblingIndex, const a3ui32 otherIndex);

// A3: Check if a node is an ancestor of another.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param ancestorIndex: non-negative possible ancestor node index
//	param otherIndex: non-negative index of node to check for relationship
//	return: boolean, 1 if the node is a parent of the other; 0 if not
//	return: -1 if invalid params
a3ret a3hierarchyIsAncestorNode(const a3_Hierarchy *hierarchy, const a3ui32 ancestorIndex, const a3ui32 otherIndex);

// A3: Check if a node is a descendant of another.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param descendantIndex: non-negative possible descendant node index
//	param otherIndex: non-negative index of node to check for relationship
//	return: boolean, 1 if the node is a parent of the other; 0 if not
//	return: -1 if invalid params
a3ret a3hierarchyIsDescendantNode(const a3_Hierarchy *hierarchy, const a3ui32 descendantIndex, const a3ui32 otherIndex);

// A3: Save hierarchy to binary file.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param fileStream: non-null pointer to file stream opened in write mode
//	return: number of bytes written if success
//	return: 0 if failed
//	return: -1 if invalid params
a3ret a3hierarchySaveBinary(const a3_Hierarchy *hierarchy, const a3_FileStream *fileStream);

// A3: Load hierarchy from binary file.
//	param hierarchy: non-null pointer to unused hierarchy
//	param fileStream: non-null pointer to file stream opened in read mode
//	return: number of bytes read if success
//	return: 0 if failed
//	return: -1 if invalid params
a3ret a3hierarchyLoadBinary(a3_Hierarchy *hierarchy, const a3_FileStream *fileStream);

// A3: Store hierarchy in string.
//	param hierarchy: non-null pointer to initialized hierarchy
//	param str: non-null byte array to stream into
//	return: number of bytes copied if success
//	return: 0 if failed
//	return: -1 if invalid params
a3ret a3hierarchyCopyToString(const a3_Hierarchy *hierarchy, a3byte *str);

// A3: Read hierarchy from string.
//	param hierarchy: non-null pointer to unused hierarchy
//	param str: non-null byte array to stream from
//	return: number of bytes copied if success
//	return: 0 if failed
//	return: -1 if invalid params
a3ret a3hierarchyCopyFromString(a3_Hierarchy *hierarchy, const a3byte *str);

// A3: Get hierarchy stream size.
//	param hierarchy: non-null pointer to initialized hierarchy
//	return: number of bytes required to store hierarchy in string
//	return: -1 if invalid param
a3ret a3hierarchyGetStringSize(const a3_Hierarchy *hierarchy);

// A3: Release hierarchy.
//	param hierarchy: non-null pointer to initialized hierarchy
//	return: 1 if success
//	return: -1 if invalid param
a3ret a3hierarchyRelease(a3_Hierarchy *hierarchy);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_Hierarchy.inl"


#endif	// !__ANIMAL3D_A3DX_HIERARCHY_H