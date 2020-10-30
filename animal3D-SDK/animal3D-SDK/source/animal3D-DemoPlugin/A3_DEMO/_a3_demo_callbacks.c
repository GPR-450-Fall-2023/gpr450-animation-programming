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
	
	_a3_demo_callbacks.c/.cpp
	Main implementation of window callback hooks.

	********************************************
	*** THIS IS THE LIBRARY'S CALLBACKS FILE ***
	*** App hooks your demo via this file.   ***
	********************************************
*/


#include "_a3_dylib_config_export.h"

#include "a3_DemoState.h"

#include "_a3_demo_utilities/a3_DemoMacros.h"
#include "_a3_demo_utilities/a3_DemoRenderUtils.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//-----------------------------------------------------------------------------
// callback prototypes
// NOTE: do not move to header; they should be private to this file
// NOTE: you may name these functions whatever you like, just be sure to 
//	update your debug config file: 
//	"<root>/resource/animal3D-data/animal3D-demoinfo-debug.txt"
// copy this config line and the DLL to your main config with a new name when 
//	you're happy with it: 
//	"<root>/resource/animal3D-data/animal3D-demoinfo.txt"

#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus

	A3DYLIBSYMBOL a3_DemoState* a3demoCB_load(a3_DemoState* demoState, a3boolean hotbuild);
	A3DYLIBSYMBOL a3_DemoState* a3demoCB_unload(a3_DemoState* demoState, a3boolean hotbuild);
	A3DYLIBSYMBOL a3i32 a3demoCB_display(a3_DemoState* demoState);
	A3DYLIBSYMBOL a3i32 a3demoCB_idle(a3_DemoState* demoState);
	A3DYLIBSYMBOL void a3demoCB_windowActivate(a3_DemoState* demoState);
	A3DYLIBSYMBOL void a3demoCB_windowDeactivate(a3_DemoState* demoState);
	A3DYLIBSYMBOL void a3demoCB_windowMove(a3_DemoState* demoState, a3i32 newWindowPosX, a3i32 newWindowPosY);
	A3DYLIBSYMBOL void a3demoCB_windowResize(a3_DemoState* demoState, a3i32 newWindowWidth, a3i32 newWindowHeight);
	A3DYLIBSYMBOL void a3demoCB_keyPress(a3_DemoState* demoState, a3i32 virtualKey);
	A3DYLIBSYMBOL void a3demoCB_keyHold(a3_DemoState* demoState, a3i32 virtualKey);
	A3DYLIBSYMBOL void a3demoCB_keyRelease(a3_DemoState* demoState, a3i32 virtualKey);
	A3DYLIBSYMBOL void a3demoCB_keyCharPress(a3_DemoState* demoState, a3i32 asciiKey);
	A3DYLIBSYMBOL void a3demoCB_keyCharHold(a3_DemoState* demoState, a3i32 asciiKey);
	A3DYLIBSYMBOL void a3demoCB_mouseClick(a3_DemoState* demoState, a3i32 button, a3i32 cursorX, a3i32 cursorY);
	A3DYLIBSYMBOL void a3demoCB_mouseDoubleClick(a3_DemoState* demoState, a3i32 button, a3i32 cursorX, a3i32 cursorY);
	A3DYLIBSYMBOL void a3demoCB_mouseRelease(a3_DemoState* demoState, a3i32 button, a3i32 cursorX, a3i32 cursorY);
	A3DYLIBSYMBOL void a3demoCB_mouseWheel(a3_DemoState* demoState, a3i32 delta, a3i32 cursorX, a3i32 cursorY);
	A3DYLIBSYMBOL void a3demoCB_mouseMove(a3_DemoState* demoState, a3i32 cursorX, a3i32 cursorY);
	A3DYLIBSYMBOL void a3demoCB_mouseLeave(a3_DemoState* demoState);

#ifdef __cplusplus
}
#endif	// __cplusplus


//-----------------------------------------------------------------------------
// demo state callbacks

// input callbacks
void a3demo_input_keyCharPress(a3_DemoState* demoState, a3i32 const asciiKey);
void a3demo_input_keyCharHold(a3_DemoState* demoState, a3i32 const asciiKey);

