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
	
	a3_ModelLoader_WavefrontOBJ.h
	Interface for loading models using the Wavefront OBJ file format.
	NOTE: This is not a full implementation; it's just enough to get 
		model data, extended with the capacity to load Maya's XML skin 
		format for added utility.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_MODELLOADER_WAVEFRONTOBJ_H
#define __ANIMAL3D_MODELLOADER_WAVEFRONTOBJ_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"
#include "animal3D-A3DG/a3graphics/a3_VertexDrawable.h"
#include "a3_GeometryData.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef enum a3_ModelLoaderFlag	a3_ModelLoaderFlag;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// A3: Flags for model loader.
	// NOTE: file must have texture coordinates to calculate tangents.
	enum a3_ModelLoaderFlag
	{
		a3model_loadTexcoords = 1,
		a3model_loadNormals,
		a3model_loadNormals_loadTexcoords,
		a3model_calculateFaceNormals,
		a3model_calculateFaceNormals_loadTexcoords,
		a3model_calculateFaceTangents = 13,
		a3model_calculateVertexNormals = 20,
		a3model_calculateVertexNormals_loadTexcoords,
		a3model_calculateVertexTangents = 61,
	};


//-----------------------------------------------------------------------------

	// A3: Load a Wavefront OBJ file's model data.
	//		NOTE: polygons should be no more than 4 sides; only triangles and 
	//		quads are accepted.
	//	param geom_out: non-null pointer to uninitialized geometry data
	//	param filePath: non-null, non-empty cstring of file location
	//	param flags: load options; see above enum
	//	param transform_opt: optional array of 16 floats representing a 
	//		*column-major* transformation matrix for all vertices
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3modelLoadOBJ(a3_GeometryData *geom_out, const a3byte *filePath, const a3_ModelLoaderFlag flags, const a3f32 *transform_opt);

	// A3: Load a Wavefront OBJ file's model data, and a text skin weights 
	//		file exported from Maya in XML format.
	//		NOTE: polygons should be no more than 4 sides; only triangles and 
	//		quads are accepted. Also, only 4 skin influences per vertex are 
	//		accepted; influences will be normalized.
	//	param geom_out: non-null pointer to uninitialized geometry data
	//	param filePath: non-null, non-empty cstring of file location
	//	param flags: load options; see above enum
	//	param weightsFilePath: valid cstring of XML file path; if invalid, 
	//		the model will be loaded without weights
	//	param influenceNames: non-null array of valid cstrings containing the 
	//		names of the hierarchy corresponding with the weights
	//	param numInfluences: non-zero influence count
	//	param transform_opt: optional array of 16 floats representing a 
	//		*column-major* transformation matrix for all vertices
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3modelLoadOBJSkinWeights(a3_GeometryData *geom_out, const a3byte *filePath, const a3_ModelLoaderFlag flags, const a3byte *weightsFilePath, const a3byte *influenceNames[], const a3ui32 numInfluences, const a3f32 *transform_opt);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_ModelLoader_WavefrontOBJ.inl"


#endif	// !__ANIMAL3D_MODELLOADER_WAVEFRONTOBJ_H