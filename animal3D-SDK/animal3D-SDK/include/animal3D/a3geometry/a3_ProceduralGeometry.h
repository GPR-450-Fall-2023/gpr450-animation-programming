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
	
	a3_ProceduralGeometry.h
	Interface for procedural geometry generation.

	**DO NOT MODIFY THIS FILE**
*/

#ifndef __ANIMAL3D_PROCEDURALGEOMETRY_H
#define __ANIMAL3D_PROCEDURALGEOMETRY_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3/a3types_real.h"
#include "animal3D-A3DG/a3graphics/a3_VertexDrawable.h"
#include "a3_GeometryData.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_ProceduralGeometryDescriptor		a3_ProceduralGeometryDescriptor;
	typedef enum a3_ProceduralGeometryShape				a3_ProceduralGeometryShape;
	typedef enum a3_ProceduralGeometryFlag				a3_ProceduralGeometryFlag;
	typedef enum a3_ProceduralGeometryAxis				a3_ProceduralGeometryAxis;
#endif	// __cplusplus


//-----------------------------------------------------------------------------
	// descriptors & lists

	// A3: Enumeration of procedural geometry shapes.
	enum a3_ProceduralGeometryShape
	{
		a3geomShape_none,
		a3geomShape_triangle,	// not indexed
		a3geomShape_circle,		// indexed if solid or using subdivisions
		a3geomShape_plane,		// indexed only if using subdivisions
		a3geomShape_pyramid,	// indexed
		a3geomShape_octahedron, // indexed if wireframe or only positions
		a3geomShape_box,		// indexed
		a3geomShape_semisphere,	// indexed
		a3geomShape_sphere,		// indexed
		a3geomShape_cone,		// indexed
		a3geomShape_diamond,	// indexed
		a3geomShape_cylinder,	// indexed
		a3geomShape_capsule,	// indexed
		a3geomShape_torus,		// indexed
		a3geomShape_axes,		// indexed, can hold color
	};

	// A3: Number of shapes.
	enum a3_ProceduralGeometryShapeCount
	{
		a3geomShape_count = 14
	};


	// A3: Enumeration of procedural configuration flags.
	enum a3_ProceduralGeometryFlag
	{
		a3geomFlag_vanilla,				// positions only
		a3geomFlag_texcoords,			// positions and texture coordinates
		a3geomFlag_normals,				// positions and normals
		a3geomFlag_texcoords_normals,	// positions, texcoords, normals
		a3geomFlag_tangents = 7,		// also enables normals and texcoords
		a3geomFlag_wireframe,			// back to positions only
	};


	// A3: Enumeration of default axis directions.
	enum a3_ProceduralGeometryAxis
	{
		a3geomAxis_x,
		a3geomAxis_y,
		a3geomAxis_z,
		a3geomAxis_x_negative,
		a3geomAxis_y_negative,
		a3geomAxis_z_negative,
		a3geomAxis_default = a3geomAxis_z
	};


	// A3: Procedural geometry descriptor for any procedural shape.
	//	member shape: which shape does this describe
	//	members bParams, fParams: internal generation parameters
	struct a3_ProceduralGeometryDescriptor
	{
		a3_ProceduralGeometryShape shape;
		a3ubyte bParams[4];
		a3f32 fParams[4];
	};