// idle loop
void a3demo_input(a3_DemoState* demoState, a3f64 const dt);
void a3demo_update(a3_DemoState* demoState, a3f64 const dt);
void a3demo_render(a3_DemoState const* demoState, a3f64 const dt);


//-----------------------------------------------------------------------------

// loading
void a3demo_loadGeometry(a3_DemoState* demoState);
void a3demo_loadShaders(a3_DemoState* demoState);
void a3demo_loadTextures(a3_DemoState* demoState);
void a3demo_loadFramebuffers(a3_DemoState* demoState);
void a3demo_loadValidate(a3_DemoState* demoState);

// unloading
void a3demo_unloadGeometry(a3_DemoState* demoState);
void a3demo_unloadShaders(a3_DemoState* demoState);
void a3demo_unloadTextures(a3_DemoState* demoState);
void a3demo_unloadFramebuffers(a3_DemoState* demoState);
void a3demo_unloadValidate(a3_DemoState const* demoState);


//-----------------------------------------------------------------------------

void a3starter_load(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode);
void a3animation_load(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode);

void a3starter_loadValidate(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode);
void a3animation_loadValidate(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode);

void a3starter_unload(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode);
void a3animation_unload(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode);

void a3starter_unloadValidate(a3_DemoState const* demoState, a3_DemoMode0_Starter* demoMode);
void a3animation_unloadValidate(a3_DemoState const* demoState, a3_DemoMode1_Animation* demoMode);


//-----------------------------------------------------------------------------
// miscellaneous functions

// get the size of the persistent state to allocate
//	(good idea to keep it relatively constant, so that addresses don't change 
//	when the library is reloaded... that would mess everything up!)
inline a3ui32 a3demo_getPersistentStateSize()
{
	const a3ui32 minimum = sizeof(a3_DemoState);
	a3ui32 size = 1;
	while (size < minimum)
		size += size;
	return size;
}


// consistent text initialization
inline void a3demo_initializeText(a3_DemoState* demoState)
{
	a3textInitialize(demoState->text + 0, 18, 1, 0, 0, 0);
	a3textInitialize(demoState->text + 1, 16, 0, 0, 0, 0);
}

inline void a3demo_releaseText(a3_DemoState* demoState)
{
	a3textRelease(demoState->text + 0);
	a3textRelease(demoState->text + 1);
}


//-----------------------------------------------------------------------------

void a3demo_load(a3_DemoState* demoState)
{
	// demo modes
	demoState->demoMode = demoState_modeAnimation;
	demoState->demoModeCallbacksPtr = demoState->demoModeCallbacks + demoState->demoMode;
	a3starter_load(demoState, demoState->demoMode0_starter);
	a3animation_load(demoState, demoState->demoMode1_animation);


	// geometry
	a3demo_loadGeometry(demoState);

	// shaders
	a3demo_loadShaders(demoState);

	// textures
	a3demo_loadTextures(demoState);


	// set flags
	demoState->displayGrid = a3true;
	demoState->displayWorldAxes = a3true;
	demoState->displayObjectAxes = a3true;
	demoState->displayTangentBases = a3false;
	demoState->displayWireframe = a3false;
	demoState->displaySkybox = a3true;
	demoState->displayHiddenVolumes = a3true;
	demoState->updateAnimation = a3true;
	demoState->stencilTest = a3false;
	demoState->skipIntermediatePasses = a3false;
}

void a3demo_unload(a3_DemoState* demoState)
{
	a3demo_unloadGeometry(demoState);
	a3demo_unloadShaders(demoState);
	a3demo_unloadTextures(demoState);
	a3demo_unloadFramebuffers(demoState);

	a3starter_unload(demoState, demoState->demoMode0_starter);
	a3animation_unload(demoState, demoState->demoMode1_animation);
}

void a3demoMode_loadValidate(a3_DemoState* demoState)
{
	demoState->demoModeCallbacksPtr = demoState->demoModeCallbacks + demoState->demoMode;
	a3starter_loadValidate(demoState, demoState->demoMode0_starter);
	a3animation_loadValidate(demoState, demoState->demoMode1_animation);
}

void a3demoMode_unloadValidate(a3_DemoState* demoState)
{
	a3starter_unloadValidate(demoState, demoState->demoMode0_starter);
	a3animation_unloadValidate(demoState, demoState->demoMode1_animation);
}

