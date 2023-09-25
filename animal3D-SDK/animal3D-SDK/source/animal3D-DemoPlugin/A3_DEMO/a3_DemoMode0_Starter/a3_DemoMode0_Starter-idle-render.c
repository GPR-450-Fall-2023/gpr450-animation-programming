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

	a3_DemoMode0_Starter_idle-render.c
	Demo mode implementations: starter scene.

	********************************************
	*** RENDERING FOR STARTER SCENE MODE     ***
	********************************************
*/

/*
	animal3D SDK: Keyframe and Clip Controller Framework
	By Dillon Drummond, Neo Kattan, Joseph Lyons

	a3_DemoMode0_Starter-render.c
	Demo mode implementations: keyframe and clip controller

	RENDER FOR KEYFRAME AND CLIP CONTROLLER
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode0_Starter.h"

#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoRenderUtils.h"


// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32


//-----------------------------------------------------------------------------

// controls for pipelines mode
void a3starter_render_controls(a3_DemoState const* demoState, a3_DemoMode0_Starter const* demoMode,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// display mode info
	a3byte const* pipelineText[starter_pipeline_max] = {
		"Forward rendering",
	};

	// forward pipeline names
	a3byte const* renderProgramName[starter_render_max] = {
		"Solid color",
		"Texture",
	};

	// forward display names
	a3byte const* displayProgramName[starter_display_max] = {
		"Texture",
	};

	// active camera name
	a3byte const* cameraText[starter_camera_max] = {
		"starter scene camera",
	};

	// clip controller names
	a3byte const* clipControllers[3] = {
		"Clip Controller 1",
		"Clip Controller 2",
		"Clip Controller 3",
	};

	// play/pause names
	a3byte const* playingState[starter_play_pause_max] = {
		"Playing",
		"Paused",
	};

	// clip names
	a3byte const* clips[5] = {
		"Clip 1",
		"Clip 2",
		"Clip 3",
		"Clip 4",
		"Clip 5",
	};

	// playback direction names
	a3byte const* playbackDirection[starter_playing_max] = {
		"Forward",
		"Backward",
	};

	// slowmo speed names
	a3byte const* slowmoSpeed[starter_slowmo_speed_max] = {
		"Normal Speed",
		"Half Speed",
		"Quarter Speed",
	};

	// terminus action names
	a3byte const* terminusAction[starter_terminus_max] = {
		"Loop",
		"Stop",
		"Ping Pong",
	};

	// constant color target names
	a3byte const colorBufferText[] = "Color target 0: FINAL DISPLAY COLOR";
	// constant depth target name
	a3byte const depthBufferText[] = "Depth buffer";

	// pass names
	a3byte const* passName[starter_pass_max] = {
		"Pass: Render scene objects",
		"Pass: Composite",
	};
	a3byte const* targetText_scene[starter_target_scene_max] = {
		colorBufferText,
		depthBufferText,
	};
	a3byte const* targetText_composite[starter_target_scene_max] = {
		colorBufferText,
	};
	a3byte const* const* targetText[starter_pass_max] = {
		targetText_scene,
		targetText_composite,
	};

	// pipeline and target
	a3_DemoMode0_Starter_RenderProgramName const render = demoMode->render;
	a3_DemoMode0_Starter_DisplayProgramName const display = demoMode->display;
	a3_DemoMode0_Starter_ActiveCameraName const activeCamera = demoMode->activeCamera;
	a3_DemoMode0_Starter_PipelineName const pipeline = demoMode->pipeline;
	a3_DemoMode0_Starter_PassName const pass = demoMode->pass;
	a3_DemoMode0_Starter_TargetName const targetIndex = demoMode->targetIndex[pass];
	a3_DemoMode0_Starter_TargetName const targetCount = demoMode->targetCount[pass];

	//a3_DemoMode0_Starter_Play_Pause const playPause = demoMode->playPause;
	//a3_DemoMode0_Starter_Playing_Direction const forwardBackward = demoMode->forwardBackward;
	//a3_DemoMode0_Starter_Slowmo_Speed const slowmo = demoMode->slowmo;
	//a3_DemoMode0_Starter_Terminus_Action const terminus = demoMode->terminus;

	// demo modes
	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"    Pipeline (%u / %u) ('[' | ']'): %s", pipeline + 1, starter_pipeline_max, pipelineText[pipeline]);
	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"    Display pass (%u / %u) ('(' | ')'): %s", pass + 1, starter_pass_max, passName[pass]);
	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"        Target (%u / %u) ('{' | '}'): %s", targetIndex + 1, targetCount, targetText[pass][targetIndex]);

	// lighting modes
	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"    Rendering mode (%u / %u) ('j' | 'k'): %s", render + 1, starter_render_max, renderProgramName[render]);
	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"    Display mode (%u / %u) ('J' | 'K'): %s", display + 1, starter_display_max, displayProgramName[display]);
	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"    Active camera (%u / %u) ('c' prev | next 'v'): %s", activeCamera + 1, starter_camera_max, cameraText[activeCamera]);

	// Basic Testing Interface
	// COMMENTED OUT BY JOEY, THERE IS A BUG HERE THAT CAUSES A CRASH
	/*a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Controls:");
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Select clip controller to edit (q/w): %s", clipControllers[demoMode->currentController]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Play/pause controller playback (p)");
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Set to first/last frame in current clip (e/r)");
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Change clip to control (a/s): %s", clips[demoMode->clipCtrlPool.clipControllers[demoMode->currentController].clip]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Flip playback direction (b)");
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Slow-motion (n/m)");*/
	/*a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Terminus Action (%u / %u) ( , | . ): %s", terminus + 1, starter_terminus_max, terminusAction[terminus]);*/

	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"Clip 1:");
	//a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"\nClip Time : % f\nClip Duration : % f\nClip Normalized : % f\n\nKeyframeTime: % f\nKeyframe Duration : % f\nKeyframe Normalized : % f\n",
	//	clipCtrl->clipTime, clip.duration, clipCtrl->clipParameter, clipCtrl->keyframeTime, keyframe.duration, clipCtrl->keyframeParameter);


}


