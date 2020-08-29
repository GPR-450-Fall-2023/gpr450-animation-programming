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
	
	a3_Framebuffer-OpenGL.c
	Definitions for OpenGL framebuffer object.

	**DO NOT MODIFY THIS FILE**
*/

#include "animal3D-A3DG/a3graphics/a3_Framebuffer.h"

#include <GL/glew.h>

#include <string.h>


//-----------------------------------------------------------------------------
// internal utilities

// release functions
void a3framebufferInternalHandleReleaseFunc(a3i32 count, a3ui32 *handlePtr)
{
	// first is framebuffer
	// the rest are textures
	glDeleteFramebuffers(1, handlePtr++);
	glDeleteTextures(count -= 1, handlePtr);
	memset(handlePtr, 0, count * sizeof(a3ui32));
}

void a3framebufferDoubleInternalHandleReleaseFunc(a3i32 count, a3ui32 *handlePtr)
{
	// first two are framebuffers
	// the rest are textures
	glDeleteFramebuffers(2, handlePtr++);
	*(handlePtr++) = 0;
	glDeleteTextures(count -= 2, handlePtr);
	memset(handlePtr, 0, count * sizeof(a3ui32));
}


// internal validate color target count
a3ui32 a3framebufferInternalValidateColorTargets(const a3ui32 colorTargets)
{
	a3ui32 query;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (a3i32 *)(&query));
	if (query > a3fbo_colorTargetMax) 
		query = a3fbo_colorTargetMax;
	return (colorTargets <= query ? colorTargets : query);
}

// internal creation function (returns new handle if success)
a3ui32 a3framebufferInternalCreate(a3ui32 *colorHandles, a3ui32 *depthHandle, const a3ui32 colorTargets, const a3_TexturePixelFormatDescriptor *colorPixelFormat, const a3_TexturePixelFormatDescriptor *depthPixelFormat, const a3ui16 width, const a3ui16 height)
{
	a3ui32 target;
	a3ui32 handle;

	// proceed if using color and/or depth
	if (colorPixelFormat || depthPixelFormat)
	{
		// create FBO
		glGenFramebuffers(1, &handle);
		if (handle)
		{
			// bind and configure FBO
			glBindFramebuffer(GL_FRAMEBUFFER, handle);

			// generate texture handles for color
			if (colorTargets && colorPixelFormat)
			{
				// configure all color textures
				glGenTextures(colorTargets, colorHandles);
				for (target = 0; target < colorTargets; ++target, ++colorHandles)
				{
					// bind and initialize texture with default settings
					glBindTexture(GL_TEXTURE_2D, *colorHandles);
					glTexImage2D(GL_TEXTURE_2D, 0, colorPixelFormat->internalFormatBits, width, height, 0, colorPixelFormat->internalFormat, colorPixelFormat->internalDataType, 0);
					a3textureDefaultSettings();

					// bind color texture to FBO as render target
					glFramebufferTexture2D(GL_FRAMEBUFFER, (GL_COLOR_ATTACHMENT0 + target), GL_TEXTURE_2D, *colorHandles, 0);
				}
			}

			// generate texture handle for depth
			if (depthPixelFormat)
			{
				// configure depth texture
				glGenTextures(1, depthHandle);
				glBindTexture(GL_TEXTURE_2D, *depthHandle);
				glTexImage2D(GL_TEXTURE_2D, 0, depthPixelFormat->internalFormatBits, width, height, 0, depthPixelFormat->internalFormat, depthPixelFormat->internalDataType, 0);
				a3textureDefaultSettings();

				// bind depth texture to FBO
				target = (depthPixelFormat->internalFormat == GL_DEPTH_STENCIL ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT);
				glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, *depthHandle, 0);
			}

			// done, deactivate and return
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			return handle;
		}
	}
	return 0;
}


