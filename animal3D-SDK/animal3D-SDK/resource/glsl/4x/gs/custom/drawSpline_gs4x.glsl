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

#version 450

#define MAX_VERTICES 32

layout (points) in;

layout (line_strip, max_vertices = MAX_VERTICES) out;


// uniforms: modelviewproj, actual keyframe values, etc.
#define MAX_DATA 128
uniform vec3 uAxis[MAX_DATA];
uniform int uCount;
uniform float uFlag;


vec3 lerp(vec3 start, vec3 end, float u)
{
	return start + ((end - start) * u);
}


vec3 catmullRom(vec3 pPrev, vec3 p0, vec3 p1, vec3 pNext, float t)
{
	vec3 pPrevCalc = (-t + (2 * pow(t, 2)) - pow(t, 3)) * pPrev;
	vec3 p0Calc = (2 - (5 * pow(t, 2)) + (3 * pow(t, 3))) * p0;
	vec3 p1Calc = (t + (4 * pow(t, 2)) - (3 * pow(t, 3))) * p1;
	vec3 pNextCalc = (-1 * pow(t, 2) + pow(t, 3)) * pNext;
	return (pPrevCalc + p0Calc + p1Calc + pNextCalc) / 2.0;
}


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


//	for(int i = 0; i <= MAX_VERTICES; i++)
//	{
//		// LERP VERSION
//		//vec4 v = vec4(lerp(uAxis[0], uAxis[1], float(i) / MAX_VERTICES), 1.0);
//
//		// CATMULL ROM VERSION
////		vec3 v3 = catmullRom(uAxis[0], uAxis[1], uAxis[2], uAxis[3], float(i) / MAX_VERTICES);
////		v3.x = (2 * float(i) / MAX_VERTICES) - 1;
////		vec4 v = vec4(v3, 1.0f);
//
//		// LINE OF CODE SHOULD MAKE HORIZONTAL LINE FROM 0 TO 1 IF NO SHADER ERRORS
//		//vec4 v = vec4(float(i) / MAX_VERTICES, 0, 0, 1.0f);
//
//		//gl_Position = v;
//		//EmitVertex();
//	}

	if(uCount != -1)
	{
		for(int k = 0; k <= MAX_VERTICES; k++)
		{
			// Lerp
			gl_Position = vec4(lerp(uAxis[1], uAxis[2], float(k) / MAX_VERTICES), 1.0f);
			
			// Catmull Rom
//			vec3 v3 = catmullRom(uAxis[0], uAxis[1], uAxis[2], uAxis[3], float(k) / MAX_VERTICES);
//			vec4 v = vec4(v3, 1.0f);
//			gl_Position = v;

			EmitVertex();
		}
	}
	else
	{
		vec4 bottom = vec4(uFlag, -1, 0, 1.0f);
		gl_Position = bottom;
		EmitVertex();

		vec4 top = vec4(uFlag, 1, 0, 1.0f);
		gl_Position = top;
		EmitVertex();
	}

	// INTERPOLATE using algorithm of choice between uAxis[k] and uAxis[k+1]
	//	from param = [0,1)
	// total number of emits will be MAX_VERTICES
}
