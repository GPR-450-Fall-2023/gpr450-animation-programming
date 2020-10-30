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
	
	a3_DemoState_idle-render.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     RENDERING THE STATS in this file.        ***
	****************************************************
*/

//-----------------------------------------------------------------------------

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
// RENDER TEXT

void a3starter_render_controls(a3_DemoState const* demoState, a3_DemoMode0_Starter const* demoMode,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset);
void a3animation_render_controls(a3_DemoState const* demoState, a3_DemoMode1_Animation const* demoMode,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset);


// display current mode controls
void a3demo_render_controls_global(a3_DemoState const* demoState,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Xbox controller target object control: ");
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"    Right joystick = rotate | Left joystick & triggers = move");
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Toggle text display:        'Dpad up/down' (toggle) | 'Select/back' (alloc/dealloc) ");
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Reload all shader programs: 'Start' ****CHECK CONSOLE FOR ERRORS!**** ");
	}
	else
	{
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Keyboard/mouse target object control: ");
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"    Left click & drag = rotate | WASDEQ = move | wheel = zoom");
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Toggle text display:        't' (toggle) | 'T' (alloc/dealloc) ");
		a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Reload all shader programs: 'P' ****CHECK CONSOLE FOR ERRORS!**** ");
	}
}

// display current mode controls
void a3demo_render_controls(a3_DemoState const* demoState,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// display mode info
	a3byte const* modeText[demoState_mode_max] = {
		"STARTER SCENE",
		"ANIMATION SCENE",
	};

	// demo mode
	a3_DemoState_ModeName const demoMode = demoState->demoMode;

	// demo mode
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Demo mode (%u / %u) ('</,' prev | next '>/.'): %s", demoMode + 1, demoState_mode_max, modeText[demoMode]);

	// draw controls for specific modes
	switch (demoMode)
	{
	case demoState_modeStarter:
		a3starter_render_controls(demoState, demoState->demoMode0_starter, text, col, textAlign, textDepth, textOffsetDelta, textOffset);
		break;
	case demoState_modeAnimation:
		a3animation_render_controls(demoState, demoState->demoMode1_animation, text, col, textAlign, textDepth, textOffsetDelta, textOffset);
		break;
	}

	// global/input-dependent controls
	textOffset = -0.6f;
	a3demo_render_controls_global(demoState, text, col,
		textAlign, textDepth, textOffsetDelta, textOffset);
}


// display general controls
void a3demo_render_controls_gen(a3_DemoState const* demoState,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// boolean text
	a3byte const boolText[2][4] = {
		"OFF",
		"ON ",
	};

	// toggles
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"GRID (toggle 'g') %s | SKYBOX ('b') %s | HIDDEN VOLUMES ('h') %s", boolText[demoState->displayGrid], boolText[demoState->displaySkybox], boolText[demoState->displayHiddenVolumes]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"WORLD AXES (toggle 'x') %s | OBJECT AXES ('z') %s", boolText[demoState->displayWorldAxes], boolText[demoState->displayObjectAxes]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"TANGENT BASES ('B') %s | WIREFRAME ('F') %s", boolText[demoState->displayTangentBases], boolText[demoState->displayWireframe]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"ANIMATION (toggle 'm') %s", boolText[demoState->updateAnimation]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"STENCIL TEST (toggle 'i') %s", boolText[demoState->stencilTest]);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"SKIP INTERMEDIATE PASSES (toggle 'I') %s", boolText[demoState->skipIntermediatePasses]);

	// global/input-dependent controls
	textOffset = -0.6f;
	a3demo_render_controls_global(demoState, text, col,
		textAlign, textDepth, textOffsetDelta, textOffset);
}


