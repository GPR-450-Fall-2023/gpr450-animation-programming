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
	
	a3_Framebuffer.c
	Definitions for common framebuffer object functions.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_Framebuffer.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// internal utility declarations

void a3framebufferInternalHandleReleaseFunc(a3i32 count, a3ui32 *handlePtr);
void a3framebufferDoubleInternalHandleReleaseFunc(a3i32 count, a3ui32 *handlePtr);
a3ui32 a3framebufferInternalValidateColorTargets(const a3ui32 colorTargets);
a3ui32 a3framebufferInternalCreate(a3ui32 *colorHandles, a3ui32 *depthHandle, const a3ui32 colorTargets, const a3_TexturePixelFormatDescriptor *colorPixelFormat, const a3_TexturePixelFormatDescriptor *depthPixelFormat, const a3ui16 width, const a3ui16 height);


//-----------------------------------------------------------------------------
// framebuffer

a3ret a3framebufferCreate(a3_Framebuffer *framebuffer_out, const a3byte name_opt[32], a3ui32 colorTargets, const a3_FramebufferColorType colorType, const a3_FramebufferDepthType depthType, const a3ui16 frameWidth, const a3ui16 frameHeight)
{
	a3_Framebuffer ret = { 0 };
	a3_TexturePixelFormatDescriptor colorPixelFormat[1] = { 0 }, depthPixelFormat[1] = { 0 }, *colorPixelFormatPtr = 0, *depthPixelFormatPtr = 0;
	a3ui32 handle, numHandles;

	// validate params
	if (framebuffer_out && frameWidth && frameHeight)
	{
		// validate unused
		if (!framebuffer_out->handle->handle)
		{
			// get pixel formats
			if (colorType && colorTargets)
			{
				colorTargets = a3framebufferInternalValidateColorTargets(colorTargets);
				a3textureCreatePixelFormatDescriptor(colorPixelFormat, (a3_TexturePixelType)colorType);
				colorPixelFormatPtr = colorPixelFormat;
			}
			if (depthType)
			{
				a3textureCreatePixelFormatDescriptor(depthPixelFormat, (a3_TexturePixelType)depthType);
				depthPixelFormatPtr = depthPixelFormat;
			}

			// prepare FBO
			handle = a3framebufferInternalCreate(ret.colorTextureHandle, ret.depthTextureHandle, 
				colorTargets, colorPixelFormatPtr, depthPixelFormatPtr, frameWidth, frameHeight);
			if (handle)
			{
				// initialize graphics object, copy data and return
				numHandles = (2 + a3fbo_colorTargetMax);
				a3handleCreateHandle(ret.handle, a3framebufferInternalHandleReleaseFunc, name_opt, handle, numHandles);
				ret.color = (colorType != a3fbo_colorDisable) * colorTargets;
				ret.depthStencil = depthType;
				ret.frameWidth = frameWidth;
				ret.frameHeight = frameHeight;

				// done
				*framebuffer_out = ret;
				a3framebufferReference(framebuffer_out);
				return 1;
			}
			else
				printf("\n A3 ERROR (FBO \'%s\'): \n\t Invalid handle; framebuffer not created.", name_opt);

			// fail
			return 0;
		}
	}
	return -1;
}


a3ret a3framebufferHandleUpdateReleaseCallback(a3_Framebuffer *framebuffer)
{
	if (framebuffer)
		return a3handleSetReleaseFunc(framebuffer->handle, a3framebufferInternalHandleReleaseFunc);
	return -1;
}


//-----------------------------------------------------------------------------
// framebuffer double-buffer

a3ret a3framebufferDoubleCreate(a3_FramebufferDouble *framebufferDouble_out, const a3byte name_opt[32], a3ui32 colorTargets, const a3_FramebufferColorType colorType, const a3_FramebufferDepthType depthType, const a3ui16 frameWidth, const a3ui16 frameHeight)
{
	a3_FramebufferDouble ret = { 0 };
	a3_TexturePixelFormatDescriptor colorPixelFormat[1] = { 0 }, depthPixelFormat[1] = { 0 }, *colorPixelFormatPtr = 0, *depthPixelFormatPtr = 0;
	a3ui32 handleBack, handleFront, numHandles;

	// validate params
	if (framebufferDouble_out && frameWidth && frameHeight)
	{
		// validate unused
		if (!framebufferDouble_out->handle->handle)
		{
			// get pixel formats
			if (colorType && colorTargets)
			{
				colorTargets = a3framebufferInternalValidateColorTargets(colorTargets);
				a3textureCreatePixelFormatDescriptor(colorPixelFormat, (a3_TexturePixelType)colorType);
				colorPixelFormatPtr = colorPixelFormat;
			}
			if (depthType)
			{
				a3textureCreatePixelFormatDescriptor(depthPixelFormat, (a3_TexturePixelType)depthType);
				depthPixelFormatPtr = depthPixelFormat;
			}

			// prepare 2 FBOs
			handleBack = a3framebufferInternalCreate(ret.colorTextureHandle, ret.depthTextureHandle,
				colorTargets, colorPixelFormatPtr, depthPixelFormatPtr, frameWidth, frameHeight);
			handleFront = a3framebufferInternalCreate(ret.colorTextureHandle + a3fbo_colorTargetMax, ret.depthTextureHandle + 1,
				colorTargets, colorPixelFormatPtr, depthPixelFormatPtr, frameWidth, frameHeight);
			if (handleBack && handleFront)
			{
				// init
				numHandles = (2 + a3fbo_colorTargetMax) * 2;
				a3handleCreateHandle(ret.handle, a3framebufferDoubleInternalHandleReleaseFunc, name_opt, handleBack, numHandles);
				ret.handleDouble = handleFront;
				ret.color = (colorType != a3fbo_colorDisable) * colorTargets;
				ret.depthStencil = depthType;
				ret.frameWidth = frameWidth;
				ret.frameHeight = frameHeight;

				// set back and front
				ret.frontColor = a3fbo_colorTargetMax;
				ret.frontDepth = 1;

				// done
				*framebufferDouble_out = ret;
				a3framebufferDoubleReference(framebufferDouble_out);
				return 1;
			}
			else
				printf("\n A3 ERROR (DFBO \'%s\'): \n\t Invalid handle; double framebuffer not created.", name_opt);

			// fail
			return 0;
		}
	}
	return -1;
}


a3ret a3framebufferDoubleHandleUpdateReleaseCallback(a3_FramebufferDouble *framebufferDouble)
{
	if (framebufferDouble)
		return a3handleSetReleaseFunc(framebufferDouble->handle, a3framebufferDoubleInternalHandleReleaseFunc);
	return -1;
}


//-----------------------------------------------------------------------------