//-----------------------------------------------------------------------------
	// A3: Create descriptors.
	// NOTE: for all shapes, entering invalid params will not result in the 
	//	geometry failing, but will use internally-set defaults; i.e. if you 
	//	want the defaults, use values outside the specified ranges.

	// A3: Create triangle descriptor (geometry is not indexed).
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape (plane normal for 2D)
	//	params base, height: positive dimensions of triangle
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorTriangle(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 base, a3f32 height);

	// A3: Create circle descriptor (geometry is not indexed).
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape (plane normal for 2D)
	//	param radius: positive radius of circle
	//	param slices: number of divisions; defines circle resolution; 
	//		must be 3 or greater, less than 256
	//	param subdivsBaseRings: number subdivisions from center of 
	//		circle to edge; must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorCircle(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3ubyte slices, a3ubyte subdivsBaseRings);

	// A3: Create plane descriptor (geometry is indexed only if subdivisions).
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape (plane normal for 2D)
	//	params width, height: positive dimensions of plane in space
	//	params subdivsW, subdivsH: subdivisions along the width and height 
	//		dimensions; 1 means the shape is a single-cell basic rectangle; 
	//		must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorPlane(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 width, a3f32 height, a3ubyte subdivsW, a3ubyte subdivsH);

	// A3: Create pyramid descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param width: positive size of square base
	//	param length: positive distance along axis from base to point
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorPyramid(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 width, a3f32 length);

	// A3: Create octahedron descriptor (double-ended pyramid).
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param width: positive size of square center
	//	param length: positive distance along axis from point to point
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorOctahedron(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 width, a3f32 length);

	// A3: Create box descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	params width, height, length: positive dimensions of box in space, 
	//		align with X, Y and Z axes respectively
	//	params subdivsW, subdivsH, subdivsL: subdivisions along dimensions; 
	//		must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorBox(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, a3f32 width, a3f32 height, a3f32 length, a3ubyte subdivsW, a3ubyte subdivsH, a3ubyte subdivsL);

	// A3: Create half-sphere descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param radius: positive radius of sphere
	//	param slices: number of divisions around axis; 
	//		must be 3 or greater, less than 256
	//	param stacks: number of divisions along axis; 
	//		must be 1 or greater, less than 256
	//	param subdivsBaseRings: number subdivisions from center of 
	//		base to edge; must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorSemisphere(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3ubyte slices, a3ubyte stacks, a3ubyte subdivsBaseRings);

	// A3: Create sphere descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param radius: positive radius of sphere
	//	param slices: number of divisions around axis; 
	//		must be 3 or greater, less than 256
	//	param stacks: number of divisions along axis; 
	//		must be 2 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorSphere(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3ubyte slices, a3ubyte stacks);

	// A3: Create cone descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param radius: positive radius of base
	//	param length: positive distance along axis from base to point
	//	param slices: number of divisions around axis; 
	//		must be 3 or greater, less than 256
	//	param subdivsL: number of subdivisions along axis; 
	//		must be 1 or greater, less than 256
	//	param subdivsBaseRings: number subdivisions from center of 
	//		base to edge; must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorCone(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3f32 length, a3ubyte slices, a3ubyte subdivsL, a3ubyte subdivsBaseRings);

	// A3: Create diamond descriptor (double-ended cone).
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param radius: positive radius of base
	//	param length: positive distance along axis from point to point
	//	param slices: number of divisions around axis; 
	//		must be 3 or greater, less than 256
	//	param subdivsPerHalfL: number of subdivisions along axis on 
	//		each half of shape; must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorDiamond(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3f32 length, a3ubyte slices, a3ubyte subdivsPerHalfL);

	// A3: Create cylinder descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param radius: positive radius of body
	//	param length: positive distance along axis from base to base
	//	param slices: number of divisions; 
	//		must be 3 or greater, less than 256
	//	param subdivsL: number of subdivisions along axis; 
	//		must be 1 or greater, less than 256
	//	param subdivsBaseRings: number subdivisions from center of 
	//		base to edge; must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorCylinder(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3f32 length, a3ubyte slices, a3ubyte subdivsL, a3ubyte subdivsBaseRings);

	// A3: Create capsule descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param radius: positive radius of caps and body
	//	param lengthBody: positive distance along axis of cylindrical part
	//		(total length of object is body length + 2x cap radius)
	//	param slices: number of divisions around axis; 
	//		must be 3 or greater, less than 256
	//	param stacksCap: number of divisions along axis for each cap; 
	//		must be 1 or greater, less than 256
	//	param subdivsL: number of body subdivisions along axis; 
	//		must be 1 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorCapsule(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radius, a3f32 lengthBody, a3ubyte slices, a3ubyte stacksCap, a3ubyte subdivsL);

	// A3: Create torus descriptor (donut).
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param axis: axis direction for shape
	//	param radiusMajor: positive radius from the center of the object to 
	//		the "core" ring of the actual geometry
	//	param radiusMinor: positive radius of the ring itself; the distance 
	//		from the "core" to the surface
	//	param slices: number of divisions around axis; 
	//		must be 3 or greater, less than 256
	//	param subdivsR: number of divisions on surface around the core; 
	//		must be 3 or greater, less than 256
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorTorus(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis, a3f32 radiusMajor, a3f32 radiusMinor, a3ubyte slices, a3ubyte subdivsR);

	// A3: Create coordinate axes descriptor.
	//	param geom_out: non-null pointer to descriptor
	//	param flags: configuration flags
	//	param thickness: thickness of axes to change visibility
	//		must be greater than 0 and not greater than 1
	//	param color (binary): enable color attribute in any mode with half 
	//		transparency; requires a different VAO from all other shapes if 
	//		this is enabled
	//	return: 1 if success
	//	return: 0 if failed
	//	return: -1 if invalid params
	a3ret a3proceduralCreateDescriptorAxes(a3_ProceduralGeometryDescriptor *geom_out, const a3_ProceduralGeometryFlag flags, a3f32 thickness, const a3byte color);


