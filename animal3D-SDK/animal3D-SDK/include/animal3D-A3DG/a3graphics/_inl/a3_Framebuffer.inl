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
	
	a3_Framebuffer.inl
	Inline definitions for framebuffer.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_FRAMEBUFFER_H
#ifndef __ANIMAL3D_FRAMEBUFFER_INL
#define __ANIMAL3D_FRAMEBUFFER_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3framebufferReference(a3_Framebuffer *framebuffer)
{
	if (framebuffer)
		return a3handleIncrementCount(framebuffer->handle);
	return -1;
}

A3_INLINE a3ret a3framebufferRelease(a3_Framebuffer *framebuffer)
{
	if (framebuffer)
	{
		a3i32 ret = a3handleDecrementCount(framebuffer->handle);
		if (ret == 0)
			framebuffer->frameWidth = framebuffer->frameHeight = framebuffer->color = framebuffer->depthStencil = 0;
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3framebufferDoubleSwap(a3_FramebufferDouble *framebufferDouble)
{
	a3ui32 tmpHandle;
	if (framebufferDouble && framebufferDouble->handle->handle)
	{
		// swap handles
		tmpHandle = framebufferDouble->handle->handle;
		framebufferDouble->handle->handle = framebufferDouble->handleDouble;
		framebufferDouble->handleDouble = tmpHandle;

		// swap texture offsets
		framebufferDouble->frontColor = a3fbo_colorTargetMax - framebufferDouble->frontColor;
		framebufferDouble->frontDepth = 1 - framebufferDouble->frontDepth;

		// done
		return 1;
	}
	return -1;
}

A3_INLINE a3ret a3framebufferDoubleReference(a3_FramebufferDouble *framebufferDouble)
{
	if (framebufferDouble)
		return a3handleIncrementCount(framebufferDouble->handle);
	return -1;
}

A3_INLINE a3ret a3framebufferDoubleRelease(a3_FramebufferDouble *framebufferDouble)
{
	if (framebufferDouble)
	{
		a3i32 ret = a3handleDecrementCount(framebufferDouble->handle);
		if (ret == 0)
		{
			// reset
			framebufferDouble->frontColor = framebufferDouble->frontDepth = 0;
			framebufferDouble->frameWidth = framebufferDouble->frameHeight = framebufferDouble->color = framebufferDouble->depthStencil = 0;
		}
		return ret;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_FRAMEBUFFER_INL
#endif	// __ANIMAL3D_FRAMEBUFFER_H