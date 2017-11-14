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

// BEGIN_MARIANO: Con mipmapping en el mapa de alturas
in float tcsNivelLod[];
out float tesNivelLod;
// BEGIN_MARIANO: Con mipmapping


// Output UV coordinates, interpolated using the tessellation
// patch coordinates
out vec2 teUvCoordinates;
out vec3 teNormal;
out vec3 teEyeVector;
out float hbase;
out float incUV;

// World-space eye position, used for parallax mapping
uniform vec3 uEyeWorldPosition;
uniform mat4 uMvpMatrix;
uniform mat4 uModelMatrix;
uniform sampler2D uHeightmap;
uniform float uHeightScale;
uniform int uSubdivisionCount;

void main() 
{
	vec4 pc;
	vec2 uvc = (gl_TessCoord.xy + gl_in[0].gl_Position.xz) / (uSubdivisionCount + 1);

// BEGIN_MARIANO: Sin mipmapping en el mapa de alturas
	vec4 heightNormal = texture(uHeightmap, uvc);
// END_MARIANO: Sin mipmapping
// BEGIN_MARIANO: Con mipmapping
	//tesNivelLod = tcsNivelLod[0];
	//vec4 heightNormal = textureLod(uHeightmap, uvc, tesNivelLod);
// END_MARIANO: Con mipmapping

	pc.xz = uvc - 0.5f;
	pc.y = uHeightScale * heightNormal.r;
	pc.w = 1.0f;

	//Modifica las componentes de la normal pq no están en el mismo S.R. que el Mundo (están rotadas 90º)
	teNormal = normalize(heightNormal.gba * 2f - 1f); // Normal vector (in world space)

	// Eye vector (in world space)
	teEyeVector = normalize(uEyeWorldPosition - vec3(uModelMatrix * pc));

	teUvCoordinates = uvc;
	
	hbase = heightNormal.r;

	incUV = 1.0 / (gl_TessLevelInner[0] * (uSubdivisionCount + 1));


	gl_Position = uMvpMatrix * pc;
}
