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
	
	a3_DemoState_loading.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     LOADING in this file.                    ***
	****************************************************
*/

//-----------------------------------------------------------------------------

// uncomment this to link extension library (if available)
//#define A3_USER_ENABLE_EXTENSION

// **WARNING: FOR TESTING/COMPARISON ONLY, DO NOT USE IN DELIVERABLE BUILDS**
// uncomment this to allow shader decoding (if available)
#define A3_USER_ENABLE_SHADER_DECODING


//-----------------------------------------------------------------------------

#ifdef A3_USER_ENABLE_SHADER_DECODING
// override shader loading function name before including
#define a3shaderCreateFromFileList a3shaderCreateFromFileListEncoded
#endif	// A3_USER_ENABLE_SHADER_DECODING


#ifdef _WIN32
#ifdef A3_USER_ENABLE_EXTENSION
// force link extension lib
#pragma comment(lib,"animal3D-A3DX.lib")
#endif	// A3_USER_ENABLE_EXTENSION
#ifdef A3_USER_ENABLE_SHADER_DECODING
// add lib for shader decoding
#pragma comment(lib,"animal3D-UtilityLib.lib")
#endif	// A3_USER_ENABLE_SHADER_DECODING
#endif	// _WIN32


// define resource directories
#define A3_DEMO_RES_DIR	"../../../../resource/"
#define A3_DEMO_GLSL	A3_DEMO_RES_DIR"glsl/"
#define A3_DEMO_TEX		A3_DEMO_RES_DIR"tex/"
#define A3_DEMO_OBJ		A3_DEMO_RES_DIR"obj/"

// define resource subdirectories
#define A3_DEMO_VS		A3_DEMO_GLSL"4x/vs/"
#define A3_DEMO_TS		A3_DEMO_GLSL"4x/ts/"
#define A3_DEMO_GS		A3_DEMO_GLSL"4x/gs/"
#define A3_DEMO_FS		A3_DEMO_GLSL"4x/fs/"
#define A3_DEMO_CS		A3_DEMO_GLSL"4x/cs/"


//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// GENERAL UTILITIES

a3real4x4r a3demo_setAtlasTransform_internal(a3real4x4p m_out,
	const a3ui16 atlasWidth, const a3ui16 atlasHeight,
	const a3ui16 subTexturePosX, const a3ui16 subTexturePosY,
	const a3ui16 subTextureWidth, const a3ui16 subTextureHeight,
	const a3ui16 subTextureBorderPadding, const a3ui16 subTextureAdditionalPadding)
{
	a3real4x4SetIdentity(m_out);
	m_out[0][0] = (a3real)(subTextureWidth) / (a3real)(atlasWidth);
	m_out[1][1] = (a3real)(subTextureHeight) / (a3real)(atlasHeight);
	m_out[3][0] = (a3real)(subTexturePosX + subTextureBorderPadding + subTextureAdditionalPadding) / (a3real)(atlasWidth);
	m_out[3][1] = (a3real)(subTexturePosY + subTextureBorderPadding + subTextureAdditionalPadding) / (a3real)(atlasHeight);
	return m_out;
}


// initialize dummy drawable
inline void a3demo_initDummyDrawable_internal(a3_DemoState *demoState)
{
	// dummy drawable for point drawing: copy any of the existing ones, 
	//	set vertex count to 1 and primitive to points (0x0000)
	// DO NOT RELEASE THIS DRAWABLE; it is a managed stand-in!!!
	*demoState->dummyDrawable = *demoState->draw_grid;
	demoState->dummyDrawable->primitive = 0;
	demoState->dummyDrawable->count = 1;
}


//-----------------------------------------------------------------------------
// uniform helpers

#define a3demo_getUniformLocation(demoProgram, handleName, getLocFunc) (demoProgram->handleName = getLocFunc(demoProgram->program, #handleName))
#define a3demo_getUniformLocationValid(demoProgram, handleName, getLocFunc) if (a3demo_getUniformLocation(demoProgram, handleName, getLocFunc) >= 0)
#define a3demo_setUniformDefault(demoProgram, handleName, sendFunc, type, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformGetLocation) \
		sendFunc(type, demoProgram->handleName, 1, value)
#define a3demo_setUniformDefaultMat(demoProgram, handleName, sendFunc, type, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformGetLocation) \
		sendFunc(type, 0, demoProgram->handleName, 1, value)
#define a3demo_setUniformDefaultBlock(demoProgram, handleName, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformBlockGetLocation) \
		a3shaderUniformBlockBind(demoProgram->program, demoProgram->handleName, value)

#define a3demo_setUniformDefaultFloat(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_single, value)
#define a3demo_setUniformDefaultVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec2, value)
#define a3demo_setUniformDefaultVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec3, value)
#define a3demo_setUniformDefaultVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec4, value)
#define a3demo_setUniformDefaultDouble(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_single, value)
#define a3demo_setUniformDefaultDVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec2, value)
#define a3demo_setUniformDefaultDVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec3, value)
#define a3demo_setUniformDefaultDVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec4, value)
#define a3demo_setUniformDefaultInteger(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_single, value)
#define a3demo_setUniformDefaultIVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec2, value)
#define a3demo_setUniformDefaultIVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec3, value)
#define a3demo_setUniformDefaultIVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec4, value)
#define a3demo_setUniformDefaultMat2(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat2, a3mat2_identity.mm)
#define a3demo_setUniformDefaultMat3(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat3, a3mat3_identity.mm)
#define a3demo_setUniformDefaultMat4(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat4, a3mat4_identity.mm)