//-----------------------------------------------------------------------------
	// descriptor getters and setters

	// A3: Get the flags from a shape descriptor.
	//	param geom: non-null pointer to initialized descriptor
	//	return: flag value (see enum)
	//	return: -1 if invalid params
	a3ret a3proceduralGetDescriptorFlags(const a3_ProceduralGeometryDescriptor *geom);

	// A3: Get the axis direction from a shape descriptor.
	//	param geom: non-null pointer to initialized descriptor
	//	return: axis direction (see enum)
	//	return: -1 if invalid params
	a3ret a3proceduralGetDescriptorAxis(const a3_ProceduralGeometryDescriptor *geom);

	// A3: Set the flags of a shape descriptor (retains axis direction).
	//	param geom: non-null pointer to initialized descriptor
	//	param flags: configuration flags
	//	return: 1 if success
	//	return: -1 if invalid params
	a3ret a3proceduralSetDescriptorFlags(a3_ProceduralGeometryDescriptor *geom, const a3_ProceduralGeometryFlag flags);

	// A3: Set the axis direction of a shape descriptor (retains flags).
	//	param geom: non-null pointer to initialized descriptor
	//	param axis: axis direction to set
	//	return: 1 if success
	//	return: 0 if shape does not use axis
	//	return: -1 if invalid params
	a3ret a3proceduralSetDescriptorAxis(a3_ProceduralGeometryDescriptor *geom, const a3_ProceduralGeometryAxis axis);

	// A3: Set the flags and axis direction of a shape descriptor.
	//	param geom: non-null pointer to initialized descriptor
	//	param flags: configuration flags
	//	param axis: axis direction to set
	//	return: 1 if success
	//	return: 0 if shape does not use axis
	//	return: -1 if invalid params
	a3ret a3proceduralSetDescriptorFlagsAxis(a3_ProceduralGeometryDescriptor *geom, const a3_ProceduralGeometryFlag flags, const a3_ProceduralGeometryAxis axis);


//-----------------------------------------------------------------------------

	// A3: The 'go' button: generate data for a procedural shape.
	//	param geomData_out: non-null pointer to uninitialized geometry data
	//	param geom: non-null pointer to initialized procedural shape descriptor
	//	param transform_opt: optional array of 16 floats representing a 
	//		*column-major* transformation matrix for all vertices
	//	return: 1 if success
	//	return: -1 if invalid params
	a3ret a3proceduralGenerateGeometryData(a3_GeometryData *geomData_out, const a3_ProceduralGeometryDescriptor *geom, const a3f32 *transform_opt);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_ProceduralGeometry.inl"


#endif	// !__ANIMAL3D_PROCEDURALGEOMETRY_H