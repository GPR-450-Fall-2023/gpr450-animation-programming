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
	
	a3_HierarchyState.h
	Hierarchy transformation state.
*/

#ifndef __ANIMAL3D_HIERARCHYSTATE_H
#define __ANIMAL3D_HIERARCHYSTATE_H


// A3 hierarchy
#include "a3_Hierarchy.h"

// A3 spatial pose
#include "a3_SpatialPose.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_HierarchyPose			a3_HierarchyPose;
typedef struct a3_HierarchyTransform	a3_HierarchyTransform;
typedef struct a3_HierarchyPoseGroup	a3_HierarchyPoseGroup;
typedef struct a3_HierarchyState		a3_HierarchyState;
#endif	// __cplusplus
	

//-----------------------------------------------------------------------------

// single pose for a collection of nodes
// makes algorithms easier to keep this as a separate data type
struct a3_HierarchyPose
{
	a3_SpatialPose *spatialPose;
};


// collection of matrices for transformation set
struct a3_HierarchyTransform
{
	a3mat4 *transform;
};


// pose group
struct a3_HierarchyPoseGroup
{
	// pointer to hierarchy
	const a3_Hierarchy *hierarchy;
};


// hierarchy state structure, with a pointer to the source pose group 
//	and transformations for kinematics
struct a3_HierarchyState
{
	// pointer to pose set that the poses come from
	const a3_HierarchyPoseGroup *poseGroup;
};
	

//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount);

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup);

// get offset to hierarchy pose in contiguous set
a3i32 a3hierarchyPoseGroupGetPoseOffsetIndex(const a3_HierarchyPoseGroup *poseGroup, const a3ui32 poseIndex);

// get offset to single node pose in contiguous set
a3i32 a3hierarchyPoseGroupGetNodePoseOffsetIndex(const a3_HierarchyPoseGroup *poseGroup, const a3ui32 poseIndex, const a3ui32 nodeIndex);


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_HierarchyPoseGroup *poseGroup);

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state);

// update inverse object-space matrices
a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState *state, const a3boolean usingScale);

// update bind-to-current given bind-pose object-space transforms
a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState *state, const a3_HierarchyTransform *objectSpaceBindInverse);


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyState.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATE_H