//-----------------------------------------------------------------------------
// LOADING

// utility to load geometry
void a3demo_loadGeometry(a3_DemoState *demoState)
{
	// tmp descriptor for loaded model
	typedef struct a3_TAG_DEMOSTATELOADEDMODEL {
		const a3byte *modelFilePath, *skinWeightsFilePath;
		const a3real *transform;
		a3_ModelLoaderFlag flag;
	} a3_DemoStateLoadedModel;

	// static model transformations
	static const a3mat4 downscale20x_y2z_x2y = {
		 0.00f, +0.05f,  0.00f,  0.00f,
		 0.00f,  0.00f, +0.05f,  0.00f,
		+0.05f,  0.00f,  0.00f,  0.00f,
		 0.00f,  0.00f,  0.00f, +1.00f,
	};
	static const a3mat4 scale1x_z2y = {
		+0.05f,  0.00f,  0.00f,  0.00f,
		 0.00f,  0.00f, -0.05f,  0.00f,
		 0.00f, +0.05f,  0.00f,  0.00f,
		 0.00f,  0.00f,  0.00f, +1.00f,
	};

	// pointer to shared vbo/ibo
	a3_VertexBuffer *vbo_ibo;
	a3_VertexArrayDescriptor *vao;
	a3_VertexDrawable *currentDrawable;
	a3ui32 sharedVertexStorage = 0, sharedIndexStorage = 0;
	a3ui32 numVerts = 0;
	a3ui32 i, j;


	// file streaming (if requested)
	a3_FileStream fileStream[1] = { 0 };
	const a3byte *const geometryStream = "./data/gpro_base_geom.dat";

	// geometry data
	a3_GeometryData displayShapesData[4] = { 0 };
	a3_GeometryData proceduralShapesData[6] = { 0 };
	a3_GeometryData loadedModelsData[3] = { 0 };
	a3_GeometryData morphTargetsData[1][5] = { 0 };
	const a3ui32 displayShapesCount = sizeof(displayShapesData) / sizeof(a3_GeometryData);
	const a3ui32 proceduralShapesCount = sizeof(proceduralShapesData) / sizeof(a3_GeometryData);
	const a3ui32 loadedModelsCount = sizeof(loadedModelsData) / sizeof(a3_GeometryData);
	const a3ui32 morphTargetsPerModel = sizeof(*morphTargetsData) / sizeof(a3_GeometryData);
	const a3ui32 morphModelsCount = sizeof(morphTargetsData) / sizeof(*morphTargetsData);

	// morphing vertex format
	a3_VertexFormatDescriptor morphFormat[1] = { 0 };
	a3_VertexAttributeDescriptor morphAttrib[16] = { 0 };
	a3ui32 const morphAttribCount = sizeof(morphAttrib) / sizeof(a3_VertexAttributeDescriptor);

	// common index format
	a3_IndexFormatDescriptor sceneCommonIndexFormat[1] = { 0 };
	a3ui32 bufferOffset, *const bufferOffsetPtr = &bufferOffset;


	// procedural scene objects
	// attempt to load stream if requested
	if (demoState->streaming && a3fileStreamOpenRead(fileStream, geometryStream))
	{
		// read from stream

		// static display objects
		for (i = 0; i < displayShapesCount; ++i)
			a3fileStreamReadObject(fileStream, displayShapesData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// procedurally-generated objects
		for (i = 0; i < proceduralShapesCount; ++i)
			a3fileStreamReadObject(fileStream, proceduralShapesData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// loaded model objects
		for (i = 0; i < loadedModelsCount; ++i)
			a3fileStreamReadObject(fileStream, loadedModelsData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// morphing objects
		for (i = 0; i < morphModelsCount; ++i)
			for (j = 0; j < morphTargetsPerModel; ++j)
				a3fileStreamReadObject(fileStream, morphTargetsData[i] + j, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (!demoState->streaming || a3fileStreamOpenWrite(fileStream, geometryStream))
	{
		// create new data
		a3_ProceduralGeometryDescriptor displayShapes[4] = { a3geomShape_none };
		a3_ProceduralGeometryDescriptor proceduralShapes[6] = { a3geomShape_none };
		const a3_DemoStateLoadedModel loadedShapes[3] = {
			{ A3_DEMO_OBJ"teapot/teapot.obj", 0, downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
			//{ A3_DEMO_OBJ"egnaro/egnaro_mesh.obj", A3_DEMO_OBJ"egnaro/egnaro_skin.xml", a3mat4_identity.mm, a3model_calculateVertexTangents },
			{ A3_DEMO_OBJ"xbot/xbot_surface.obj", A3_DEMO_OBJ"xbot/xbot_surface.xml", scale1x_z2y.mm, a3model_calculateVertexTangents },
			{ A3_DEMO_OBJ"xbot/xbot_joints.obj", A3_DEMO_OBJ"xbot/xbot_joints.xml", scale1x_z2y.mm, a3model_calculateVertexTangents },
		};
		const a3_DemoStateLoadedModel morphShapes[1][5] = {
			{
				{ A3_DEMO_OBJ"teapot/morph/teapot_base.obj", 0, downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale.obj", 0, downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale_x.obj", 0, downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale_y.obj", 0, downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
				{ A3_DEMO_OBJ"teapot/morph/teapot_scale_z.obj", 0, downscale20x_y2z_x2y.mm, a3model_calculateVertexTangents },
			},
		};

		// static scene procedural objects
		//	(axes, grid)
		a3proceduralCreateDescriptorAxes(displayShapes + 0, a3geomFlag_wireframe, 0.0f, 1);
		a3proceduralCreateDescriptorPlane(displayShapes + 1, a3geomFlag_wireframe, a3geomAxis_default, 20.0f, 20.0f, 20, 20);
		a3proceduralCreateDescriptorCone(displayShapes + 2, a3geomFlag_wireframe, a3geomAxis_default, 0.25f, 1.0f, 4, 1, 1);
		a3proceduralCreateDescriptorSphere(displayShapes + 3, a3geomFlag_wireframe, a3geomAxis_default, 0.25f, 4, 2);
		for (i = 0; i < displayShapesCount; ++i)
		{
			a3proceduralGenerateGeometryData(displayShapesData + i, displayShapes + i, 0);
			a3fileStreamWriteObject(fileStream, displayShapesData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// other procedurally-generated objects
		a3proceduralCreateDescriptorPlane(proceduralShapes + 0, a3geomFlag_tangents, a3geomAxis_default, 1.0f, 1.0f, 1, 1);
		a3proceduralCreateDescriptorBox(proceduralShapes + 1, a3geomFlag_tangents, 1.0f, 1.0f, 1.0f, 1, 1, 1);
		a3proceduralCreateDescriptorSphere(proceduralShapes + 2, a3geomFlag_tangents, a3geomAxis_default, 1.0f, 32, 24);
		a3proceduralCreateDescriptorCylinder(proceduralShapes + 3, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 1.0f, 32, 4, 4);
		a3proceduralCreateDescriptorCapsule(proceduralShapes + 4, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 1.0f, 32, 12, 4);
		a3proceduralCreateDescriptorTorus(proceduralShapes + 5, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 0.25f, 32, 24);
		for (i = 0; i < proceduralShapesCount; ++i)
		{
			a3proceduralGenerateGeometryData(proceduralShapesData + i, proceduralShapes + i, 0);
			a3fileStreamWriteObject(fileStream, proceduralShapesData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// objects loaded from mesh files
		for (i = 0; i < loadedModelsCount; ++i)
		{
			if (loadedShapes[i].skinWeightsFilePath)
			{
				a3byte *nodeNames[128] = { 0 };
				a3ui32 nodeCount = a3hierarchyGetNodeNames(nodeNames, demoState->demoMode1_animation->hierarchy_skel);
				a3modelLoadOBJSkinWeights(loadedModelsData + i, loadedShapes[i].modelFilePath, loadedShapes[i].flag, loadedShapes[i].skinWeightsFilePath, nodeNames, nodeCount, loadedShapes[i].transform);
			}
			else
			{
				a3modelLoadOBJ(loadedModelsData + i, loadedShapes[i].modelFilePath, loadedShapes[i].flag, loadedShapes[i].transform);
			}
			a3fileStreamWriteObject(fileStream, loadedModelsData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// morphing objects
		for (i = 0; i < morphModelsCount; ++i)
			for (j = 0; j < morphTargetsPerModel; ++j)
			{
				a3modelLoadOBJ(morphTargetsData[i] + j, morphShapes[i][j].modelFilePath, morphShapes[i][j].flag, morphShapes[i][j].transform);
				a3fileStreamWriteObject(fileStream, morphTargetsData[i] + j, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
			}

		// done
		a3fileStreamClose(fileStream);
	}


	// GPU data upload process: 
	//	- determine storage requirements
	//	- allocate buffer
	//	- create vertex arrays using unique formats
	//	- create drawable and upload data

	// get storage size
	sharedVertexStorage = numVerts = 0;
	for (i = 0; i < displayShapesCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(displayShapesData + i);
		numVerts += displayShapesData[i].numVertices;
	}
	for (i = 0; i < proceduralShapesCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(proceduralShapesData + i);
		numVerts += proceduralShapesData[i].numVertices;
	}
	for (i = 0; i < loadedModelsCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(loadedModelsData + i);
		numVerts += loadedModelsData[i].numVertices;
	}


	// morph targets setup
	//	- add required data from each model to new data descriptor
	{
		a3_VertexAttributeDescriptor* morphAttribPtr;
		a3_VertexAttributeName attribName;
		a3ui32 morphModelIndex = 0;

		for (i = 0, morphAttribPtr = morphAttrib, attribName = a3attrib_user00; i < morphTargetsPerModel; ++i)
			for (j = 0; j < 3; ++j, ++morphAttribPtr, ++attribName)
				// create descriptors for position, normal and tangent
				a3vertexAttribCreateDescriptor(morphAttribPtr, (a3_VertexAttributeName)attribName, a3attrib_vec3);
		// the final attribute will be for texture coordinates
		a3vertexAttribCreateDescriptor(morphAttribPtr, a3attrib_user15, a3attrib_vec2);

		// make format
		a3vertexFormatCreateDescriptor(morphFormat, morphAttrib, morphAttribCount);
		for (i = 0; i < morphModelsCount; ++i)
		{
			sharedVertexStorage += a3vertexFormatGetStorageSpaceRequired(morphFormat, morphTargetsData[i]->numVertices);
			numVerts += morphTargetsData[i]->numVertices;
		}
	}


	// common index format required for shapes that share vertex formats
	a3geometryCreateIndexFormat(sceneCommonIndexFormat, numVerts);
	sharedIndexStorage = 0;
	for (i = 0; i < displayShapesCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, displayShapesData[i].numIndices);
	for (i = 0; i < proceduralShapesCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, proceduralShapesData[i].numIndices);
	for (i = 0; i < loadedModelsCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, loadedModelsData[i].numIndices);
	for (i = 0; i < morphModelsCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, morphTargetsData[i]->numIndices);

	// create shared buffer
	vbo_ibo = demoState->vbo_staticSceneObjectDrawBuffer;
	a3bufferCreateSplit(vbo_ibo, "vbo/ibo:scene", a3buffer_vertex, sharedVertexStorage, sharedIndexStorage, 0, 0);
	sharedVertexStorage = 0;


	// create vertex formats and drawables
	// axes: position and color
	vao = demoState->vao_position_color;
	a3geometryGenerateVertexArray(vao, "vao:pos+col", displayShapesData + 0, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_axes;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// grid: position attribute only
	// overlay objects are also just position
	vao = demoState->vao_position;
	a3geometryGenerateVertexArray(vao, "vao:pos", displayShapesData + 1, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_grid;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_link;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_node;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 3, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// models
	vao = demoState->vao_tangentbasis_texcoord;
	a3geometryGenerateVertexArray(vao, "vao:tb+tc", proceduralShapesData + 0, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_unit_plane_z;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_box;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_sphere;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_cylinder;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 3, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_capsule;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 4, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unit_torus;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 5, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	
	currentDrawable = demoState->draw_teapot;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, loadedModelsData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// skinned models
	vao = demoState->vao_tangentbasis_texcoord_skin;
	a3geometryGenerateVertexArray(vao, "vao:tb+tc+skin", loadedModelsData + 1, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_character_skin;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, loadedModelsData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_character_skin_alt;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, loadedModelsData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// morphing models
	vao = demoState->vao_tangentbasis_texcoord_morph5;
	a3vertexArrayCreateDescriptor(vao, "vao:tb+tc+morph5", vbo_ibo, morphFormat, sharedVertexStorage);
	{
		a3_VertexAttributeDataDescriptor morphAttribData[16] = { 0 }, * morphAttribDataPtr;
		a3_VertexAttributeName attribName;
		a3ui32 morphModelIndex = 0;

		for (i = 0, morphAttribDataPtr = morphAttribData, attribName = a3attrib_user00; i < morphTargetsPerModel; ++i)
			for (j = 0; j < 3; ++j, ++morphAttribDataPtr, ++attribName)
				a3vertexAttribDataCreateDescriptor(morphAttribDataPtr, (a3_VertexAttributeName)attribName, morphTargetsData[morphModelIndex][i].attribData[j * j]);
		a3vertexAttribDataCreateDescriptor(morphAttribDataPtr, a3attrib_user15, morphTargetsData[morphModelIndex]->attribData[a3attrib_geomTexcoord]);

		a3vertexArrayStore(vao, morphAttribData, morphTargetsData[morphModelIndex]->numVertices, 0, 0);
		a3indexBufferStore(vbo_ibo, sceneCommonIndexFormat, morphTargetsData[morphModelIndex]->indexData, morphTargetsData[morphModelIndex]->numIndices, 0, &i, 0);

		currentDrawable = demoState->draw_teapot_morph;
		a3vertexDrawableCreateIndexed(currentDrawable, vao, vbo_ibo, sceneCommonIndexFormat, morphTargetsData[morphModelIndex]->primType, i, morphTargetsData[morphModelIndex]->numIndices);
	}
	

	// release data when done
	for (i = 0; i < displayShapesCount; ++i)
		a3geometryReleaseData(displayShapesData + i);
	for (i = 0; i < proceduralShapesCount; ++i)
		a3geometryReleaseData(proceduralShapesData + i);
	for (i = 0; i < loadedModelsCount; ++i)
		a3geometryReleaseData(loadedModelsData + i);
	for (i = 0; i < morphModelsCount; ++i)
		for (j = 0; j < morphTargetsPerModel; ++j)
			a3geometryReleaseData(morphTargetsData[i] + j);


	// dummy
	a3demo_initDummyDrawable_internal(demoState);
}


// utility to load shaders
void a3demo_loadShaders(a3_DemoState *demoState)
{
	// structure to help with shader management
	typedef struct a3_TAG_DEMOSTATESHADER {
		a3_Shader shader[1];
		a3byte shaderName[32];

		a3_ShaderType shaderType;
		a3ui32 srcCount;
		const a3byte* filePath[8];	// max number of source files per shader
	} a3_DemoStateShader;

	// direct to demo programs
	a3_DemoStateShaderProgram *currentDemoProg;
	a3i32 flag;
	a3ui32 i;

	// maximum uniform buffer size
	const a3ui32 uBlockSzMax = a3shaderUniformBlockMaxSize();

	// some default uniform values
	const a3f32 defaultFloat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const a3f64 defaultDouble[] = { 0.0, 0.0, 0.0, 1.0 };
	const a3i32 defaultInt[] = { 0, 0, 0, 1 };
	const a3i32 defaultTexUnits[] = {
		a3tex_unit00, a3tex_unit01, a3tex_unit02, a3tex_unit03,
		a3tex_unit04, a3tex_unit05, a3tex_unit06, a3tex_unit07,
		a3tex_unit08, a3tex_unit09, a3tex_unit10, a3tex_unit11,
		a3tex_unit12, a3tex_unit13, a3tex_unit14, a3tex_unit15
	};


	// list of all unique shaders
	// this is a good idea to avoid multi-loading 
	//	those that are shared between programs
	union {
		struct {
			// vertex shaders
			// base
			a3_DemoStateShader
				passthru_transform_vs[1],
				passColor_transform_vs[1],
				passthru_transform_instanced_vs[1],
				passColor_transform_instanced_vs[1];
			// 00-common
			a3_DemoStateShader
				passTexcoord_transform_vs[1],
				passTangentBasis_transform_vs[1],
				passTangentBasis_morph5_transform_vs[1],
				passTangentBasis_skin_transform_vs[1],
				passTexcoord_transform_instanced_vs[1],
				passTangentBasis_transform_instanced_vs[1],
				passTangentBasis_morph5_transform_instanced_vs[1],
				passTangentBasis_skin_transform_instanced_vs[1];

			// geometry shaders
			// 00-common
			a3_DemoStateShader
				drawTangentBasis_gs[1];

			// fragment shaders
			// base
			a3_DemoStateShader
				drawColorUnif_fs[1],
				drawColorAttrib_fs[1];
			// 00-common
			a3_DemoStateShader
				drawTexture_fs[1],
				drawLambert_fs[1],
				drawPhong_fs[1];
		};
	} shaderList = {
		{
			// ****REMINDER: 'Encoded' shaders are available for proof-of-concept
			//	testing ONLY!  Insert /e before file names.
			// DO NOT SUBMIT WORK USING ENCODED SHADERS OR YOU WILL GET ZERO!!!

			// vs
			// base
			{ { { 0 },	"shdr-vs:passthru-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"e/passthru_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-col-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"e/passColor_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:passthru-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"e/passthru_transform_instanced_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-col-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"e/passColor_transform_instanced_vs4x.glsl" } } },
			// 00-common
			{ { { 0 },	"shdr-vs:pass-tex-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTexcoord_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTangentBasis_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-morph5-t",			a3shader_vertex  ,	2,{ A3_DEMO_VS"00-common/e/passTangentBasis_morph5_transform_vs4x.glsl",
																					A3_DEMO_VS"00-common/e/utilCommon_vs4x.glsl",} } },
			{ { { 0 },	"shdr-vs:pass-tb-skin-t",			a3shader_vertex  ,	2,{ A3_DEMO_VS"00-common/e/passTangentBasis_skin_transform_vs4x.glsl",
																					A3_DEMO_VS"00-common/e/utilCommon_vs4x.glsl",} } },
			{ { { 0 },	"shdr-vs:pass-tex-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTexcoord_transform_instanced_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"00-common/e/passTangentBasis_transform_instanced_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-morph5-t-inst",	a3shader_vertex  ,	2,{ A3_DEMO_VS"00-common/e/passTangentBasis_morph5_transform_instanced_vs4x.glsl",
																					A3_DEMO_VS"00-common/e/utilCommon_vs4x.glsl",} } },
			{ { { 0 },	"shdr-vs:pass-tb-skin-t-inst",		a3shader_vertex  ,	2,{ A3_DEMO_VS"00-common/e/passTangentBasis_skin_transform_instanced_vs4x.glsl",
																					A3_DEMO_VS"00-common/e/utilCommon_vs4x.glsl",} } },

			// gs
			// 00-common
			{ { { 0 },	"shdr-gs:draw-tb",					a3shader_geometry,	2,{ A3_DEMO_GS"00-common/e/drawTangentBasis_gs4x.glsl",
																					A3_DEMO_GS"00-common/e/utilCommon_gs4x.glsl",} } },

			// fs
			// base
			{ { { 0 },	"shdr-fs:draw-col-unif",			a3shader_fragment,	1,{ A3_DEMO_FS"e/drawColorUnif_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-col-attr",			a3shader_fragment,	1,{ A3_DEMO_FS"e/drawColorAttrib_fs4x.glsl" } } },
			// 00-common
			{ { { 0 },	"shdr-fs:draw-tex",					a3shader_fragment,	1,{ A3_DEMO_FS"00-common/e/drawTexture_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Lambert",				a3shader_fragment,	2,{ A3_DEMO_FS"00-common/e/drawLambert_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
			{ { { 0 },	"shdr-fs:draw-Phong",				a3shader_fragment,	2,{ A3_DEMO_FS"00-common/e/drawPhong_fs4x.glsl",
																					A3_DEMO_FS"00-common/e/utilCommon_fs4x.glsl",} } },
		}
	};
	a3_DemoStateShader *const shaderListPtr = (a3_DemoStateShader *)(&shaderList), *shaderPtr;
	const a3ui32 numUniqueShaders = sizeof(shaderList) / sizeof(a3_DemoStateShader);


	printf("\n\n---------------- LOAD SHADERS STARTED  ---------------- \n");


	// load unique shaders: 
	//	- load file contents
	//	- create and compile shader object
	//	- release file contents
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;
		flag = a3shaderCreateFromFileList(shaderPtr->shader,
			shaderPtr->shaderName, shaderPtr->shaderType,
			shaderPtr->filePath, shaderPtr->srcCount);
		if (flag == 0)
			printf("\n ^^^^ SHADER %u '%s' FAILED TO COMPILE \n\n", i, shaderPtr->shader->handle->name);
	}


	// setup programs: 
	//	- create program object
	//	- attach shader objects

	// base programs: 
	// transform-only program
	currentDemoProg = demoState->prog_transform;
	a3shaderProgramCreate(currentDemoProg->program, "prog:transform");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_vs->shader);
	// transform-only program with instancing
	currentDemoProg = demoState->prog_transform_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:transform-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_instanced_vs->shader);
	// uniform color program
	currentDemoProg = demoState->prog_drawColorUnif;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-unif");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorUnif_fs->shader);
	// color attrib program
	currentDemoProg = demoState->prog_drawColorAttrib;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-attr");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// uniform color program with instancing
	currentDemoProg = demoState->prog_drawColorUnif_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-unif-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorUnif_fs->shader);
	// color attrib program with instancing
	currentDemoProg = demoState->prog_drawColorAttrib_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-attr-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);

	// 00-common programs: 
	// texturing
	currentDemoProg = demoState->prog_drawTexture;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_fs->shader);
	// texturing with instancing
	currentDemoProg = demoState->prog_drawTexture_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_fs->shader);
	// Lambert
	currentDemoProg = demoState->prog_drawLambert;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Lambert");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawLambert_fs->shader);
	// Lambert with instancing
	currentDemoProg = demoState->prog_drawLambert_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Lambert-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawLambert_fs->shader);
	// Phong
	currentDemoProg = demoState->prog_drawPhong;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);
	// Phong with instancing
	currentDemoProg = demoState->prog_drawPhong_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);
	// Phong for 5-target morphing
	currentDemoProg = demoState->prog_drawPhong_morph5;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-morph5");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_morph5_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);
	// Phong for 5-target morphing with instancing
	currentDemoProg = demoState->prog_drawPhong_morph5_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-morph5-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_morph5_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);
	// Phong for skinning
	currentDemoProg = demoState->prog_drawPhong_skin;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-skin");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_skin_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);
	// Phong for skinning with instancing
	currentDemoProg = demoState->prog_drawPhong_skin_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-skin-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_skin_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_fs->shader);

	// tangent basis
	currentDemoProg = demoState->prog_drawTangentBasis;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// tangent basis with instancing
	currentDemoProg = demoState->prog_drawTangentBasis_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// tangent basis for 5-target morphing
	currentDemoProg = demoState->prog_drawTangentBasis_morph5;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-morph5");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_morph5_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// tangent basis for 5-target morphing with instancing
	currentDemoProg = demoState->prog_drawTangentBasis_morph5_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-morph5-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_morph5_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// tangent basis for skinning
	currentDemoProg = demoState->prog_drawTangentBasis_skin;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-skin");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_skin_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// tangent basis for skinning with instancing
	currentDemoProg = demoState->prog_drawTangentBasis_skin_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tb-skin-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_skin_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTangentBasis_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);


	// activate a primitive for validation
	// makes sure the specified geometry can draw using programs
	// good idea to activate the drawable with the most attributes
	a3vertexDrawableActivate(demoState->draw_axes);

	// link and validate all programs
	for (i = 0; i < demoStateMaxCount_shaderProgram; ++i)
	{
		currentDemoProg = demoState->shaderProgram + i;
		flag = a3shaderProgramLink(currentDemoProg->program);
		if (flag == 0)
			printf("\n ^^^^ PROGRAM %u '%s' FAILED TO LINK \n\n", i, currentDemoProg->program->handle->name);

		flag = a3shaderProgramValidate(currentDemoProg->program);
		if (flag == 0)
			printf("\n ^^^^ PROGRAM %u '%s' FAILED TO VALIDATE \n\n", i, currentDemoProg->program->handle->name);
	}

	// if linking fails, contingency plan goes here
	// otherwise, release shaders
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;
		a3shaderRelease(shaderPtr->shader);
	}


	// prepare uniforms algorithmically instead of manually for all programs
	// get uniform and uniform block locations and set default values for all 
	//	programs that have a uniform that will either never change or is
	//	consistent for all programs
	for (i = 0; i < demoStateMaxCount_shaderProgram; ++i)
	{
		// activate program
		currentDemoProg = demoState->shaderProgram + i;
		a3shaderProgramActivate(currentDemoProg->program);

		// common VS
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVP);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMV);
		a3demo_setUniformDefaultMat4(currentDemoProg, uP);
		a3demo_setUniformDefaultMat4(currentDemoProg, uP_inv);
		a3demo_setUniformDefaultMat4(currentDemoProg, uPB);
		a3demo_setUniformDefaultMat4(currentDemoProg, uPB_inv);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMV_nrm);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVPB);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVPB_other);
		a3demo_setUniformDefaultMat4(currentDemoProg, uAtlas);

		// common texture
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_dm, defaultTexUnits + 0);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_sm, defaultTexUnits + 1);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_nm, defaultTexUnits + 2);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_hm, defaultTexUnits + 3);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_ramp_dm, defaultTexUnits + 4);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_ramp_sm, defaultTexUnits + 5);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage00, defaultTexUnits + 0);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage01, defaultTexUnits + 1);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage02, defaultTexUnits + 2);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage03, defaultTexUnits + 3);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage04, defaultTexUnits + 4);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage05, defaultTexUnits + 5);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage06, defaultTexUnits + 6);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage07, defaultTexUnits + 7);

		// common general
		a3demo_setUniformDefaultInteger(currentDemoProg, uIndex, defaultInt);
		a3demo_setUniformDefaultInteger(currentDemoProg, uCount, defaultInt);
		a3demo_setUniformDefaultDouble(currentDemoProg, uAxis, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uSize, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uFlag, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uTime, defaultDouble);
		a3demo_setUniformDefaultVec4(currentDemoProg, uColor0, a3vec4_one.v);
		a3demo_setUniformDefaultVec4(currentDemoProg, uColor, a3vec4_one.v);

		// transformation uniform blocks
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformStack, 0);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformBlend, 1);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformMVP, 0);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformMVPB, 1);
	}


	// allocate uniform buffers
	a3bufferCreate(demoState->ubo_transformStack, "ubo:transformStack", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_transformBlend, "ubo:transformBlend", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_transformMVP, "ubo:transformMVP", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_transformMVPB, "ubo:transformMVPB", a3buffer_uniform, a3index_countMaxShort, 0);
	for (i = 0; i < 4; ++i)
		a3bufferCreate(demoState->ubo_transformSkelMVP + i, "ubo:transformSkelMVP", a3buffer_uniform, a3index_countMaxShort, 0);


	printf("\n\n---------------- LOAD SHADERS FINISHED ---------------- \n");

	//done
	a3shaderProgramDeactivate();
	a3vertexDrawableDeactivate();
}