//-----------------------------------------------------------------------------

//void a3demo_render_bloomIteration(a3_DemoState const* demoState, a3real2 pixelSize, a3_Framebuffer const* fbo_prev,
//	a3_Framebuffer const* fbo_bright, a3_Framebuffer const* fbo_blur_horiz, a3_Framebuffer const* fbo_blur_vert);

// sub-routine for rendering the demo state using the shading pipeline
void a3starter_render(a3_DemoState const* demoState, a3_DemoMode0_Starter const* demoMode, a3f64 const dt)
{
	// pointers
	const a3_VertexDrawable* currentDrawable;
	const a3_DemoStateShaderProgram* currentDemoProgram;

	// framebuffers
	const a3_Framebuffer* currentWriteFBO;
	const a3_Framebuffer* currentReadFBO, * currentDisplayFBO;

	// indices
	a3ui32 i, j;

	// RGB
	const a3vec4 rgba4[] = {
		{ 1.00f, 0.00f, 0.00f, 1.00f },	// red
		{ 1.00f, 0.25f, 0.00f, 1.00f },
		{ 1.00f, 0.50f, 0.00f, 1.00f },	// orange
		{ 1.00f, 0.75f, 0.00f, 1.00f },
		{ 1.00f, 1.00f, 0.00f, 1.00f },	// yellow
		{ 0.75f, 1.00f, 0.00f, 1.00f },
		{ 0.50f, 1.00f, 0.00f, 1.00f },	// lime
		{ 0.25f, 1.00f, 0.00f, 1.00f },
		{ 0.00f, 1.00f, 0.00f, 1.00f },	// green
		{ 0.00f, 1.00f, 0.25f, 1.00f },
		{ 0.00f, 1.00f, 0.50f, 1.00f },	// aqua
		{ 0.00f, 1.00f, 0.75f, 1.00f },
		{ 0.00f, 1.00f, 1.00f, 1.00f },	// cyan
		{ 0.00f, 0.75f, 1.00f, 1.00f },
		{ 0.00f, 0.50f, 1.00f, 1.00f },	// sky
		{ 0.00f, 0.25f, 1.00f, 1.00f },
		{ 0.00f, 0.00f, 1.00f, 1.00f },	// blue
		{ 0.25f, 0.00f, 1.00f, 1.00f },
		{ 0.50f, 0.00f, 1.00f, 1.00f },	// purple
		{ 0.75f, 0.00f, 1.00f, 1.00f },
		{ 1.00f, 0.00f, 1.00f, 1.00f },	// magenta
		{ 1.00f, 0.00f, 0.75f, 1.00f },
		{ 1.00f, 0.00f, 0.50f, 1.00f },	// rose
		{ 1.00f, 0.00f, 0.25f, 1.00f },
	};
	const a3vec4 grey4[] = {
		{ 0.5f, 0.5f, 0.5f, 1.0f },	// solid grey
		{ 0.5f, 0.5f, 0.5f, 0.5f },	// translucent grey
	};
	const a3real
		* const red = rgba4[0].v, * const orange = rgba4[2].v, * const yellow = rgba4[4].v, * const lime = rgba4[6].v,
		* const green = rgba4[8].v, * const aqua = rgba4[10].v, * const cyan = rgba4[12].v, * const sky = rgba4[14].v,
		* const blue = rgba4[16].v, * const purple = rgba4[18].v, * const magenta = rgba4[20].v, * const rose = rgba4[22].v,
		* const grey = grey4[0].v, * const grey_t = grey4[1].v;
	const a3ui32 hueCount = sizeof(rgba4) / sizeof(*rgba4);

	// camera used for drawing
	const a3_DemoProjector* activeCamera = demoMode->projector + demoMode->activeCamera;
	const a3_DemoSceneObject* activeCameraObject = activeCamera->sceneObject;

	// current hull for scene object being rendered, for convenience
	const a3_DemoSceneObject* currentSceneObject, * endSceneObject;

	// temp drawable pointers
	const a3_VertexDrawable* drawable[] = {
		demoState->draw_unit_box,		// skybox
		demoState->draw_unit_plane_z,
		demoState->draw_unit_box,
		demoState->draw_unit_sphere,
		demoState->draw_unit_cylinder,
		demoState->draw_unit_capsule,
		demoState->draw_unit_torus,
		demoState->draw_teapot,
	};

	// temp texture pointers
	const a3_Texture* texture_dm[] = {
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
	};

	// forward pipeline shader programs
	const a3_DemoStateShaderProgram* renderProgram[starter_pipeline_max][starter_render_max] = {
		{
			demoState->prog_drawColorUnif,
			demoState->prog_drawTexture,
		},
	};

	// display shader programs
	const a3_DemoStateShaderProgram* displayProgram[starter_display_max] = {
		demoState->prog_drawTexture,
	};

	// framebuffers to which to write based on pipeline mode
	const a3_Framebuffer* writeFBO[starter_pass_max] = {
		demoState->fbo_scene_c16d24s8_mrt,
		demoState->fbo_composite_c16,
	};

	// framebuffers from which to read based on pipeline mode
	const a3_Framebuffer* readFBO[starter_pass_max][4] = {
		{ 0, },
		{ demoState->fbo_scene_c16d24s8_mrt, },
	};

	// target info
	a3_DemoMode0_Starter_RenderProgramName const render = demoMode->render;
	a3_DemoMode0_Starter_DisplayProgramName const display = demoMode->display;
	a3_DemoMode0_Starter_PipelineName const pipeline = demoMode->pipeline;
	a3_DemoMode0_Starter_PassName const pass = demoMode->pass;
	a3_DemoMode0_Starter_TargetName const targetIndex = demoMode->targetIndex[pass], targetCount = demoMode->targetCount[pass];
	a3_DemoMode0_Starter_PassName currentPass;


	// pixel size and effect axis
//	a3vec2 pixelSize = a3vec2_one;

	// FSQ matrix
	const a3mat4 fsq = {
		2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};


	// bias matrix
	const a3mat4 bias = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};
	const a3mat4 unbias = {
		 2.0f,  0.0f,  0.0f, 0.0f,
		 0.0f,  2.0f,  0.0f, 0.0f,
		 0.0f,  0.0f,  2.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
	};

	// final model matrix and full matrix stack
	a3mat4 viewProjectionMat = activeCamera->viewProjectionMat;
	a3mat4 modelViewProjectionMat = viewProjectionMat;
	a3mat4 modelMat = a3mat4_identity, modelViewMat = a3mat4_identity;
	a3mat4 projectionBiasMat = activeCamera->projectionMat, projectionBiasMat_inv = activeCamera->projectionMatInv;

	// init
	a3real4x4Product(projectionBiasMat.m, bias.m, activeCamera->projectionMat.m);
	a3real4x4Product(projectionBiasMat_inv.m, activeCamera->projectionMatInv.m, unbias.m);


	//-------------------------------------------------------------------------
	// 0) PRE-SCENE PASS: shadow pass renders scene to depth-only
	//	- activate shadow pass framebuffer
	//	- draw scene
	//		- clear depth buffer
	//		- render shapes using appropriate shaders
	//		- capture depth


	//-------------------------------------------------------------------------
	// 1) SCENE PASS: render scene with desired shader
	//	- activate scene framebuffer
	//	- draw scene
	//		- clear buffers
	//		- render shapes using appropriate shaders
	//		- capture color and depth

	// select target framebuffer
	currentPass = starter_passScene;
	currentWriteFBO = writeFBO[currentPass];
	switch (pipeline)
	{
		// shading with MRT
	case starter_forward:
		// target scene framebuffer
		a3demo_setSceneState(currentWriteFBO, demoState->displaySkybox);
		break;
	}


	// optional stencil test before drawing objects
	a3real4x4SetScale(modelMat.m, a3real_four);
	if (demoState->stencilTest)
		a3demo_drawStencilTest(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, demoState->prog_drawColorUnif, demoState->draw_unit_sphere);


	// select program based on settings
	currentDemoProgram = renderProgram[pipeline][render];
	a3shaderProgramActivate(currentDemoProgram->program);

	// send shared data: 
	//	- projection matrix
	//	- light data
	//	- activate shared textures including atlases if using
	//	- shared animation data
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, activeCamera->projectionMat.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP_inv, 1, activeCamera->projectionMatInv.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uPB, 1, projectionBiasMat.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uPB_inv, 1, projectionBiasMat_inv.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
	a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, hueCount, rgba4->v);
	a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->timer_display->totalTime);

	// select pipeline algorithm
	glDisable(GL_BLEND);
	switch (pipeline)
	{
		// scene pass using forward pipeline
	case starter_forward: {

		// forward shading algorithms
		switch (demoMode->render)
		{
		case starter_renderSolid:
		case starter_renderTexture:
			// individual object requirements: 
			//	- modelviewprojection
			//	- modelview
			//	- modelview for normals
			//	- per-object animation data
			for (currentSceneObject = demoMode->obj_plane, endSceneObject = demoMode->obj_teapot,
				j = (a3ui32)(currentSceneObject - demoMode->object_scene);
				currentSceneObject <= endSceneObject;
				++j, ++currentSceneObject)
			{
				// send data and draw
				i = (j * 2 + 11) % hueCount;
				currentDrawable = drawable[currentSceneObject - demoMode->obj_skybox];
				a3textureActivate(texture_dm[j], a3tex_unit00);
				a3real4x4Product(modelViewProjectionMat.m, viewProjectionMat.m, currentSceneObject->modelMat.m);
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
				a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, rgba4[i].v);
				a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &j);
				a3vertexDrawableActivateAndRender(currentDrawable);
			}
			break;
		}

	}	break;
		// end forward scene pass
	}


	// stop using stencil
	if (demoState->stencilTest)
		glDisable(GL_STENCIL_TEST);


	//-------------------------------------------------------------------------
	// COMPOSITE PASS
	//	- activate composite framebuffer
	//	- composite scene layers

	currentPass = starter_passComposite;
	currentWriteFBO = writeFBO[currentPass];
	a3framebufferActivate(currentWriteFBO);

	// composite skybox
	currentDemoProgram = demoState->displaySkybox ? demoState->prog_drawTexture : demoState->prog_drawColorUnif;
	a3demo_drawModelTexturedColored_invertModel(modelViewProjectionMat.m, viewProjectionMat.m, demoMode->obj_skybox->modelMat.m, a3mat4_identity.m, currentDemoProgram, demoState->draw_unit_box, demoState->tex_skybox_clouds, a3vec4_one.v);
	a3demo_enableCompositeBlending();

	// draw textured quad with previous pass image on it
	// repeat as necessary to complete composite
	currentDrawable = demoState->draw_unit_plane_z;
	a3vertexDrawableActivate(currentDrawable);

	switch (pipeline)
	{
	case starter_forward:
		// use simple texturing program
		currentDemoProgram = demoState->prog_drawTexture;
		a3shaderProgramActivate(currentDemoProgram->program);
		// scene (color)
		currentReadFBO = readFBO[currentPass][0];
		a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
		break;
	}
	// reset other uniforms
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, fsq.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
	a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
	a3vertexDrawableRenderActive();


	//-------------------------------------------------------------------------
	// PREPARE FOR POST-PROCESSING
	//	- double buffer swap (if applicable)
	//	- ensure blending is disabled
	//	- re-activate FSQ drawable IF NEEDED (i.e. changed in previous step)
	glDisable(GL_BLEND);
	currentDrawable = demoState->draw_unit_plane_z;
	a3vertexDrawableActivate(currentDrawable);


	//-------------------------------------------------------------------------
	// POST-PROCESSING
	//	- activate target framebuffer
	//	- activate texture from previous framebuffer
	//	- draw FSQ with processing program active


	//-------------------------------------------------------------------------
	// DISPLAY: final pass, perform and present final composite
	//	- finally draw to back buffer
	//	- select display texture(s)
	//	- activate final pass program
	//	- draw final FSQ

	// revert to back buffer and disable depth testing
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
		-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);

	// select framebuffer to display based on mode
	currentDisplayFBO = writeFBO[demoMode->pass];

	// select output to display
	switch (demoMode->pass)
	{
	case starter_passScene:
		if (currentDisplayFBO->color && (!currentDisplayFBO->depthStencil || targetIndex < targetCount - 1))
			a3framebufferBindColorTexture(currentDisplayFBO, a3tex_unit00, targetIndex);
		else
			a3framebufferBindDepthTexture(currentDisplayFBO, a3tex_unit00);
		break;
	case starter_passComposite:
		a3framebufferBindColorTexture(currentDisplayFBO, a3tex_unit00, targetIndex);
		break;
	}


	// final display: activate desired final program and draw FSQ
	if (currentDisplayFBO)
	{
		// prepare for final draw
		currentDrawable = demoState->draw_unit_plane_z;
		a3vertexDrawableActivate(currentDrawable);

		// determine if additional passes are required
		currentDemoProgram = displayProgram[display];
		a3shaderProgramActivate(currentDemoProgram->program);

		switch (demoMode->display)
		{
			// most basic option: simply display texture
		case starter_displayTexture:
			break;
		}

		// done
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, fsq.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
		a3vertexDrawableRenderActive();
	}


	//-------------------------------------------------------------------------
	// OVERLAYS: done after FSQ so they appear over everything else
	//	- disable depth testing
	//	- draw overlays appropriately

	// enable alpha
	a3demo_enableCompositeBlending();

	// scene overlays
	if (demoMode->pass >= starter_passScene)
	{
		if (demoState->displayGrid || demoState->displayTangentBases || demoState->displayWireframe)
		{
			// activate scene FBO and clear color; reuse depth
			currentWriteFBO = demoState->fbo_scene_c16d24s8_mrt;
			a3framebufferActivate(currentWriteFBO);
			glDisable(GL_STENCIL_TEST);
			glClear(GL_COLOR_BUFFER_BIT);
		
			// draw grid aligned to world
			if (demoState->displayGrid)
			{
				a3demo_drawModelSolidColor(modelViewProjectionMat.m, viewProjectionMat.m, a3mat4_identity.m, demoState->prog_drawColorUnif, demoState->draw_grid, blue);
			}
		
			if (demoState->displayTangentBases || demoState->displayWireframe)
			{
				const a3i32 flag[1] = { demoState->displayTangentBases * 3 + demoState->displayWireframe * 4 };
				const a3f32 size[1] = { 0.0625f };

				currentDemoProgram = demoState->prog_drawTangentBasis;
				a3shaderProgramActivate(currentDemoProgram->program);

				// projection matrix
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, activeCamera->projectionMat.mm);
				// wireframe color
				a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor0, hueCount, rgba4->v);
				// blend color
				a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
				// tangent basis size
				a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uSize, 1, size);
				// overlay flag
				a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uFlag, 1, flag);

				// draw objects again
				for (currentSceneObject = demoMode->obj_plane, endSceneObject = demoMode->obj_teapot,
					j = (a3ui32)(currentSceneObject - demoMode->object_scene);
					currentSceneObject <= endSceneObject;
					++j, ++currentSceneObject)
				{
					// calculate per-object uniforms
					i = (j * 2 + 23) % hueCount;
					currentDrawable = drawable[currentSceneObject - demoMode->obj_skybox];
					a3real4x4Product(modelViewMat.m, activeCameraObject->modelMatInv.m, currentSceneObject->modelMat.m);
					a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV, 1, modelViewMat.mm);
					a3demo_quickInvertTranspose_internal(modelViewMat.m);
					modelViewMat.v3 = a3vec4_zero;
					a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV_nrm, 1, modelViewMat.mm);
					a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
					a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &i);
					a3vertexDrawableActivateAndRender(currentDrawable);
				}
			}

			// display color target with scene overlays
			a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
				-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);
			currentDrawable = demoState->draw_unit_plane_z;
			currentDemoProgram = demoState->prog_drawTexture;
			a3vertexDrawableActivate(currentDrawable);
			a3shaderProgramActivate(currentDemoProgram->program);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, fsq.mm);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
			a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
			a3vertexDrawableRenderActive();
		}

		// hidden volumes
		if (demoState->displayHiddenVolumes)
		{
			#define MAX_KEYFRAMES 1024
			#define NUM_VEC_COMPONENTS 3

			// DRAW SPLINE
			currentDemoProgram = demoState->prog_drawSpline;
			a3shaderProgramActivate(currentDemoProgram->program);
			a3vertexDrawableDeactivate();

			a3_ClipController controller = demoMode->clipCtrlPool.clipControllers[0];
			a3_Clip clip = controller.clipPool->clip[controller.clip];

			for(a3ui32 axisIndex = 0; axisIndex < NUM_VEC_COMPONENTS; axisIndex++)
			{
				a3real verticalOffsetOnScreen = ((2.0f / NUM_VEC_COMPONENTS) * axisIndex) + (1.0f / NUM_VEC_COMPONENTS);

				a3vec3 k[MAX_KEYFRAMES];

				float xProgress = -1.2f;
				float yMin = 999999;
				float yMax = -999999;

				for (a3ui32 i = 0; i < clip.keyframeCount; i++)
				{
					a3_Keyframe keyframe = clip.keyframePool->keyframe[clip.firstKeyframeIndex + i];
					
					a3vec3 vec;

					float xPortion = (keyframe.duration / clip.duration) * 2;

					vec.x = xProgress + xPortion; // replace with * durationInverse later
					xProgress += xPortion;

					// Y is our dependent variable, for now we'll say it's the y value of keyframe data
					vec.y = keyframe.data[axisIndex];

					if (vec.y < yMin)
					{
						yMin = vec.y;
					}

					if (vec.y > yMax)
					{
						yMax = vec.y;
					}

					vec.z = 0;

					k[i] = vec;
				}

				for (a3ui32 i = 0; i < clip.keyframeCount; i++)
				{
					k[i].y = (k[i].y - yMin) / (yMax - yMin);
					k[i].y = (k[i].y * .4f) - .2f;
					k[i].y += -1 + (2.0f * axisIndex / NUM_VEC_COMPONENTS) + (1.0f / NUM_VEC_COMPONENTS);
				}

				for (a3ui32 i = 0; i < clip.keyframeCount; i++)
				{
					a3ui32 sectionDataCount = 4;

					a3ui32 iPrev;

					if (i == 0)
					{
						iPrev = 0;
					}
					else
					{
						iPrev = i - 1;
					}

					a3ui32 i0 = i;
					a3ui32 i1 = min(i + 1, clip.keyframeCount - 1);
					a3ui32 iNext = min(i + 2, clip.keyframeCount - 1);


					a3vec3 sectionData[] =
					{
						k[iPrev],
						k[i0],
						k[i1],
						k[iNext]
					};

					if (i == controller.keyframe - controller.clipPool->clip[controller.clip].firstKeyframeIndex)
					{
						a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, blue);
					}
					else
					{
						a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, red);
					}

					a3shaderUniformSendFloat(a3unif_vec3, currentDemoProgram->uAxis, 4, (a3f32*) sectionData);
					a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uCount, 1, &sectionDataCount);

					glDrawArrays(GL_POINTS, 0, 1);
				}
			}

			a3real normalizedTime = controller.clipTime / clip.duration;

			normalizedTime = (2 * normalizedTime) - 1;

			a3ui32 negOne = -1;



			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uFlag, 1, &normalizedTime);
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uCount, 1, &negOne);

			glDrawArrays(GL_POINTS, 0, 1);


		}


		// superimpose axes
		// draw coordinate axes in front of everything
		currentDemoProgram = demoState->prog_drawColorAttrib;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3vertexDrawableActivate(demoState->draw_axes);

		// center of world from current viewer
		// also draw other viewer/viewer-like object in scene
		if (demoState->displayWorldAxes)
		{
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, viewProjectionMat.mm);
			a3vertexDrawableRenderActive();
		}

		// individual objects
		if (demoState->displayObjectAxes)
		{
			// scene objects
			for (currentSceneObject = demoMode->obj_plane, endSceneObject = demoMode->obj_teapot,
				j = (a3ui32)(currentSceneObject - demoMode->object_scene);
				currentSceneObject <= endSceneObject;
				++j, ++currentSceneObject)
				a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, currentSceneObject->modelMat.m, currentDemoProgram);
		}
	}
}


//-----------------------------------------------------------------------------