// internal select draw buffers
void a3framebufferInternalActivate(const a3ui16 color, const a3ui16 depthStencil)
{
	static const a3ui32 drawBuffers[] = {
		GL_COLOR_ATTACHMENT0,	GL_COLOR_ATTACHMENT1,	GL_COLOR_ATTACHMENT2,	GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,	GL_COLOR_ATTACHMENT5,	GL_COLOR_ATTACHMENT6,	GL_COLOR_ATTACHMENT7,
		GL_COLOR_ATTACHMENT8,	GL_COLOR_ATTACHMENT9,	GL_COLOR_ATTACHMENT10,	GL_COLOR_ATTACHMENT11,
		GL_COLOR_ATTACHMENT12,	GL_COLOR_ATTACHMENT13,	GL_COLOR_ATTACHMENT14,	GL_COLOR_ATTACHMENT15,
	};

	// change color targets
	if (color)
		glDrawBuffers(color, drawBuffers);
	else
		glDrawBuffer(GL_NONE);

	// change depth test
	if (depthStencil)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	// change stencil test
	if (depthStencil == a3fbo_depth24_stencil8)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);
}


//-----------------------------------------------------------------------------
// framebuffer

a3ret a3framebufferActivate(const a3_Framebuffer *framebuffer)
{
	// validate
	if (framebuffer && framebuffer->handle->handle)
	{
		// activate
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->handle->handle);
		a3framebufferInternalActivate(framebuffer->color, framebuffer->depthStencil);

		// set viewport
		glViewport(0, 0, framebuffer->frameWidth, framebuffer->frameHeight);

		// done
		return 1;
	}

	// deactivate
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	return 0;
}

a3ret a3framebufferDeactivate()
{
	// disable
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	return 0;
}

a3ret a3framebufferDeactivateSetViewport(const a3_FramebufferDepthType depthType, const a3i32 viewportPosX, const a3i32 viewportPosY, const a3ui32 viewportWidth, const a3ui32 viewportHeight)
{
	// disable
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	// change depth tests
	if (depthType)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	if (depthType == a3fbo_depth24_stencil8)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);

	// change viewport
	glViewport(viewportPosX, viewportPosY, viewportWidth, viewportHeight);

	// done
	return 0;
}

a3ret a3framebufferBindColorTexture(const a3_Framebuffer *framebuffer, const a3_TextureUnit unit, const a3ui32 colorTarget)
{
	// validate
	if (framebuffer && framebuffer->handle->handle && colorTarget < framebuffer->color)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, framebuffer->colorTextureHandle[colorTarget]);
		return 1;
	}
	return -1;
}

a3ret a3framebufferBindDepthTexture(const a3_Framebuffer *framebuffer, const a3_TextureUnit unit)
{
	// validate
	if (framebuffer && framebuffer->handle->handle && framebuffer->depthStencil)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, framebuffer->depthTextureHandle[0]);
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------
// framebuffer double-buffer

a3ret a3framebufferDoubleActivate(const a3_FramebufferDouble *framebufferDouble)
{
	if (framebufferDouble && framebufferDouble->handle->handle)
	{
		// activate
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferDouble->handle->handle);
		a3framebufferInternalActivate(framebufferDouble->color, framebufferDouble->depthStencil);
		glViewport(0, 0, framebufferDouble->frameWidth, framebufferDouble->frameHeight);
		return 1;
	}

	// deactivate
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	return 0;
}

a3ret a3framebufferDoubleBindColorTexture(const a3_FramebufferDouble *framebufferDouble, const a3_TextureUnit unit, const a3ui32 colorTarget)
{
	if (framebufferDouble && framebufferDouble->handleDouble && colorTarget < framebufferDouble->color)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, framebufferDouble->colorTextureHandle[framebufferDouble->frontColor + colorTarget]);
		return 1;
	}
	return -1;
}

a3ret a3framebufferDoubleBindDepthTexture(const a3_FramebufferDouble *framebufferDouble, const a3_TextureUnit unit)
{
	if (framebufferDouble && framebufferDouble->handleDouble && framebufferDouble->depthStencil)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, framebufferDouble->depthTextureHandle[framebufferDouble->frontDepth]);
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------
