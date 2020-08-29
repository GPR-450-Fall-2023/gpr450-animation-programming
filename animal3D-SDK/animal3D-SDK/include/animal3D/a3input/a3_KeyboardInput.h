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
	
	a3_KeyboardInput.h
	Basic keyboard input state tracking interface.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_KEYBOARDINPUT_H
#define __ANIMAL3D_KEYBOARDINPUT_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_InputState.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_KeyboardInput		a3_KeyboardInput;
	typedef enum a3_KeyboardKey			a3_KeyboardKey;
	typedef enum a3_KeyboardKeyASCII	a3_KeyboardKeyASCII;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Keyboard state tracker.
	//	members key, key0: current and previous key states, respectively
	//	members keyASCII, keyASCII0: current and previous ASCII key states
	//		respectively; these can only be set to press, will automatically 
	//		set to released on update
	struct a3_KeyboardInput
	{
		struct {
			a3byte key[256];
		} key, key0;

		struct {
			a3byte key[128];
		} keyASCII, keyASCII0;
	};


	// A3: Enumeration of virtual keys: these are not ASCII characters, they 
	//		are the codes for the actual keys; for special ASCII characters 
	//		codes, see below.
	enum a3_KeyboardKey
	{
		a3key_null,
		a3key_backspace = 8,
		a3key_tab,
		a3key_clear = 12,
		a3key_return,
		a3key_enter = a3key_return,
		a3key_shift = 16,
		a3key_control,
		a3key_alt,			// not processed
		a3key_pause,
		a3key_capsLock,
		a3key_escape = 27,
		a3key_space = 32,
		a3key_pageup,
		a3key_pagedown,
		a3key_end,
		a3key_home,
		a3key_leftArrow,
		a3key_upArrow,
		a3key_rightArrow,
		a3key_downArrow,
		a3key_select,
		a3key_print,
		a3key_execute,
		a3key_printScreen,	// processed only with release callback
		a3key_insert,
		a3key_delete,
		a3key_help,
		a3key_0,
		a3key_1,
		a3key_2,
		a3key_3,
		a3key_4,
		a3key_5,
		a3key_6,
		a3key_7,
		a3key_8,
		a3key_9,
		a3key_rightParenthesis = a3key_0,
		a3key_exclamation,
		a3key_at,
		a3key_number,
		a3key_dollar,
		a3key_percent,
		a3key_caret,
		a3key_ampersand,
		a3key_asterisk,
		a3key_leftParenthesis,
		a3key_A = 65,
		a3key_B,
		a3key_C,
		a3key_D,
		a3key_E,
		a3key_F,
		a3key_G,
		a3key_H,
		a3key_I,
		a3key_J,
		a3key_K,
		a3key_L,
		a3key_M,
		a3key_N,
		a3key_O,
		a3key_P,
		a3key_Q,
		a3key_R,
		a3key_S,
		a3key_T,
		a3key_U,
		a3key_V,
		a3key_W,
		a3key_X,
		a3key_Y,
		a3key_Z,
		a3key_leftCommand_window,
		a3key_rightCommand_window,
		a3key_appsMenu,
		a3key_sleep = 95,
		a3key_numpad0,
		a3key_numpad1,
		a3key_numpad2,
		a3key_numpad3,
		a3key_numpad4,
		a3key_numpad5,
		a3key_numpad6,
		a3key_numpad7,
		a3key_numpad8,
		a3key_numpad9,
		a3key_numpadTimes,
		a3key_numpadPlus,
		a3key_numpadSeparator,
		a3key_numpadMinus,
		a3key_numpadDecimal,
		a3key_numpadDivide,
		a3key_F1,
		a3key_F2,
		a3key_F3,
		a3key_F4,
		a3key_F5,
		a3key_F6,
		a3key_F7,
		a3key_F8,
		a3key_F9,
		a3key_F10,			// not processed
		a3key_F11,
		a3key_F12,
		a3key_F13,
		a3key_F14,
		a3key_F15,
		a3key_F16,
		a3key_F17,
		a3key_F18,
		a3key_F19,
		a3key_F20,
		a3key_F21,
		a3key_F22,
		a3key_F23,
		a3key_F24,
		a3key_numLock = 144,
		a3key_scrollLock,
		a3key_numpadEqual,
		a3key_leftShift = 160,	// not processed with callbacks
		a3key_rightShift,		// " 
		a3key_leftControl,		// "
		a3key_rightControl,		// "
		a3key_leftMenu,			// "
		a3key_rightMenu,		// "
		a3key_browserBack,
		a3key_browserForward,
		a3key_browserRefresh,
		a3key_browserStop,
		a3key_browserSearch,
		a3key_browserFavorites,
		a3key_browserHome,
		a3key_mediaMute,
		a3key_mediaVolumeDown,
		a3key_mediaVolumeUp,
		a3key_mediaNext,
		a3key_mediaPrevious,
		a3key_mediaStop,
		a3key_mediaPlayPause,
		a3key_launchEmail,
		a3key_launchMedia,
		a3key_launchApp1,
		a3key_launchApp2,
		a3key_semicolon = 186,
		a3key_equal,
		a3key_comma,
		a3key_dash,
		a3key_period,
		a3key_slash,
		a3key_backquote,
		a3key_minus = a3key_dash,
		a3key_colon = a3key_semicolon,
		a3key_plus,
		a3key_lessthan,
		a3key_underscore,
		a3key_greaterthan,
		a3key_question,
		a3key_tilde,
		a3key_leftBracket = 219,
		a3key_backslash,
		a3key_rightBracket,
		a3key_apostrophe,
		a3key_singleQuote = a3key_apostrophe,
		a3key_leftBrace = a3key_leftBracket,
		a3key_pipe,
		a3key_rightBrace,
		a3key_quote,
		a3key_doubleQuote = a3key_quote,
	};

	
	// A3: Enumeration of ASCII keys: these are for the special keys that are 
	//		accompanied by ASCII codes and will respond to the keyCharPressed 
	//		callback; if key can be represented by a character, use a literal 
	//		character (e.g. 'a') with the keyChar callback; otherwise, use one 
	//		of these codes to check the state of an ascii key.
	enum a3_KeyboardKeyASCII
	{
		a3ascii_null,
		a3ascii_ctrl_A,				// ctrl + alpha
		a3ascii_ctrl_B,
		a3ascii_ctrl_C,
		a3ascii_ctrl_D,
		a3ascii_ctrl_E,
		a3ascii_ctrl_F,
		a3ascii_ctrl_G,
		a3ascii_ctrl_H,
		a3ascii_ctrl_I,
		a3ascii_ctrl_J,
		a3ascii_ctrl_K,
		a3ascii_ctrl_L,
		a3ascii_ctrl_M,
		a3ascii_ctrl_N,
		a3ascii_ctrl_O,
		a3ascii_ctrl_P,
		a3ascii_ctrl_Q,
		a3ascii_ctrl_R,
		a3ascii_ctrl_S,
		a3ascii_ctrl_T,
		a3ascii_ctrl_U,
		a3ascii_ctrl_V,
		a3ascii_ctrl_W,
		a3ascii_ctrl_X,
		a3ascii_ctrl_Y,
		a3ascii_ctrl_Z,
		a3ascii_ctrl_leftBracket,	// ctrl + '['
		a3ascii_ctrl_backslash,		// ctrl + '\'
		a3ascii_ctrl_rightBracket,	// ctrl + ']'
		a3ascii_ctrl_pause = 3,
		a3ascii_ctrl_scrollLock = 3,
		a3ascii_alert = 7,			// '\a'
		a3ascii_backspace,			// '\b'
		a3ascii_tab,				// '\t'
		a3ascii_tab_horizontal = a3ascii_tab,
		a3ascii_newline,			// '\n', ctrl + enter
		a3ascii_ctrl_enter = a3ascii_newline,
		a3ascii_tab_vertical,		// '\v'
		a3ascii_formfeed,			// '\f'
		a3ascii_return,				// '\r'
		a3ascii_enter = a3ascii_return,
		a3ascii_escape = 27,		// '\e' sometimes available
		a3ascii_ctrl_backspace = 127,
	};


