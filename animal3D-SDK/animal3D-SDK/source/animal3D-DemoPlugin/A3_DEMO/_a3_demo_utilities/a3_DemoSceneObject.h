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
	
	a3_DemoSceneObject.h
	Example of demo utility header file.
*/

#ifndef __ANIMAL3D_DEMOSCENEOBJECT_H
#define __ANIMAL3D_DEMOSCENEOBJECT_H


// math library
#include "animal3D-A3DM/animal3D-A3DM.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_DemoModelMatrixStack	a3_DemoModelMatrixStack;
	typedef struct a3_DemoViewerMatrixStack	a3_DemoViewerMatrixStack;
	typedef struct a3_DemoSceneObject		a3_DemoSceneObject;
	typedef struct a3_DemoProjector			a3_DemoProjector;
	typedef struct a3_DemoPointLight		a3_DemoPointLight;
#endif	// __cplusplus

	
//-----------------------------------------------------------------------------

	// matrix stack for a single object
	struct a3_DemoModelMatrixStack
	{
		a3mat4 modelMat;						// model matrix (object -> world)
		a3mat4 modelMatInverse;					// model inverse matrix (world -> object)
		a3mat4 modelMatInverseTranspose;		// model inverse-transpose matrix (object -> world skewed)
		a3mat4 modelViewMat;					// model-view matrix (object -> viewer)
		a3mat4 modelViewMatInverse;				// model-view inverse matrix (viewer -> object)
		a3mat4 modelViewMatInverseTranspose;	// model-view inverse transpose matrix (object -> viewer skewed)
		a3mat4 modelViewProjectionMat;			// model-view-projection matrix (object -> clip)
		a3mat4 atlasMat;						// atlas matrix (texture -> cell)
	};

	// matrix stack for a viewer object
	struct a3_DemoViewerMatrixStack
	{
		a3mat4 projectionMat;					// projection matrix (viewer -> clip)
		a3mat4 projectionMatInverse;			// projection inverse matrix (clip -> viewer)
		a3mat4 projectionBiasMat;				// projection-bias matrix (viewer -> biased clip)
		a3mat4 projectionBiasMatInverse;		// projection-bias inverse matrix (biased clip -> viewer)
		a3mat4 viewProjectionMat;				// view-projection matrix (world -> clip)
		a3mat4 viewProjectionMatInverse;		// view-projection inverse matrix (clip -> world)
		a3mat4 viewProjectionBiasMat;			// view projection-bias matrix (world -> biased clip)
		a3mat4 viewProjectionBiasMatInverse;	// view-projection-bias inverse matrix (biased clip -> world)
	};

	// general scene objects
	struct a3_DemoSceneObject
	{
		a3mat4 modelMat;	// model matrix: transform relative to scene
		a3mat4 modelMatInv;	// inverse model matrix: scene relative to this
		a3vec3 euler;		// euler angles for direct rotation control
		a3vec3 position;	// scene position for direct control
		a3vec3 scale;		// scale (not accounted for in update)
		a3i32 scaleMode;	// 0 = off; 1 = uniform; other = non-uniform (nightmare)
	};

	// projector/camera/viewer
	struct a3_DemoProjector
	{
		a3_DemoSceneObject *sceneObject;	// pointer to scene object
		a3mat4 projectionMat;				// projection matrix
		a3mat4 projectionMatInv;			// inverse projection matrix
		a3mat4 viewProjectionMat;			// concatenation of view-projection
		a3boolean perspective;				// perspective or orthographic
		a3real fovy;						// persp: vert field of view/ortho: vert size
		a3real aspect;						// aspect ratio
		a3real znear, zfar;					// near and far clipping planes
		a3real ctrlMoveSpeed;				// how fast controlled camera moves
		a3real ctrlRotateSpeed;				// control rotate speed (degrees)
		a3real ctrlZoomSpeed;				// control zoom speed (degrees)
	};

	// simple point light
	struct a3_DemoPointLight
	{
		a3vec4 worldPos;					// position in world space
		a3vec4 viewPos;						// position in viewer space
		a3vec4 color;						// RGB color with padding
		a3real radius;						// radius (distance of effect from center)
		a3real radiusSq;					// radius squared (if needed)
		a3real radiusInv;					// radius inverse (also used for attenuation sometimes)
		a3real radiusInvSq;					// radius inverse squared (attenuation factor)
	};


//-----------------------------------------------------------------------------

	// scene object initializers and updates
	inline void a3demo_initSceneObject(a3_DemoSceneObject *sceneObject);
	inline void a3demo_updateSceneObject(a3_DemoSceneObject *sceneObject, const a3boolean useZYX);
	inline a3i32 a3demo_rotateSceneObject(a3_DemoSceneObject *sceneObject, const a3real speed, const a3real deltaX, const a3real deltaY, const a3real deltaZ);
	inline a3i32 a3demo_moveSceneObject(a3_DemoSceneObject *sceneObject, const a3real speed, const a3real deltaX, const a3real deltaY, const a3real deltaZ);

	inline void a3demo_setProjectorSceneObject(a3_DemoProjector *projector, a3_DemoSceneObject *sceneObject);
	inline void a3demo_initProjector(a3_DemoProjector *projector);
	inline void a3demo_updateProjectorProjectionMat(a3_DemoProjector *projector);
	inline void a3demo_updateProjectorViewProjectionMat(a3_DemoProjector *projector);

	inline void a3demo_resetModelMatrixStack(a3_DemoModelMatrixStack* model);
	inline void a3demo_resetViewerMatrixStack(a3_DemoViewerMatrixStack* viewer);
	inline void a3demo_updateModelMatrixStack(a3_DemoModelMatrixStack* model, a3real4x4p const projectionMat_viewer, a3real4x4p const modelMat_viewer, a3real4x4p const modelMatInv_viewer, a3real4x4p const modelMat, a3real4x4p const atlasMat);
	inline void a3demo_updateViewerMatrixStack(a3_DemoViewerMatrixStack* viewer, a3real4x4p const modelMat_viewer, a3real4x4p const modelMatInv_viewer, a3real4x4p const projectionMat, a3real4x4p const projectionMatInv, a3real4x4p const biasMat, a3real4x4p const biasMatInv);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOSCENEOBJECT_H