void a3demo_idle(a3_DemoState* demoState, a3f64 const dt)
{
	// track updates
	if (demoState->timer->totalTime > 2.0)
	{
		demoState->n_timer += 1;
		demoState->dt_timer = demoState->timer_display->totalTime - demoState->t_timer;
		demoState->dt_timer_tot += demoState->dt_timer;
		demoState->t_timer = demoState->timer_display->totalTime;
	}
	else
	{
		demoState->n_timer = 0;
		demoState->dt_timer = demoState->timer_display->totalTime;
		demoState->dt_timer_tot = 0.0;
		demoState->t_timer = demoState->timer_display->totalTime;
	}

	// main idle loop
	a3demo_input(demoState, dt);
	a3demo_update(demoState, dt);
	a3demo_render(demoState, dt);

	// update input
	a3mouseUpdate(demoState->mouse);
	a3keyboardUpdate(demoState->keyboard);
	a3XboxControlUpdate(demoState->xcontrol);

	// extra controls
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_up))
			a3demoCtrlIncLoop(demoState->textMode, demoState_text_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_down))
			a3demoCtrlDecLoop(demoState->textMode, demoState_text_max);

		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_back))
		{
			if (!a3textIsInitialized(demoState->text))
			{
				a3demo_initializeText(demoState);
				demoState->textInit = a3true;
			}
			else
			{
				a3demo_releaseText(demoState);
				demoState->textInit = a3false;
			}
		}

		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_start))
		{
			a3demo_unloadShaders(demoState);
			a3demo_loadShaders(demoState);
		}
	}
}


//-----------------------------------------------------------------------------
// callback implementations

// demo is loaded
A3DYLIBSYMBOL a3_DemoState *a3demoCB_load(a3_DemoState *demoState, a3boolean hotbuild)
{
	a3ui32 const stateSize = a3demo_getPersistentStateSize();
	a3ui32 const trigSamplesPerDegree = 4;
	
	// do any re-allocation tasks
	if (demoState && hotbuild)
	{
		// example 1: copy memory directly
		a3_DemoState *demoState_copy = (a3_DemoState*)malloc(stateSize);
		if (demoState_copy)
		{
			// copy and release
			*demoState_copy = *demoState;
			free(demoState);
			demoState = demoState_copy;
		}
		
		// reset pointers
		a3trigInitSetTables(trigSamplesPerDegree, demoState->trigTable);

		// call refresh to re-link pointers in case demo state address changed
		a3demo_loadValidate(demoState);
		a3demoMode_loadValidate(demoState);
	}

	// do any initial allocation tasks
	// HEAP allocate persistent state
	// stack object will be deleted at the end of the function
	// good idea to set the whole block of memory to zero
	else if (demoState = (a3_DemoState*)malloc(stateSize))
	{
		// reset state
		memset(demoState, 0, stateSize);

		// set up trig table (A3DM)
		a3trigInit(trigSamplesPerDegree, demoState->trigTable);

		// text
		a3demo_initializeText(demoState);
		demoState->textInit = a3true;
		demoState->textMode = demoState_textControls;

		// enable asset streaming between loads
		demoState->streaming = a3true;

		// create directory for data
		a3fileStreamMakeDirectory("./data");

		// set default GL state
		a3demo_setDefaultGraphicsState();

		// demo modes
		a3demoMode_loadValidate(demoState);
		a3demo_load(demoState);

		// initialize state variables
		// e.g. timer, thread, etc.
		a3timerSet(demoState->timer_display, 30.0);
		a3timerStart(demoState->timer_display);
	}

	// return persistent state pointer
	return demoState;
}

// demo is unloaded; option to unload to prep for hotbuild
A3DYLIBSYMBOL a3_DemoState *a3demoCB_unload(a3_DemoState *demoState, a3boolean hotbuild)
{
	// release things that need releasing always, whether hotbuilding or not
	// e.g. kill thread
	// nothing in this example, but then...

	// release persistent state if not hotbuilding
	// good idea to release in reverse order that things were loaded...
	//	...normally; however, in this case there are reference counters 
	//	to make sure things get deleted when they need to, so there is 
	//	no need to reverse!
	if (demoState)
	{
		if (!hotbuild)
		{
			// free fixed objects
			a3demo_releaseText(demoState);

			// free graphics objects
			a3demo_unload(demoState);

			// validate unload
			a3demoMode_unloadValidate(demoState);
			a3demo_unloadValidate(demoState);

			// erase other stuff
			a3trigFree();

			// erase persistent state
			free(demoState);
			demoState = 0;
		}
	}

	// return state pointer
	return demoState;
}

