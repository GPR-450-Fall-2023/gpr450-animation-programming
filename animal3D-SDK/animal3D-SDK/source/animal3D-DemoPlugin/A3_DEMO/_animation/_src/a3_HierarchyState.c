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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;

		// allocate everything (one malloc)
		poseGroup_out->hpose = (a3_HierarchyPose*)malloc(memreq);
		poseGroup_out->hpose->pose = poseGroup_out->pose = (a3_SpatialPose*)(poseGroup_out->hpose + hposeCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)(poseGroup_out->pose + sposeCount);
		poseGroup_out->order = (a3_SpatialPoseEulerOrder*)(poseGroup_out->channel + nodeCount);

		// set pointers
		for (i = 1; i < hposeCount; ++i)
			poseGroup_out->hpose[i].pose = poseGroup_out->hpose[i - 1].pose + nodeCount;

		// reset all data
		a3hierarchyPoseReset(poseGroup_out->hpose, sposeCount);
		memset(poseGroup_out->channel, a3poseChannel_none, channelSpace);
		memset(poseGroup_out->order, a3poseEulerOrder_xyz, orderSpace);
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hposeCount = hposeCount;
		poseGroup_out->poseCount = sposeCount;

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup->hpose);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hpose = 0;
		poseGroup->pose = 0;
		poseGroup->channel = 0;
		poseGroup->order = 0;

		// done
		return 1;
	}
	return -1;
}

// load binary
a3i32 a3hierarchyPoseGroupLoadBinary(a3_HierarchyPoseGroup* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	a3ui32 dataSize = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && !poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
				ret += (a3ui32)fread(&poseGroup->hposeCount, 1, sizeof(a3ui32), fp);
				if (ret)
				{
					a3ui32 const nodeCount = poseGroup->hierarchy->numNodes;
					a3ui32 const hposeCount = poseGroup->hposeCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
					a3ui32 const sposeCount = poseGroup->poseCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
					a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
					a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
					a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
					a3index i;

					// allocate everything (one malloc)
					poseGroup->hpose = (a3_HierarchyPose*)malloc(memreq);
					poseGroup->hpose->pose = poseGroup->pose = (a3_SpatialPose*)(poseGroup->hpose + hposeCount);
					poseGroup->channel = (a3_SpatialPoseChannel*)(poseGroup->pose + sposeCount);
					poseGroup->order = (a3_SpatialPoseEulerOrder*)(poseGroup->channel + nodeCount);
					ret += (a3ui32)fread(poseGroup->pose, 1, sposeSpace, fp);
					ret += (a3ui32)fread(poseGroup->channel, 1, channelSpace, fp);
					ret += (a3ui32)fread(poseGroup->order, 1, orderSpace, fp);

					// set pointers
					for (i = 1; i < hposeCount; ++i)
						poseGroup->hpose[i].pose = poseGroup->hpose[i - 1].pose + nodeCount;
				}
			}
			return ret;
		}
	}
	return -1;
}

