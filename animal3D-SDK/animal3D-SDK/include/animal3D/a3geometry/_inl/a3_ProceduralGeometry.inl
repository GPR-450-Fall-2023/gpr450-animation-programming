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
	
	a3_ProceduralGeometry.inl
	Inline definitions for procedural geometry generation.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_PROCEDURALGEOMETRY_H
#ifndef __ANIMAL3D_PROCEDURALGEOMETRY_INL
#define __ANIMAL3D_PROCEDURALGEOMETRY_INL


//-----------------------------------------------------------------------------

A3_INLINE a3ret a3proceduralGetDescriptorFlags(const a3_ProceduralGeometryDescriptor *geom)
{
	if (geom && geom->shape)
		return (geom->bParams[3] & 0x0f);
	return -1;
}

A3_INLINE a3ret a3proceduralGetDescriptorAxis(const a3_ProceduralGeometryDescriptor *geom)
{
	if (geom && geom->shape)
		return (geom->bParams[3] & 0xf0) >> 4;
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_PROCEDURALGEOMETRY_INL
#endif	// __ANIMAL3D_PROCEDURALGEOMETRY_H