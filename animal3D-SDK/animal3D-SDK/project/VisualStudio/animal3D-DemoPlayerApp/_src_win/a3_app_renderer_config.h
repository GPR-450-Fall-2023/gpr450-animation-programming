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
	
	a3_app_config_renderer.h
	Renderer selection configuration.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_APP_CONFIG_RENDERER_H
#define __ANIMAL3D_APP_CONFIG_RENDERER_H


// select default tag
#ifdef A3_RENDERER_DEFAULT
#define A3_RENDERER_OPENGL
#endif	// A3_RENDERER_DEFAULT


#ifdef A3_RENDERER_OPENGL
// link OpenGL dependencies
#pragma comment(lib, "animal3D-A3DG-OpenGL.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

#else	// !A3_RENDERER_OPENGL
#ifdef A3_RENDERER_VULKAN

#else	// !A3_RENDERER_VULKAN
#ifdef A3_RENDERER_DIRECTX

#else	// !A3_RENDERER_DIRECTX
#ifdef A3_RENDERER_METAL

#else	// !A3_RENDERER_METAL
// no renderer available
#endif	// A3_RENDERER_METAL
#endif	// A3_RENDERER_DIRECTX
#endif	// A3_RENDERER_VULKAN
#endif	// A3_RENDERER_OPENGL


#endif	// !__ANIMAL3D_APP_CONFIG_RENDERER_H