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
	
	a3_DemoState_idle-update.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     UPDATING THE STATE in this file.         ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------
// GENERAL UTILITIES

void a3demo_applyScale_internal(a3_DemoSceneObject *sceneObject, a3real4x4p s)
{
	if (sceneObject->scaleMode)
	{
		if (sceneObject->scaleMode == 1)
		{
			s[0][0] = s[1][1] = s[2][2] = sceneObject->scale.x;
			a3real4x4ConcatL(sceneObject->modelMat.m, s);
			a3real4x4TransformInverseUniformScale(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
		}
		else
		{
			s[0][0] = sceneObject->scale.x;
			s[1][1] = sceneObject->scale.y;
			s[2][2] = sceneObject->scale.z;
			a3real4x4ConcatL(sceneObject->modelMat.m, s);
			a3real4x4TransformInverse(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
		}
	}
	else
		a3real4x4TransformInverseIgnoreScale(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
}


//-----------------------------------------------------------------------------
// UPDATE SUB-ROUTINES

void a3demo_update_objects(a3f64 const dt, a3_DemoSceneObject* sceneObjectBase,
	a3ui32 count, a3boolean useZYX, a3boolean applyScale)
{
	a3ui32 i;
	a3mat4 scaleMat = a3mat4_identity;

	if (applyScale)
		for (i = 0; i < count; ++i, ++sceneObjectBase)
		{
			// update transforms
			a3demo_updateSceneObject(sceneObjectBase, useZYX);

			// apply scale
			a3demo_applyScale_internal(sceneObjectBase, scaleMat.m);
		}
	else
		for (i = 0; i < count; ++i, ++sceneObjectBase)
		{
			a3demo_updateSceneObject(sceneObjectBase, useZYX);
		}
}

void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis)
{
	const a3f32 dr = demoState->updateAnimation ? (a3f32)dt * 15.0f : 0.0f;
	a3ui32 i;

	// do simple animation
	for (i = 0, axis %= 3; i < count; ++i, ++sceneObjectBase)
		sceneObjectBase->euler.v[axis] = a3trigValid_sind(sceneObjectBase->euler.v[axis] + dr);
}

void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox)
{
	// model transformations (if needed)
	const a3mat4 convertY2Z = {
		+1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, +1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, +1.0f,
	};

	// bind skybox to camera
	obj_skybox->position = obj_camera->position;
	obj_skybox->modelMat.v3 = obj_camera->modelMat.v3;

	// correct rotations as needed
	// need to rotate skybox if Z-up
	a3real4x4ConcatL(obj_skybox->modelMat.m, convertY2Z.m);
}

void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count)
{
	a3ui32 i;

	// update lights view positions for current camera
	for (i = 0; i < count; ++i, ++pointLightBase)
	{
		// convert to view space and retrieve view position
		a3real4Real4x4Product(pointLightBase->viewPos.v, obj_camera->modelMatInv.m, pointLightBase->worldPos.v);
	}
}


//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update(a3_DemoState *demoState, a3f64 const dt)
{
	demoState->demoModeCallbacksPtr->handleUpdate(demoState,
		demoState->demoModeCallbacksPtr->demoMode, dt);
}


//-----------------------------------------------------------------------------
