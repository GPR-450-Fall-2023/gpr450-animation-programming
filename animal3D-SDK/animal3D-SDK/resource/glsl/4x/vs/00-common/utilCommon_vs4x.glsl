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
	
	utilCommon_vs4x.glsl
	Common utilities for vertex shaders.
*/


vec4 lerp(in vec4 v0, in vec4 v1, in float u)
{
	return (v0 + (v1 - v0) * u);
}

vec4 nlerp(in vec4 v0, in vec4 v1, in float u)
{
	return normalize(lerp(v0, v1, u));
}

vec4 CatmullRom(in vec4 vP, in vec4 v0, in vec4 v1, in vec4 vN, in float u)
{
	mat4 v = mat4(vP, v0, v1, vN);
	const mat4 k = mat4( 0.0, +2.0,  0.0,  0.0,
						-1.0,  0.0, +1.0,  0.0,
						+2.0, -5.0, +4.0, -1.0,
						-1.0, +3.0, -3.0, +1.0);
	float t0 = 0.5, t1 = t0 * u, t2 = t1 * u, t3 = t2 * u;
	return (v * (k * vec4(t0, t1, t2, t3)));
}

vec4 nCatmullRom(in vec4 vP, in vec4 v0, in vec4 v1, in vec4 vN, in float u)
{
	return normalize(CatmullRom(vP, v0, v1, vN, u));
}
