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
	
	a3_Framebuffer.h
	Interface to wrap OpenGL's framebuffer object (FBO) for off-screen drawing.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_FRAMEBUFFER_H
#define __ANIMAL3D_FRAMEBUFFER_H


#include "animal3D/a3/a3types_integer.h"
#include "a3_Texture.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Framebuffer			a3_Framebuffer;
	typedef struct a3_FramebufferDouble		a3_FramebufferDouble;
	typedef enum a3_FramebufferColorType	a3_FramebufferColorType;
	typedef enum a3_FramebufferDepthType	a3_FramebufferDepthType;
#endif	// __cplusplus

//-----------------------------------------------------------------------------

	// A3: Absolute maximum number of color targets allowed.
	enum a3_FramebufferColorTargetMax
	{
		a3fbo_colorTargetMax = 16
	};


	// A3: Framebuffer structure used for off-screen rendering.
	//	member handle: graphics object handle
	//	member colorTextureHandle: list of graphics handles for color targets
	//	member depthTextureHandle: graphics handle for depth target
	//	members frameWidth, frameHeight: dimensions of frame buffer in pixels
	//	member color: number of color targets attached to this framebuffer
	//	member depthStencil: depth buffer format
	struct a3_Framebuffer
	{
		struct {
			a3_GraphicsObjectHandle handle[1];
			a3ui32 colorTextureHandle[a3fbo_colorTargetMax];
			a3ui32 depthTextureHandle[1];
		};
		a3ui16 frameWidth, frameHeight;
		a3ui16 color, depthStencil;
	};

	// A3: Paired framebuffers for off-screen double-buffering.
	//	member handle: graphics object handle
	//	member handleDouble: graphics object handle for double buffer
	//	member colorTextureHandle: list of graphics handles for color targets
	//	member depthTextureHandle: graphics handles for depth targets
	//	members frontColor, frontDepth: color texture offsets for front buffer
	//	members frameWidth, frameHeight: dimensions of frame buffers in pixels
	//	member color: number of color targets attached to each framebuffer
	//	member depthStencil: depth buffer format
	struct a3_FramebufferDouble
	{
		struct {
			a3_GraphicsObjectHandle handle[1];
			a3ui32 handleDouble;
			a3ui32 colorTextureHandle[a3fbo_colorTargetMax*2];
			a3ui32 depthTextureHandle[2];
		};
		a3ui16 frontColor, frontDepth;
		a3ui16 frameWidth, frameHeight;
		a3ui16 color, depthStencil;
	};


	// A3: High-level descriptor of color format for framebuffers.
	enum a3_FramebufferColorType
	{
		a3fbo_colorDisable,				// no color targets attached
		a3fbo_colorRGB8 = a3tex_rgb8,	// 3 channels, 8 bits each (byte)
		a3fbo_colorRGB16,				// 3 channels, 16 bits each (short)
		a3fbo_colorRGB32F,				// 3 channels, 32 bits each (float)
		a3fbo_colorRGBA8,				// 4 channels, 8 bits each
		a3fbo_colorRGBA16,				// 4 channels, 16 bits each
		a3fbo_colorRGBA32F,				// 4 channels, 32 bits each (float)
	};

	// A3: High-level descriptor of depth format for framebuffers.
	enum a3_FramebufferDepthType
	{
		a3fbo_depthDisable,				// no depth target
		a3fbo_depth16 = a3tex_depth16,	// 16-bit depth (65536 possible values)
		a3fbo_depth24,					// 24-bit depth (16 million values)
		a3fbo_depth32,					// 32-bit depth (4 billion values)
		a3fbo_depth24_stencil8,			// 24-bit depth with 8-bit stencil
	};


