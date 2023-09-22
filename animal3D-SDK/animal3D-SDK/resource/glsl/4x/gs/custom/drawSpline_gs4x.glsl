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
	
	drawTangentBases_gs4x.glsl
	Draw tangent bases of vertices and/or faces, and/or wireframe shapes, 
		determined by flag passed to program.
*/

#version 450

#define MAX_VERTICES 32

layout (points) in;

layout (line_strip, max_vertices = MAX_VERTICES) out;


// uniforms: modelviewproj, actual keyframe values, etc.
#define MAX_DATA 128
uniform vec3 uAxis[MAX_DATA];


void main()
{
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


	vec4 v0 = vec4(uAxis[0], 1.0);
	gl_Position = v0;
	EmitVertex();

	vec4 v1 = vec4(uAxis[1], 1.0);
	gl_Position = v1;
	EmitVertex();

	// INTERPOLATE using algorithm of choice between uAxis[k] and uAxis[k+1]
	//	from param = [0,1)
	// total number of emits will be MAX_VERTICES
}
