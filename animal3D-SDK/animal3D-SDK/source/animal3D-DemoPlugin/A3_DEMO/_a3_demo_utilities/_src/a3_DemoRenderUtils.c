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
	
	a3_DemoRenderUtils.c
	Drawing utilities implementations.
*/

#include "../a3_DemoRenderUtils.h"


//-----------------------------------------------------------------------------

// OpenGL
#ifdef _WIN32
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32


//-----------------------------------------------------------------------------
// GENERAL SETUP AND STATE CHANGE UTILITIES

// blending state for composition
extern inline void a3demo_enableCompositeBlending()
{
	// result = ( new*[new alpha] ) + ( old*[1 - new alpha] )
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// blending state for accumulation
extern inline void a3demo_enableAdditiveBlending()
{
	// result = ( new*[1] ) + ( old*[1] )
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
}

// default stencil state for writing
extern inline void a3demo_enableStencilWrite()
{
	glStencilFunc(GL_ALWAYS, 1, 0xff);			// any stencil value will be set to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	// replace stencil value if S&D tests pass
	glStencilMask(0xff);						// write to stencil buffer
}

// default stencil state for comparing
extern inline void a3demo_enableStencilCompare()
{
	glStencilFunc(GL_EQUAL, 1, 0xff);			// stencil test passes if equal to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		// keep stencil value if S&D tests pass
	glStencilMask(0x00);						// don't write to stencil buffer
}


// set default GL state
extern inline void a3demo_setDefaultGraphicsState()
{
	const a3f32 lineWidth = 2.0f;
	const a3f32 pointSize = 4.0f;

	// lines and points
	glLineWidth(lineWidth);
	glPointSize(pointSize);

	// backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// textures
	glEnable(GL_TEXTURE_2D);

	// background
	glClearColor(0.0f, 0.0f, 0.0, 0.0f);

	// alpha blending
	a3demo_enableCompositeBlending();
}

// set scene draw state
extern inline void a3demo_setSceneState(a3_Framebuffer const* currentWriteFBO, a3boolean displaySkybox)
{
	// activate FBO
	a3framebufferActivate(currentWriteFBO);

	// choose background color
//	if (displaySkybox)
//		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	else
//		glClearColor(0.1f, 0.1f, 0.6f, 1.0f);

	// clear now, handle skybox later
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


//-----------------------------------------------------------------------------
// RENDER SUB-ROUTINES

extern inline void a3demo_drawModelSimple(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program)
{
	a3real4x4Product(modelViewProjectionMat, viewProjectionMat, modelMat);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uMVP, 1, *modelViewProjectionMat);
	a3vertexDrawableRenderActive();
}

extern inline void a3demo_drawModelSimple_activateModel(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable)
{
	a3real4x4Product(modelViewProjectionMat, viewProjectionMat, modelMat);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uMVP, 1, *modelViewProjectionMat);
	a3vertexDrawableActivateAndRender(drawable);
}

extern inline void a3demo_drawModelSolidColor(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable, a3real4p const color)
{
	// set up render
	a3shaderProgramActivate(program->program);
	a3shaderUniformSendFloat(a3unif_vec4, program->uColor, 1, color);
	a3demo_drawModelSimple_activateModel(modelViewProjectionMat, viewProjectionMat, modelMat, program, drawable);
}

extern inline void a3demo_drawModelTexturedColored_invertModel(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3real4x4p const atlasMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable, a3_Texture const* texture, a3real4p const color)
{
	// set up render
	a3shaderProgramActivate(program->program);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uAtlas, 1, *atlasMat);
	a3shaderUniformSendFloat(a3unif_vec4, program->uColor, 1, color);
	a3textureActivate(texture, a3tex_unit00);

	// draw inverted
	glCullFace(GL_FRONT);
	a3demo_drawModelSimple_activateModel(modelViewProjectionMat, viewProjectionMat, modelMat, program, drawable);
	glCullFace(GL_BACK);
}

extern inline void a3demo_drawModelLighting(a3real4x4p modelViewProjectionMat, a3real4x4p modelViewMat, a3real4x4p const viewProjectionMat, a3real4x4p const viewMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const*program, a3_VertexDrawable const* drawable, a3real4p const color)
{
	// set up render
	a3real4x4Product(modelViewMat, viewMat, modelMat);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uMV, 1, *modelViewMat);
	a3demo_quickInvertTranspose_internal(modelViewMat);
	a3real4SetReal4(modelViewMat[3], a3vec4_zero.v);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uMV_nrm, 1, *modelViewMat);
	a3shaderUniformSendFloat(a3unif_vec4, program->uColor, 1, color);

	// draw
	a3demo_drawModelSimple_activateModel(modelViewProjectionMat, viewProjectionMat, modelMat, program, drawable);
}

extern inline void a3demo_drawModelLighting_bias_other(a3real4x4p modelViewProjectionBiasMat_other, a3real4x4p modelViewProjectionMat, a3real4x4p modelViewMat, a3real4x4p const viewProjectionBiasMat_other, a3real4x4p const viewProjectionMat, a3real4x4p const viewMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable, a3real4p const color)
{
	// set up render
	a3real4x4Product(modelViewMat, viewMat, modelMat);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uMV, 1, *modelViewMat);
	a3demo_quickInvertTranspose_internal(modelViewMat);
	a3real4SetReal4(modelViewMat[3], a3vec4_zero.v);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uMV_nrm, 1, *modelViewMat);
	a3shaderUniformSendFloat(a3unif_vec4, program->uColor, 1, color);
	a3real4x4Product(modelViewProjectionBiasMat_other, viewProjectionBiasMat_other, modelMat);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, program->uMVPB_other, 1, *modelViewProjectionBiasMat_other);

	// draw
	a3demo_drawModelSimple_activateModel(modelViewProjectionMat, viewProjectionMat, modelMat, program, drawable);
}

extern inline void a3demo_drawStencilTest(a3real4x4p modelViewProjectionMat, a3real4x4p const viewProjectionMat, a3real4x4p const modelMat, a3_DemoStateShaderProgram const* program, a3_VertexDrawable const* drawable)
{
	// draw to stencil buffer: 
	//	- render first sphere to the stencil buffer to set drawable area
	//		- don't want values for the shape to actually be drawn to 
	//			color or depth buffers, so apply a MASK for this object
	//	- enable stencil test for everything else

	// drawing "lens" object using simple program
	a3shaderProgramActivate(program->program);

	// default stencil write settings
	a3demo_enableStencilWrite();

	// enable test and clear buffer (do this after mask is set)
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	// disable drawing this object to color or depth
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);

	// inverted small sphere in solid transparent color
	// used as our "lens" for the depth and stencil tests
	glCullFace(GL_FRONT);
	a3demo_drawModelSimple_activateModel(modelViewProjectionMat, viewProjectionMat, modelMat, program, drawable);
	glCullFace(GL_BACK);

	// enable drawing following objects to color and depth
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// default stencil compare settings
	a3demo_enableStencilCompare();
}


//-----------------------------------------------------------------------------