// utility to load textures
void a3demo_loadTextures(a3_DemoState* demoState)
{	
	// indexing
	a3_Texture* tex;
	a3ui32 i;

	// structure for texture loading
	typedef struct a3_TAG_DEMOSTATETEXTURE {
		a3_Texture* texture;
		a3byte textureName[32];
		const a3byte* filePath;
	} a3_DemoStateTexture;

	// texture objects
	union {
		struct {
			a3_DemoStateTexture texSkyClouds[1];
			a3_DemoStateTexture texSkyWater[1];
			a3_DemoStateTexture texRampDM[1];
			a3_DemoStateTexture texRampSM[1];
			a3_DemoStateTexture texTestSprite[1];
			a3_DemoStateTexture texChecker[1];
		};
	} textureList = {
		{
			{ demoState->tex_skybox_clouds,	"tex:sky-clouds",	"../../../../resource/tex/bg/sky_clouds.png" },
			{ demoState->tex_skybox_water,	"tex:sky-water",	"../../../../resource/tex/bg/sky_water.png" },
			{ demoState->tex_ramp_dm,		"tex:ramp-dm",		"../../../../resource/tex/sprite/celRamp_dm.png" },
			{ demoState->tex_ramp_sm,		"tex:ramp-sm",		"../../../../resource/tex/sprite/celRamp_sm.png" },
			{ demoState->tex_testsprite,	"tex:testsprite",	"../../../../resource/tex/sprite/spriteTest8x8.png" },
			{ demoState->tex_checker,		"tex:checker",		"../../../../resource/tex/sprite/checker.png" },
		}
	};
	const a3ui32 numTextures = sizeof(textureList) / sizeof(a3_DemoStateTexture);
	a3_DemoStateTexture* const textureListPtr = (a3_DemoStateTexture*)(&textureList), * texturePtr;

	// load all textures
	for (i = 0; i < numTextures; ++i)
	{
		texturePtr = textureListPtr + i;
		a3textureCreateFromFile(texturePtr->texture, texturePtr->textureName, texturePtr->filePath);
		a3textureActivate(texturePtr->texture, a3tex_unit00);
		a3textureDefaultSettings();
	}

	// change settings on a per-texture or per-type basis
	tex = demoState->texture;
	// skyboxes
	for (i = 0; i < 2; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear);	// linear pixel blending
	}
	// ramps
	for (i = 0; i < 2; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);	// clamp both axes
	}


	// done
	a3textureDeactivate(a3tex_unit00);
}