// window updates display
// **NOTE: DO NOT USE FOR RENDERING**
A3DYLIBSYMBOL a3i32 a3demoCB_display(a3_DemoState *demoState)
{
	// do nothing, should just return 1 to indicate that the 
	//	window's display area is controlled by the demo
	return 1;
}

// window idles
A3DYLIBSYMBOL a3i32 a3demoCB_idle(a3_DemoState *demoState)
{
	// perform any idle tasks, such as rendering
	if (!demoState->exitFlag)
	{
		if (a3timerUpdate(demoState->timer_display) > 0)
		{
			// render timer ticked, update demo state and draw
			a3f64 const dt = demoState->timer_display->secondsPerTick;
			a3demo_idle(demoState, dt);

			// render occurred this idle: return +1
			return +1;
		}

		// nothing happened this idle: return 0
		return 0;
	}

	// demo should exit now: return -1
	return -1;
}

// window gains focus
A3DYLIBSYMBOL void a3demoCB_windowActivate(a3_DemoState *demoState)
{
	// nothing really needs to be done here...
	//	but it's here just in case
}

// window loses focus
A3DYLIBSYMBOL void a3demoCB_windowDeactivate(a3_DemoState *demoState)
{
	// reset input; it won't track events if the window is inactive, 
	//	active controls will freeze and you'll get strange behaviors
	a3keyboardReset(demoState->keyboard);
	a3mouseReset(demoState->mouse);
	a3XboxControlReset(demoState->xcontrol);
	a3XboxControlSetRumble(demoState->xcontrol, 0, 0);
}

// window moves
A3DYLIBSYMBOL void a3demoCB_windowMove(a3_DemoState *demoState, a3i32 newWindowPosX, a3i32 newWindowPosY)
{
	// nothing needed here
}

// window resizes
A3DYLIBSYMBOL void a3demoCB_windowResize(a3_DemoState *demoState, a3i32 newWindowWidth, a3i32 newWindowHeight)
{
	// account for borders here
	const a3i32 frameBorder = 0;
	const a3ui32 frameWidth = newWindowWidth + frameBorder + frameBorder;
	const a3ui32 frameHeight = newWindowHeight + frameBorder + frameBorder;
	const a3real windowAspect = (a3real)newWindowWidth / (a3real)newWindowHeight;
	const a3real frameAspect = (a3real)frameWidth / (a3real)frameHeight;

	// copy new values to demo state
	demoState->windowWidth = newWindowWidth;
	demoState->windowHeight = newWindowHeight;
	demoState->windowWidthInv = a3recip((a3real)newWindowWidth);
	demoState->windowHeightInv = a3recip((a3real)newWindowHeight);
	demoState->windowAspect = windowAspect;
	demoState->frameWidth = frameWidth;
	demoState->frameHeight = frameHeight;
	demoState->frameWidthInv = a3recip((a3real)frameWidth);
	demoState->frameHeightInv = a3recip((a3real)frameHeight);
	demoState->frameAspect = frameAspect;
	demoState->frameBorder = frameBorder;

	// framebuffers should be initialized or re-initialized here 
	//	since they are likely dependent on the window size
	a3demo_unloadFramebuffers(demoState);
	a3demo_loadFramebuffers(demoState);

	// use framebuffer deactivate utility to set viewport
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable, -frameBorder, -frameBorder, demoState->frameWidth, demoState->frameHeight);

	// viewing info for projection matrix
	a3demoMode_loadValidate(demoState);
}

// any key is pressed
A3DYLIBSYMBOL void a3demoCB_keyPress(a3_DemoState *demoState, a3i32 virtualKey)
{
	// persistent state update
	a3keyboardSetState(demoState->keyboard, (a3_KeyboardKey)virtualKey, a3input_down);
}

// any key is held
A3DYLIBSYMBOL void a3demoCB_keyHold(a3_DemoState *demoState, a3i32 virtualKey)
{
	// persistent state update
	a3keyboardSetState(demoState->keyboard, (a3_KeyboardKey)virtualKey, a3input_down);
}

