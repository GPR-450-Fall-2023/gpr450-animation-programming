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
	
	passTangentBasis_skin_transform_vs4x.glsl
	Calculate and pass tangent basis with skinning.
*/

#version 450

layout (location = 0) in vec4 aPosition;
layout (location = 2) in vec4 aNormal;
layout (location = 8) in vec4 aTexcoord;
layout (location = 10) in vec4 aTangent;
layout (location = 11) in vec4 aBitangent;

layout (location = 7) in ivec4 aBlendIndex; //4 influences
layout (location = 1) in vec4 aBlendWeight; //4 weights

uniform mat4 uP;
uniform mat4 uMV, uMV_nrm;
uniform mat4 uAtlas;

out vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
};

flat out int vVertexID;
flat out int vInstanceID;

#define MAX_SKINMATS 128

uniform ubTransformBlend
{
	mat4 skinMat[MAX_SKINMATS];
};


vec4 skin(in vec4 v)
{
	vec4 v_out = vec4(0.0);

	int j, i;
	float w;
	
	//Actual equation
	for(int i = 0; i < 4; i++)
	{
		j = aBlendIndex[i];
		w = aBlendWeight[i];
		v_out += skinMat[j] * (w * v);
	}

	return v_out;
}


void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
//	gl_Position = aPosition;

	vec4 tangent = normalize(skin(vec4(aTangent.xyz, 0.0)));
	vec4 bitangent = normalize(skin(vec4(aBitangent.xyz, 0.0)));
	vec4 normal = normalize(skin(vec4(aNormal.xyz, 0.0)));
	vec4 position = skin(aPosition);

	vTangentBasis_view = uMV_nrm * mat4(aTangent, aBitangent, aNormal, vec4(0.0));
	vTangentBasis_view[3] = uMV * aPosition;
	gl_Position = uP * vTangentBasis_view[3];
	
	vTexcoord_atlas = uAtlas * aTexcoord;

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;

}