// utility to load framebuffers
void a3demo_loadFramebuffers(a3_DemoState* demoState)
{
	// create framebuffers and change their texture settings if need be
	a3_Framebuffer* fbo;
	a3ui32 i, j;

	// frame sizes
	const a3ui16 frameWidth1 = demoState->frameWidth, frameHeight1 = demoState->frameHeight;

	// storage precision and targets
	const a3_FramebufferColorType colorType_scene = a3fbo_colorRGBA16;
	const a3_FramebufferDepthType depthType_scene = a3fbo_depth24_stencil8;
	const a3ui32 targets_scene = 1;
	const a3_FramebufferColorType colorType_composite = a3fbo_colorRGBA16;
	const a3_FramebufferDepthType depthType_composite = a3fbo_depthDisable;
	const a3ui32 targets_composite = 1;


	// initialize framebuffers: 
	//	-> scene, with or without MRT (determine your needs), add depth
	fbo = demoState->fbo_scene_c16d24s8_mrt;
	a3framebufferCreate(fbo, "fbo:scene",
		targets_scene, colorType_scene, depthType_scene,
		frameWidth1, frameHeight1);

	//  -> compositing
	fbo = demoState->fbo_composite_c16;
	a3framebufferCreate(fbo, "fbo:composite",
		targets_composite, colorType_composite, depthType_composite,
		frameWidth1, frameHeight1);


	// change texture settings for all framebuffers
	for (i = 0, fbo = demoState->framebuffer;
		i < demoStateMaxCount_framebuffer;
		++i, ++fbo)
	{
		// color, if applicable
		for (j = 0; j < fbo->color; ++j)
		{
			a3framebufferBindColorTexture(fbo, a3tex_unit00, j);
			a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);
			a3textureChangeFilterMode(a3tex_filterLinear);
		}

		// depth, if applicable
		if (fbo->depthStencil)
		{
			a3framebufferBindDepthTexture(fbo, a3tex_unit00);
			a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);
			a3textureChangeFilterMode(a3tex_filterLinear);
		}
	}


	// deactivate texture
	a3textureDeactivate(a3tex_unit00);
}


