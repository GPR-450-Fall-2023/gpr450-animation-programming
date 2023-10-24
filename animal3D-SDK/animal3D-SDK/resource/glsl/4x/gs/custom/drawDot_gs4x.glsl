/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either ex
	or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	drawTangentBases_gs4x.glsl
	Draw tangent bases of vertices and/or faces, and/or wireframe shapes, 
		determined by flag passed to program.
*/

/*
	Joseph Lyons, Neo Kattan, Dillon Drummond

	Shader for drawing keyframe data / playhead of controller
*/

#version 450

#define MAX_SEGMENTS 32

layout (points) in;

layout (triangle_strip, max_vertices = MAX_SEGMENTS) out;


// uniforms: modelviewproj, actual triangle values, etc.
#define MAX_DATA 128
uniform vec2 uAxis[MAX_DATA]; // Holds triangle points


void main()
{
	// Dan Buckstein
	// BASIC GEOMETRY SHADERING
	// TEST: draw line from left to right
	// NOTE: we are in NDC (normalized-device-coordinates)
	//	left side of screen is x=-1, right is x=+1
	//	bottom of screen is y=-1, top is y=+1
	//	near plane is z=-1, far plane is z=+1
	// HOW TO DRAW IN GS: 
	//	either read and process inputs from VS/TS (we have none here)
	//	or make them from scratch
	//	when you are finished with an INDIVIDUAL VERTEX, call EmitVertex()
	//	if you want to RESTART PRIMITIVE, call EndPrimitive()
	// THE ONE RULE: 
	//	you must set gl_Position by the end of GS
	//	(if no GS then by the end of TS, if no TS then by the end of VS)

	for(int i = 1; i < MAX_SEGMENTS; i++)
	{
		//No need to complicate things, Delaunay works with straight lines.
		gl_Position = vec4(uAxis[0], 0.0f, 1.0f); //Center of circle
		EmitVertex();

		gl_Position = vec4(uAxis[i], 0.0f, 1.0f);
		EmitVertex();

		gl_Position = vec4(uAxis[i + 1], 0.0f, 1.0f);
		EmitVertex();

		EndPrimitive();
	}

	
}