// save binary
a3i32 a3hierarchyPoseGroupSaveBinary(a3_HierarchyPoseGroup const* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
				a3ui32 const nodeCount = poseGroup->hierarchy->numNodes, hposeCount = poseGroup->hposeCount;
				a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
				a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
				a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;

				ret += (a3ui32)fwrite(&poseGroup->hposeCount, 1, sizeof(a3ui32), fp);
				ret += (a3ui32)fwrite(poseGroup->pose, 1, sposeSpace, fp);
				ret += (a3ui32)fwrite(poseGroup->channel, 1, channelSpace, fp);
				ret += (a3ui32)fwrite(poseGroup->order, 1, orderSpace, fp);
			}
			return ret;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = sizeof(state_out->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 const sposeCount = hposeCount * nodeCount;
		a3ui32 const memreq = sizeof(a3_SpatialPose) * sposeCount;
		a3index i;

		// allocate everything (one malloc)
		state_out->hpose->pose = (a3_SpatialPose*)malloc(memreq);

		// set pointers
		state_out->hierarchy = hierarchy;
		for (i = 1; i < hposeCount; ++i)
			state_out->hpose[i].pose = state_out->hpose[i - 1].pose + nodeCount;

		// reset all data
		a3hierarchyPoseReset(state_out->hpose, sposeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		// release everything (one free)
		free(state->hpose->pose);

		// reset pointers
		state->hierarchy = 0;
		state->animPose->pose = 0;
		state->localSpace->pose = 0;
		state->objectSpace->pose = 0;
		state->objectSpaceInv->pose = 0;
		state->objectSpaceBindToCurrent->pose = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
		FILE* fp = fopen(resourceFilePath, "r");
		if (fp)
		{
			enum a3_FileSectionHTR
			{
				htr_file,
				htr_header,
				htr_hierarchy,
				htr_basepose,
				htr_nodepose,
				htr_eof
			};

			// known section strings
			const a3byte sectionStr[][32] = {
				"",
				"[Header]",
				"[SegmentNames&Hierarchy]",
				"[BasePosition]",
				"",
				"[EndOfFile]"
			};

			// known section components
			const a3byte headerComponents[][32] = {
				"FileType",
				"DataType",
				"FileVersion",
				"NumSegments",
				"NumFrames",
				"DataFrameRate",
				"EulerRotationOrder",
				"CalibrationUnits",
				"RotationUnits",
				"GlobalAxisofGravity",
				"BoneLengthAxis",
				"ScaleFactor",
			};
			enum a3_FileHeaderComponentHTR
			{
				htr_FileType,
				htr_DataType,
				htr_FileVersion,
				htr_NumSegments,
				htr_NumFrames,
				htr_DataFrameRate,
				htr_EulerRotationOrder,
				htr_CalibrationUnits,
				htr_RotationUnits,
				htr_GlobalAxisofGravity,
				htr_BoneLengthAxis,
				htr_ScaleFactor,
			};
			a3i32 header[sizeof(headerComponents) / sizeof(*headerComponents)] = { 0 };
			a3i32 sectionComponents[] = {
				htr_eof, // header + hierarchy + basepose + nodepose + eof
				sizeof(header) / sizeof(*header),	// fixed
				0,	// add segments after header
				0,	// add segments after header
				0,	// add segments after header
				1,	// must hit eof
			};

			// helper info
			a3_SpatialPose* pose;
			a3byte line[256], token[2][32];
			a3f32 translation[3], angles[3], scale[1], globalScale = 1.0f, angleConvert = 1.0f;
			a3i32 n, i, section = htr_file, components[sizeof(sectionComponents) / sizeof(*sectionComponents)] = { 0 }, nodePoses[256] = { 0 };
			a3size l;
			const a3ui32 linesz = sizeof(line), tokensz = sizeof(*token), tokenct = sizeof(token) / tokensz;
			
			// read line by line and parse each one
			while (fgets(line, linesz, fp))
			{
				// line introduces a new section
				if (*line == '[')
				{
					// remove end bracket
					l = strlen(line);
					line[l - 1] = 0;

					// read the header
					if (strcmp(line, sectionStr[htr_header]) == 0)
						section = htr_header;
					// read the hierarchy
					else if (strcmp(line, sectionStr[htr_hierarchy]) == 0)
						section = htr_hierarchy;
					// read the base pose
					else if (strcmp(line, sectionStr[htr_basepose]) == 0)
						section = htr_basepose;
					// kill it with fire
					else if (strcmp(line, sectionStr[htr_eof]) == 0)
					{
						// hit eof
						section = htr_eof;
						++components[section];
						if (components[section] >= sectionComponents[section])
						{
							section = htr_file;
							++components[section];
						}
					}
					// read frame pose
					else
					{
						// determine which node, ignoring brackets
						line[l - 2] = 0;
						n = a3hierarchyGetNodeIndex(hierarchy_out, line + 1);
						if ((n >= 0) && (nodePoses[n] < header[htr_NumFrames]))
							section = htr_nodepose;
					}
				}
				// line is not a comment
				else if (*line != '#')
				{
					// check if section is incomplete
					if (components[section] < sectionComponents[section])
					{
						// parse based on current section
						switch (section)
						{
						case htr_header:
							// tokenize line
							l = strlen(line);
							line[l - 1] = 0;
							sscanf(line, "%s %s", token[0], token[1]);

							// read components
							n = -1;
							while (n < sectionComponents[htr_header])
							{
								if (strcmp(token[0], headerComponents[++n]) == 0)
								{
									// read value
									switch (n)
									{
									case htr_FileType:
									case htr_DataType:
										header[n] = *(a3ui32*)(token[1]);
										break;
									case htr_FileVersion:
									case htr_NumSegments:
									case htr_NumFrames:
									case htr_DataFrameRate:
										sscanf(token[1], "%u", header + n);
										break;
									case htr_GlobalAxisofGravity:
									case htr_BoneLengthAxis:
										i = *token[1];
										header[n] = i - (i < '`' ? 'X' : 'x');
										break;
									case htr_EulerRotationOrder:
										if (*token[1] == 'X' || *token[1] == 'x')
											header[n] = (token[1][1] == 'Y' || token[1][1] == 'y') ? a3poseEulerOrder_xyz : a3poseEulerOrder_xzy;
										else if (*token[1] == 'Y' || *token[1] == 'y')
											header[n] = (token[1][1] == 'Z' || token[1][1] == 'z') ? a3poseEulerOrder_yzx : a3poseEulerOrder_yxz;
										else // Z or z
											header[n] = (token[1][1] == 'X' || token[1][1] == 'x') ? a3poseEulerOrder_zxy : a3poseEulerOrder_zyx;
										break;
									case htr_CalibrationUnits:
										if (*token[1] == 'M' || *token[1] == 'm')
											header[n] = (token[1][1]) ? 1000 : 1;
										else if (*token[1] == 'C' || *token[1] == 'c')
											header[n] = 100;
										else // ???
											header[n] = 1;
										break;
									case htr_RotationUnits:
										header[n] = (*token[1] == 'R' || *token[1] == 'r');
										angleConvert = header[n] ? a3real_deg2rad : a3real_one;
										break;
									case htr_ScaleFactor:
										sscanf(token[1], "%f", &globalScale);
										*(a3f32*)(header + n) = globalScale;
										break;
									}

									// check if header was completed
									++components[section];
									if (components[section] >= sectionComponents[section])
									{
										// increment file component counters
										sectionComponents[htr_hierarchy] = header[htr_NumSegments];
										sectionComponents[htr_basepose] = header[htr_NumSegments];
										sectionComponents[htr_nodepose] = header[htr_NumSegments];

										// allocate data: nodes and poses + 1 (base)
										a3hierarchyCreate(hierarchy_out, header[htr_NumSegments], 0);
										a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, header[htr_NumFrames] + 1);

										// global scale defaulting to cm, get ratio with file unit
										globalScale *= 100.0f / (a3f32)header[htr_CalibrationUnits];

										// done
										section = htr_file;
										++components[section];
									}
									break;
								}
							}
							break;

						case htr_hierarchy:
							// tokenize line
							l = strlen(line);
							line[l - 1] = 0;
							sscanf(line, "%s %s", token[0], token[1]);

							// first token is node name
							i = components[section];
							strncpy(hierarchy_out->nodes[i].name, token[0], a3node_nameSize);

							// second token is parent name
							hierarchy_out->nodes[i].parentIndex = strcmp(token[1], "GLOBAL")
								? ((n = a3hierarchyGetNodeIndex(hierarchy_out, token[1])) >= 0
									? n
									: -1)
								: -1;

							// set index and continue
							hierarchy_out->nodes[i].index = i;

							// check if hierarchy is completed
							++components[section];
							if (components[section] >= sectionComponents[section])
							{
								section = htr_file;
								++components[section];
							}
							break;

						case htr_basepose:
							// tokenize line
							l = strlen(line);
							line[l - 1] = 0;
							sscanf(line, "%s %f %f %f %f %f %f %f", token[0],
								translation + 0, translation + 1, translation + 2,
								angles + 0, angles + 1, angles + 2,
								scale);

							n = a3hierarchyGetNodeIndex(hierarchy_out, token[0]);
							if (n >= 0)
							{
								pose = poseGroup_out->hpose->pose + n;
								a3spatialPoseSetTranslation(pose, translation[0] * globalScale, translation[1] * globalScale, translation[2] * globalScale);
								a3spatialPoseSetRotation(pose, angles[0] * angleConvert, angles[1] * angleConvert, angles[2] * angleConvert);
								//pose->scale.w = *scale; // not actually the case
							}

							// check if base pose is done
							++components[section];
							if (components[section] >= sectionComponents[section])
							{
								section = htr_file;
								++components[section];
							}
							break;

						case htr_nodepose:
							// tokenize line
							l = strlen(line);
							line[l - 1] = 0;
							sscanf(line, "%d %f %f %f %f %f %f %f", &i,
								translation + 0, translation + 1, translation + 2,
								angles + 0, angles + 1, angles + 2,
								scale);

							// store channel data
							pose = poseGroup_out->hpose[nodePoses[n] + 1].pose + n;
							a3spatialPoseSetTranslation(pose, translation[0] * globalScale, translation[1] * globalScale, translation[2] * globalScale);
							a3spatialPoseSetRotation(pose, angles[0] * angleConvert, angles[1] * angleConvert, angles[2] * angleConvert);
							a3spatialPoseSetScale(pose, *scale, *scale, *scale);

							// set channels: if any value changes, append flag for this node
							*scale -= 1.0f;
							poseGroup_out->channel[n] |=
								(a3isNotNearZero(translation[0]) * a3poseChannel_translate_x) |
								(a3isNotNearZero(translation[1]) * a3poseChannel_translate_y) |
								(a3isNotNearZero(translation[2]) * a3poseChannel_translate_z) |
								(a3isNotNearZero(angles[0]) * a3poseChannel_rotate_x) |
								(a3isNotNearZero(angles[1]) * a3poseChannel_rotate_y) |
								(a3isNotNearZero(angles[2]) * a3poseChannel_rotate_z) |
								(a3isNotNearZero(*scale) * a3poseChannel_scale_xyz);

							// set rotation order
							poseGroup_out->order[n] = header[htr_EulerRotationOrder];

							// check if node's poses are complete
							++nodePoses[n];
							if (nodePoses[n] >= header[htr_NumFrames])
							{
								// check if all nodes' poses are complete
								++components[section];
								if (components[section] >= sectionComponents[section])
								{
									section = htr_file;
									++components[section];
								}
							}
							break;

						case htr_eof:
						case htr_file:
							// keep calm and parse on
							break;
						}
					}
					// section is done
					else
						section = htr_file;
				}
			}

			// validate counters
			i = (components[htr_file] == sectionComponents[htr_file]);
			i = (components[htr_header] == sectionComponents[htr_header]);
			i = (components[htr_hierarchy] == sectionComponents[htr_hierarchy]);
			i = (components[htr_basepose] == sectionComponents[htr_basepose]);
			i = (components[htr_nodepose] == sectionComponents[htr_nodepose]);
			i = (components[htr_eof] == sectionComponents[htr_eof]);

			// done
			fclose(fp);
			return poseGroup_out->poseCount;
		}
	}
	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}

// save HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}

// save BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}


//-----------------------------------------------------------------------------
