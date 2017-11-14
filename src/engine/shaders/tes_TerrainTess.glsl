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
	vec4 pc;
	vec2 uvc = (gl_TessCoord.xy + gl_in[0].gl_Position.xz) / (uSubdivisionCount + 1);
	vec4 heightNormal = texture(uHeightmap, uvc);

	pc.xz = uvc - 0.5f;
	pc.y = uHeightScale * heightNormal.r;
	pc.w = 1.0f;

	teUvCoordinates = uvc;

	gl_Position = uMvpMatrix * pc;
}