// any key is released
A3DYLIBSYMBOL void a3demoCB_keyRelease(a3_DemoState *demoState, a3i32 virtualKey)
{
	// persistent state update
	a3keyboardSetState(demoState->keyboard, (a3_KeyboardKey)virtualKey, a3input_up);
}

// ASCII key is pressed (immediately preceded by "any key" pressed call above)
// NOTE: there is no release counterpart
A3DYLIBSYMBOL void a3demoCB_keyCharPress(a3_DemoState *demoState, a3i32 asciiKey)
{
	// persistent state update
	a3keyboardSetStateASCII(demoState->keyboard, (a3byte)asciiKey);

	// handle special cases immediately
	switch (asciiKey)
	{
		// uncomment to make escape key kill the current demo
		// if disabled, use 'exit demo' menu option
//	case 27: 
//		demoState->exitFlag = 1;
//		break;


		// toggle text mode
		a3demoCtrlCaseIncLoop(demoState->textMode, demoState_text_max, 't');

		// reload text
	case 'T':
		if (!a3textIsInitialized(demoState->text))
		{
			a3demo_initializeText(demoState);
			demoState->textInit = a3true;
		}
		else
		{
			a3demo_releaseText(demoState);
			demoState->textInit = a3false;
		}
		break;


		// reload all shaders in real-time
	case 'P':
		a3demo_unloadShaders(demoState);
		a3demo_loadShaders(demoState);
		break;
	}


	// demo callback
	a3demo_input_keyCharPress(demoState, asciiKey);
}

// ASCII key is held
A3DYLIBSYMBOL void a3demoCB_keyCharHold(a3_DemoState *demoState, a3i32 asciiKey)
{
	// persistent state update
	a3keyboardSetStateASCII(demoState->keyboard, (a3byte)asciiKey);

	// handle special cases immediately
//	switch (asciiKey)
//	{
//
//	}


	// demo callback
	a3demo_input_keyCharHold(demoState, asciiKey);
}

// mouse button is clicked
A3DYLIBSYMBOL void a3demoCB_mouseClick(a3_DemoState *demoState, a3i32 button, a3i32 cursorX, a3i32 cursorY)
{
	// persistent state update
	a3mouseSetState(demoState->mouse, (a3_MouseButton)button, a3input_down);
	a3mouseSetPosition(demoState->mouse, cursorX, cursorY);
}

// mouse button is double-clicked
A3DYLIBSYMBOL void a3demoCB_mouseDoubleClick(a3_DemoState *demoState, a3i32 button, a3i32 cursorX, a3i32 cursorY)
{
	// persistent state update
	a3mouseSetState(demoState->mouse, (a3_MouseButton)button, a3input_down);
	a3mouseSetPosition(demoState->mouse, cursorX, cursorY);
}

// mouse button is released
A3DYLIBSYMBOL void a3demoCB_mouseRelease(a3_DemoState *demoState, a3i32 button, a3i32 cursorX, a3i32 cursorY)
{
	// persistent state update
	a3mouseSetState(demoState->mouse, (a3_MouseButton)button, a3input_up);
	a3mouseSetPosition(demoState->mouse, cursorX, cursorY);
}

// mouse wheel is turned
A3DYLIBSYMBOL void a3demoCB_mouseWheel(a3_DemoState *demoState, a3i32 delta, a3i32 cursorX, a3i32 cursorY)
{
	// persistent state update
	a3mouseSetStateWheel(demoState->mouse, (a3_MouseWheelState)delta);
	a3mouseSetPosition(demoState->mouse, cursorX, cursorY);
}

// mouse moves
A3DYLIBSYMBOL void a3demoCB_mouseMove(a3_DemoState *demoState, a3i32 cursorX, a3i32 cursorY)
{
	// persistent state update
	a3mouseSetPosition(demoState->mouse, cursorX, cursorY);
}

// mouse leaves window
A3DYLIBSYMBOL void a3demoCB_mouseLeave(a3_DemoState *demoState)
{
	// reset mouse state or any buttons pressed will freeze
	a3mouseReset(demoState->mouse);
}


//-----------------------------------------------------------------------------