//-----------------------------------------------------------------------------

	// A3: Get the state of a key.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check the state of
	//	return:	1 if key is pressed
	//	return: 0 if key is not pressed
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardGetState(const a3_KeyboardInput *keyboard, const a3_KeyboardKey key);

	// A3: Check if key is held down from last update.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check
	//	return:	1 if key is down and was also down last update
	//	return: 0 if key is not down or has changed
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardIsHeld(const a3_KeyboardInput *keyboard, const a3_KeyboardKey key);

	// A3: Check if key was pressed or released since last update.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check
	//	return:	+1 if key changed from released to pressed
	//	return:	-1 if key changed from pressed to released
	//	return: 0 if key did not change
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardIsChanged(const a3_KeyboardInput *keyboard, const a3_KeyboardKey key);

	// A3: Check if key was pressed since last update.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check
	//	return:	1 if key was pressed
	//	return: 0 if key was not pressed
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardIsPressed(const a3_KeyboardInput *keyboard, const a3_KeyboardKey key);

	// A3: Check if key was released since last update.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check
	//	return:	1 if key was released
	//	return: 0 if key was not released
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardIsReleased(const a3_KeyboardInput *keyboard, const a3_KeyboardKey key);

	// A3: Get the state of an ASCII key.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check the state of
	//	return:	1 if key is pressed
	//	return: 0 if key is not pressed
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardGetStateASCII(const a3_KeyboardInput *keyboard, const a3byte key);

	// A3: Check if ASCII key is held down from last update.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check
	//	return:	1 if key is down and was also down last update
	//	return: 0 if key is not down or has changed
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardIsHeldASCII(const a3_KeyboardInput *keyboard, const a3byte key);

	// A3: Check if key was pressed or released from last update.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to check
	//	return:	+1 if key changed from released to pressed
	//	return:	-1 if key changed from pressed to released
	//	return: 0 if key did not change
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardIsChangedASCII(const a3_KeyboardInput *keyboard, const a3byte key);

	// A3: Get difference between two key states.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param keyPositive: 'positive' state key
	//	param keyNegative: 'negative' state key
	//	return: +1 if positive key is down and negative key is up
	//	return: -1 if negative key is down and positive key is up
	//	return: 0 if both keys have the same state
	a3ret a3keyboardGetDifference(const a3_KeyboardInput *keyboard, const a3_KeyboardKey keyPositive, const a3_KeyboardKey keyNegative);

	// A3: Get difference between two ASCII key states.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param keyPositive: 'positive' state key
	//	param keyNegative: 'negative' state key
	//	return: +1 if positive key is down and negative key is up
	//	return: -1 if negative key is down and positive key is up
	//	return: 0 if both keys have the same state
	a3ret a3keyboardGetDifferenceASCII(const a3_KeyboardInput *keyboard, const a3byte keyPositive, const a3byte keyNegative);

	// A3: Set the state of a key.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to set
	//	param state: new state for key
	//	return:	new key state
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardSetState(a3_KeyboardInput *keyboard, const a3_KeyboardKey key, const a3_InputState state);

	// A3: Set the state of an ASCII key.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	param key: key to set
	//	param state: new state for key
	//	return:	new key state
	//	return: a3input_invalid if invalid params
	a3ret a3keyboardSetStateASCII(a3_KeyboardInput *keyboard, const a3byte key);

	// A3: Update keyboard state.
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	return:	1 if success
	//	return: a3input_invalid if invalid param
	a3ret a3keyboardUpdate(a3_KeyboardInput *keyboard);

	// A3: Reset current keyboard state
	//	param keyboard: non-null pointer to keyboard state descriptor
	//	return: 1 if success
	//	return: a3input_invalid if invalid param
	a3ret a3keyboardReset(a3_KeyboardInput *keyboard);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyboardInput.inl"


#endif	// !__ANIMAL3D_KEYBOARDINPUT_H