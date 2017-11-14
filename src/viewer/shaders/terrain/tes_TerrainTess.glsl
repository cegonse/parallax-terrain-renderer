// Terrain Viewer
// Tessellation evaluation shader, file: tes_TerrainTess.glsl
//
// - This shader program calculates the height of every
//	 vertex coming from the vertex generator stage in the
//	 tessellation pipeline using a heightmap texture.
//
// Copyright (c) César González Segura 2015, ETSE-UV
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library.
//

#version 430 core

// Tessellation evaluation shader set-up:
// - Output vertices generated as quads.
// - Equally space the output vertices
// - Use counter clockwise order
layout (quads, equal_spacing, ccw) in;

// Input UV coordinates back from the tessellation control
// shader, previously uploaded from the client
in vec2 tcUvCoordinates[];

// Output UV coordinates, interpolated using the tessellation
// patch coordinates
out vec2 teUvCoordinates;

// World-space eye position, used for parallax mapping
uniform mat4 uMvpMatrix;
uniform sampler2D uHeightmap;
uniform float uHeightScale;
uniform int uSubdivisionCount;

void main() 
{
	vec4 centerVertex;
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float x = gl_in[0].gl_Position.x;
	float z = gl_in[0].gl_Position.z;

	teUvCoordinates.s = (u + x) / (uSubdivisionCount + 1);
	teUvCoordinates.t = (v + z) / (uSubdivisionCount + 1);

	centerVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
	centerVertex.y = uHeightScale * texture2D(uHeightmap, teUvCoordinates).r;
	centerVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
	centerVertex.w = 1;

	gl_Position = uMvpMatrix * centerVertex;
}
