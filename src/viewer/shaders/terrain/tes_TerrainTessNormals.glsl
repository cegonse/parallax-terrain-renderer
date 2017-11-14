// Terrain Viewer
// Tessellation evaluation shader, file: tes_TerrainTessNormals.glsl
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
out vec3 teNormal;
out vec3 teEyeVector;
out mat3 teTbnMatrix;
out vec2 teVertEye;

// World-space eye position, used for parallax mapping
uniform vec3 uEyeWorldPosition;
uniform mat4 uMvpMatrix;
uniform mat4 uModelMatrix;
uniform sampler2D uHeightmap;
uniform float uHeightScale;
uniform int uSubdivisionCount;

void main() 
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float x = gl_in[0].gl_Position.x;
	float z = gl_in[0].gl_Position.z;

	vec4 T, B, N;
	vec4 pc;
	vec2 uvc;
	vec4 heightNormal;

	uvc = (gl_TessCoord.xy + gl_in[0].gl_Position.xz) / (uSubdivisionCount + 1);

	heightNormal = texture2D(uHeightmap, uvc);

	pc.xz = uvc - 0.5f;
	pc.y = uHeightScale * heightNormal.r;
	pc.w = 1.0f;

	N.xyz = (heightNormal.gba * 2f) - 1f;
	T.xyz = cross(N.xyz, vec3(0,0,1));
	B.xyz = cross(N.xyz, vec3(1,0,0));


	// TBN matrix
	teTbnMatrix = mat3(T.xyz, B.xyz, N.xyz);

	// Eye vector (in tangent space)
	teEyeVector = teTbnMatrix * (normalize((uModelMatrix * pc).xyz - uEyeWorldPosition));

	// Normal vector (in tangent space)
	teNormal = N.xyz;

	// Eye position (in tangent space)
	vec3 eyeTangentPosition = teTbnMatrix * uEyeWorldPosition;

	teUvCoordinates = uvc;
	teVertEye.s = heightNormal.r;
	teVertEye.t = eyeTangentPosition.y;

	gl_Position = uMvpMatrix * pc;
}