// scene data (HUD)
void a3demo_render_data(const a3_DemoState* demoState,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// display some general data
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"fps_target = %07.4lf F/s", demoState->timer->ticksPerSecond);//(a3f64)demoState->timer_display->ticks / demoState->timer_display->totalTime);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"fps_current = %07.4lf F/s", 1.0 / (demoState->dt_timer));//demoState->timer_display->previousTick));
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"fps_average = %07.4lf F/s", (a3f64)demoState->n_timer / (demoState->dt_timer_tot));//(a3f64)demoState->n_timer / demoState->timer_display->totalTime);//(a3f64)demoState->timer_display->ticks / demoState->timer_display->totalTime);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"dt_render = %07.4lf ms", (demoState->dt_timer) * 1000.0);//demoState->timer_display->previousTick * 1000.0);
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"t_render = %07.4lf s | n_render = %lu", demoState->timer_display->totalTime, demoState->n_timer);//demoState->timer_display->totalTime, demoState->timer_display->ticks);

	// global/input-dependent controls
	textOffset = -0.6f;
	a3demo_render_controls_global(demoState, text, col,
		textAlign, textDepth, textOffsetDelta, textOffset);
}

/*
// bloom iteration
void a3demo_render_bloomIteration(a3_DemoState const* demoState, a3real2 pixelSize, a3_Framebuffer const* fbo_prev,
	a3_Framebuffer const* fbo_bright, a3_Framebuffer const* fbo_blur_horiz, a3_Framebuffer const* fbo_blur_vert)
{
	a3_DemoStateShaderProgram const* currentDemoProgram;
	const a3vec2 sampleAxisH = { +a3real_one, +a3real_one };// a3vec2_x;
	const a3vec2 sampleAxisV = { +a3real_one, -a3real_one };// a3vec2_y;

}
*/

//-----------------------------------------------------------------------------
// RENDER

void a3demo_render(a3_DemoState const* demoState, a3f64 const dt)
{
	// display mode for current pipeline
	// ensures we don't go through the whole pipeline if not needed
	a3_DemoState_ModeName const demoMode = demoState->demoMode;


	// amount to offset text as each line is rendered
	a3f32 const textAlign = -0.98f;
	a3f32 const textDepth = -1.00f;
	a3f32 const textOffsetDelta = -0.08f;
	a3f32 textOffset = +1.00f;


	demoState->demoModeCallbacksPtr->handleRender(demoState,
		demoState->demoModeCallbacksPtr->demoMode, dt);


	// deactivate things
	a3vertexDrawableDeactivate();
	a3shaderProgramDeactivate();
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable, 0, 0, demoState->windowWidth, demoState->windowHeight);
	a3textureDeactivate(a3tex_unit00);


	// text
	if (demoState->textInit)
	{
		// 5 passes of text to get decent visibility
		a3_TextRenderer const* text = demoState->text;
		a3vec4 const bg = a3vec4_w, fg = a3vec4_one;
		a3real const px = demoState->windowWidthInv * a3real_two, py = demoState->windowHeightInv * a3real_two;

		a3vec4 const color[5] = { bg, bg, bg, bg, fg };
		a3real const x_offset[5] = { -px, +px, -px, +px, a3real_zero };
		a3real const y_offset[5] = { -py, -py, +py, +py, a3real_zero };

		a3vec4 col;
		a3real x, y;
		a3ui32 i;
		for (i = 0; i < 5; ++i)
		{
			col = color[i];
			x = x_offset[i];
			y = y_offset[i];

			// choose text render mode
			switch (demoState->textMode)
			{
				// controls for current mode
			case demoState_textControls:
				a3demo_render_controls(demoState, text, col, textAlign + x, textDepth, textOffsetDelta, textOffset + y);
				break;

				// controls for general
			case demoState_textControls_gen:
				a3demo_render_controls_gen(demoState, text, col, textAlign + x, textDepth, textOffsetDelta, textOffset + y);
				break;

				// general data
			case demoState_textData:
				a3demo_render_data(demoState, text, col, textAlign + x, textDepth, textOffsetDelta, textOffset + y);
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
