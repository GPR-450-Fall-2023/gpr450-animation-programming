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
	
	a3_DemoRenderUtils.h
	Drawing utilities for demo state.
*/

#ifndef __ANIMAL3D_DEMORENDERUTILS_H
#define __ANIMAL3D_DEMORENDERUTILS_H


//-----------------------------------------------------------------------------
// animal3D framework includes

#include "animal3D/animal3D.h"
#include "animal3D-A3DG/animal3D-A3DG.h"
#include "animal3D-A3DM/animal3D-A3DM.h"


//-----------------------------------------------------------------------------
// demo includes

#include "a3_DemoShaderProgram.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
#endif	// __cplusplus

	
//-----------------------------------------------------------------------------
// general utilities

// get the inverse transpose of a matrix
inline a3real4x4r a3demo_quickInverseTranspose_internal(a3real4x4p m_out, const a3real4x4p m_in);

// invert and transpose a matrix
inline a3real4x4r a3demo_quickInvertTranspose_internal(a3real4x4p m_inout);

// get the transpose of a matrix with the bottom row set to zero
inline a3real4x4r a3demo_quickTransposedZeroBottomRow(a3real4x4p m_out, const a3real4x4p m_in);


//-----------------------------------------------------------------------------
// rendering states

// set default blending mode for composition
inline void a3demo_enableCompositeBlending();

// set default blending mode for accumulation
inline void a3demo_enableAdditiveBlending();

// set default state for writing to stencil buffer
inline void a3demo_enableStencilWrite();

// set default state for comparing with stencil buffer
inline void a3demo_enableStencilCompare();

// set default state for renderer
inline void a3demo_setDefaultGraphicsState();

// set state for scene draw
inline void a3demo_setSceneState(a3_Framebuffer const* currentWriteFBO, a3boolean displaySkybox);


//-----------------------------------------------------------------------------

inline void a3demo_drawModelSimple(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program);

inline void a3demo_drawModelSimple_activateModel(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable);

inline void a3demo_drawModelSolidColor(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable, a3real4p const color);

inline void a3demo_drawModelTexturedColored_invertModel(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3real4x4p const atlasMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable, a3_Texture const* texture, a3real4p const color);

inline void a3demo_drawModelLighting(a3real4x4p modelViewProjectionMat, a3real4x4p modelViewMat, a3real4x4p const viewProjectionMat, a3real4x4p const viewMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable, a3real4p const color);

inline void a3demo_drawModelLighting_bias_other(a3real4x4p modelViewProjectionBiasMat_other, a3real4x4p modelViewProjectionMat, a3real4x4p modelViewMat, a3real4x4p const viewProjectionBiasMat_other, a3real4x4p const viewProjectionMat, a3real4x4p const viewMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable, a3real4p const color);

inline void a3demo_drawStencilTest(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_DemoRenderUtils.inl"


#endif	// !__ANIMAL3D_DEMORENDERUTILS_H