//-----------------------------------------------------------------------------

	// A3: Create framebuffer.
	//	param framebuffer_out: non-null pointer to uninitialized framebuffer
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param colorTargets: number of color targets (relevant if colorType is 
	//		not 'disable')
	//	param colorType: color type to use for render targets (relevant if 
	//		colorTargets is not zero)
	//	param depthType: depth type to use
	//	params frameWidth, frameHeight: positive framebuffer dimensions
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params (null pointer or already initialized)
	a3ret a3framebufferCreate(a3_Framebuffer *framebuffer_out, const a3byte name_opt[32], a3ui32 colorTargets, const a3_FramebufferColorType colorType, const a3_FramebufferDepthType depthType, const a3ui16 frameWidth, const a3ui16 frameHeight);

	// A3: Activate framebuffer for rendering.
	//	param framebuffer: non-null pointer to initialized framebuffer; pass 
	//		null or uninitialized to deactivate
	//	return: 1 if buffer was activated
	//	return: 0 if deactivated
	a3ret a3framebufferActivate(const a3_Framebuffer *framebuffer);

	// A3: Deactivate any active framebuffer (also applies to double-buffer).
	//	return: 0
	a3ret a3framebufferDeactivate();

	// A3: Deactivate any active framebuffer with option to set new viewport.
	//	param depthType: depth type of back buffer
	//	params viewportPosX, viewportPosY: the origin of the new viewport
	//	params viewportWidth, viewportHeight: the size of the new viewport
	//	return: 0
	a3ret a3framebufferDeactivateSetViewport(const a3_FramebufferDepthType depthType, const a3i32 viewportPosX, const a3i32 viewportPosY, const a3ui32 viewportWidth, const a3ui32 viewportHeight);

	// A3: Use framebuffer's color target as texture when deactivated.
	//	param framebuffer: non-null pointer to initialized framebuffer
	//	param unit: which texture unit or "slot" to bind to
	//	param colorTarget: index of color target; must be less than attached
	//	return: 1 if success
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferBindColorTexture(const a3_Framebuffer *framebuffer, const a3_TextureUnit unit, const a3ui32 colorTarget);

	// A3: Use framebuffer's depth target as texture when deactivated.
	//	param framebuffer: non-null pointer to initialized framebuffer
	//	param unit: which texture unit to bind to
	//	return: 1 if success
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferBindDepthTexture(const a3_Framebuffer *framebuffer, const a3_TextureUnit unit);

	// A3: Reference framebuffer.
	//	param framebuffer: non-null pointer to initialized framebuffer
	//	return: new reference count if success
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferReference(a3_Framebuffer *framebuffer);

	// A3: Release framebuffer.
	//	param framebuffer: non-null pointer to initialized framebuffer
	//	return: non-negative if success
	//	return: 0 if object was permanently released; descriptor is reset
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferRelease(a3_Framebuffer *framebuffer);

	// A3: Set graphics handle release function for framebuffer 
	//		(hotload quick-fix).
	//	param framebuffer: non-null pointer to framebuffer to update
	//	return: 1 if updated
	//	return: -1 if invalid param
	a3ret a3framebufferHandleUpdateReleaseCallback(a3_Framebuffer *framebuffer);


//-----------------------------------------------------------------------------

	// A3: Create double framebuffer.
	//	param framebufferDouble_out: non-null pointer to uninitialized double-
	//		buffering framebuffer
	//	param name_opt: optional cstring for short name/description; max 31 
	//		chars + null terminator; pass null for default name
	//	param colorTargets: number of color targets (relevant if colorType is 
	//		not 'disable')
	//	param colorType: color type to use for render targets (relevant if 
	//		colorTargets is not zero)
	//	param depthType: depth type to use
	//	params frameWidth, frameHeight: positive framebuffer dimensions
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params (null pointer or already initialized)
	a3ret a3framebufferDoubleCreate(a3_FramebufferDouble *framebufferDouble_out, const a3byte name_opt[32], a3ui32 colorTargets, const a3_FramebufferColorType colorType, const a3_FramebufferDepthType depthType, const a3ui16 frameWidth, const a3ui16 frameHeight);

	// A3: Activate double-buffering framebuffer for rendering; the back 
	//		buffer is used for drawing.
	//	param framebufferDouble: non-null pointer to initialized framebuffer; 
	//		pass null or uninitialized to deactivate
	//	return: 1 if buffer was activated
	//	return: 0 if deactivated
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferDoubleActivate(const a3_FramebufferDouble *framebufferDouble);

	// A3: Use front buffer's color target as texture when deactivated.
	//	param framebufferDouble: non-null pointer to initialized framebuffer
	//	param colorTarget: index of color target; must be less than attached
	//	param textureUnit: which texture unit to bind to
	//	return: 1 if success
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferDoubleBindColorTexture(const a3_FramebufferDouble *framebufferDouble, const a3_TextureUnit unit, const a3ui32 colorTarget);

	// A3: Use front buffer's depth target as texture when deactivated.
	//	param framebuffer: non-null pointer to initialized framebuffer
	//	param textureUnit: which texture unit to bind to
	//	return: 1 if success
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferDoubleBindDepthTexture(const a3_FramebufferDouble *framebufferDouble, const a3_TextureUnit unit);

	// A3: Swap double-buffering framebuffers: front becomes backbuffer which 
	//		will be targeted for drawing, back becomes front.
	//	param framebufferDouble: non-null pointer to initialized framebuffer
	//	return: 1 if success
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferDoubleSwap(a3_FramebufferDouble *framebufferDouble);

	// A3: Reference double-buffering framebuffer.
	//	param framebufferDouble: non-null pointer to initialized framebuffer
	//	return: new reference count if success
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferDoubleReference(a3_FramebufferDouble *framebufferDouble);

	// A3: Release double-buffering framebuffer.
	//	param framebufferDouble: non-null pointer to initialized framebuffer
	//	return: non-negative if success
	//	return: 0 if object was permanently released; descriptor is reset
	//	return: -1 if invalid params or not initialized
	a3ret a3framebufferDoubleRelease(a3_FramebufferDouble *framebufferDouble);

	// A3: Set graphics handle release for double-buffering framebuffer 
	//		(hotload quick-fix).
	//	param framebufferDouble: non-null pointer to framebuffer to update
	//	return: 1 if updated
	//	return: -1 if invalid param
	a3ret a3framebufferDoubleHandleUpdateReleaseCallback(a3_FramebufferDouble *framebufferDouble);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_Framebuffer.inl"


#endif	// !__ANIMAL3D_FRAMEBUFFER_H