//-----------------------------------------------------------------------------

// internal utility for refreshing drawable
inline void a3_refreshDrawable_internal(a3_VertexDrawable *drawable, a3_VertexArrayDescriptor *vertexArray, a3_IndexBuffer *indexBuffer)
{
	drawable->vertexArray = vertexArray;
	if (drawable->indexType)
		drawable->indexBuffer = indexBuffer;
}


// the handle release callbacks are no longer valid; since the library was 
//	reloaded, old function pointers are out of scope!
// could reload everything, but that would mean rebuilding GPU data...
//	...or just set new function pointers!
void a3demo_loadValidate(a3_DemoState* demoState)
{
	a3_BufferObject* currentBuff = demoState->drawDataBuffer,
		* const endBuff = currentBuff + demoStateMaxCount_drawDataBuffer;
	a3_VertexArrayDescriptor* currentVAO = demoState->vertexArray,
		* const endVAO = currentVAO + demoStateMaxCount_vertexArray;
	a3_DemoStateShaderProgram* currentProg = demoState->shaderProgram,
		* const endProg = currentProg + demoStateMaxCount_shaderProgram;
	a3_UniformBuffer* currentUBO = demoState->uniformBuffer,
		* const endUBO = currentUBO + demoStateMaxCount_uniformBuffer;
	a3_Texture* currentTex = demoState->texture,
		* const endTex = currentTex + demoStateMaxCount_texture;
	a3_Framebuffer* currentFBO = demoState->framebuffer,
		* const endFBO = currentFBO + demoStateMaxCount_framebuffer;

	// set pointers to appropriate release callback for different asset types
	while (currentBuff < endBuff)
		a3bufferHandleUpdateReleaseCallback(currentBuff++);
	while (currentVAO < endVAO)
		a3vertexArrayHandleUpdateReleaseCallback(currentVAO++);
	while (currentProg < endProg)
		a3shaderProgramHandleUpdateReleaseCallback((currentProg++)->program);
	while (currentUBO < endUBO)
		a3bufferHandleUpdateReleaseCallback(currentUBO++);
	while (currentTex < endTex)
		a3textureHandleUpdateReleaseCallback(currentTex++);
	while (currentFBO < endFBO)
		a3framebufferHandleUpdateReleaseCallback(currentFBO++);

	// re-link specific object pointers for different asset types
	currentBuff = demoState->vbo_staticSceneObjectDrawBuffer;

	currentVAO = demoState->vao_position_color;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_axes, currentVAO, currentBuff);

	currentVAO = demoState->vao_position;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_grid, currentVAO, currentBuff);

	currentVAO = demoState->vao_tangentbasis_texcoord;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_unit_plane_z, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_box, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_sphere, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_cylinder, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_capsule, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unit_torus, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_teapot, currentVAO, currentBuff);

	currentVAO = demoState->vao_tangentbasis_texcoord_skin;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_character_skin, currentVAO, currentBuff);

	currentVAO = demoState->vao_tangentbasis_texcoord_morph5;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_teapot_morph, currentVAO, currentBuff);

	a3demo_initDummyDrawable_internal(demoState);
}


//-----------------------------------------------------------------------------
