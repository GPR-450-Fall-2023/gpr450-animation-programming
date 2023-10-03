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
	
	a3_SpatialPose.c
	Implementation of spatial pose.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_DemoMode1_Animation-idle-update.c
	Initialization and setting of spatial pose data
*/

#include "../a3_SpatialPose.h"


//-----------------------------------------------------------------------------

a3i32 a3spatialPoseInit(a3_SpatialPose* spatialPose, a3real3p translation, a3real3p rotation, a3real3p scale)
{
	if (spatialPose)
	{
		//Set translation, default if null
		if (translation)
		{
			spatialPose->translation[0] = translation[0];
			spatialPose->translation[1] = translation[1];
			spatialPose->translation[2] = translation[2];
		}
		else
		{
			spatialPose->translation[0] = 0;
			spatialPose->translation[1] = 0;
			spatialPose->translation[2] = 0;
		}

		//Set rotation, default if null
		if (rotation)
		{
			spatialPose->rotation[0] = rotation[0];
			spatialPose->rotation[1] = rotation[1];
			spatialPose->rotation[2] = rotation[2];
		}
		else
		{
			spatialPose->rotation[0] = 0;
			spatialPose->rotation[1] = 0;
			spatialPose->rotation[2] = 0;
		}

		//Set scale, default if null
		if (scale)
		{
			spatialPose->scale[0] = scale[0];
			spatialPose->scale[1] = scale[1];
			spatialPose->scale[2] = scale[2];
		}
		else
		{
			spatialPose->scale[0] = 1;
			spatialPose->scale[1] = 1;
			spatialPose->scale[2] = 1;
		}

		//Calculate transform
		a3spatialPoseConvert(&spatialPose->transform, spatialPose,
			a3poseChannel_translate_xyz | a3poseChannel_orient_xyz | a3poseChannel_scale_xyz, a3poseEulerOrder_xyz);

		return 1;
	}
	
	return -1;
}

//-----------------------------------------------------------------------------
