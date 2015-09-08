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

// Depending on the desired height resolution, the input
// data may use 8, 16 or 24 bits per sample
#define TERRAIN_FORMAT_RED_CHANNEL 0
#define TERRAIN_FORMAT_RED_GREEN_CHANNEL 1
#define TERRAIN_FORMAT_RED_GREEN_BLUE_CHANNEL 2

// Tessellation evaluation shader set-up:
// - Output vertices generated as quads.
// - Equally space the output vertices
// - Use counter clockwise order
layout (quads, equal_spacing, ccw) in;

// Input UV coordinates back from the tessellation control
// shader, previously uploaded from the client
in vec2 tcUvCoordinates[];
// Input bounding box for the tile
in vec2 tcBoundingBox[];

// Output UV coordinates, interpolated using the tessellation
// patch coordinates
out vec3 teUvCoordinates;
// Output bounding box for the tile
out vec2 teBoundingBox;
out vec3 teNormal;

// Output eye-vertex vector, used for parallax mapping
out vec3 teEyeVector;
// World-space eye position, used for parallax mapping
uniform vec3 uEyeWorldPosition;

// Model-view-projection matrix
uniform mat4 uMvpMatrix;
// Tile model-view matrix
uniform mat4 uModelMatrix;

// Heightmap texture data
uniform sampler2D uHeightmap;

// Scale for the heightmap data
uniform float uHeightScale;
// Format used for the texture (8, 16 or 24 bits per sample)
uniform int uColorFormat;
// Number of patches used for the tessellation
uniform int uSubdivisionCount;

void main() 
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float x = gl_in[0].gl_Position.x;
	float z = gl_in[0].gl_Position.z;

	teUvCoordinates.s = (u + x) / (uSubdivisionCount + 1);
	teUvCoordinates.t = (v + z) / (uSubdivisionCount + 1);

	// Normal vector calculation
	float delta = 1 / gl_TessLevelInner[0];
	
	// Center Vertex
	vec2 centerCoords = teUvCoordinates.st;
	vec3 centerVertex;
	centerVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
	centerVertex.y = uHeightScale * texture2D(uHeightmap, centerCoords).r;
	centerVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;

	// Left Vertex
	vec2 leftCoords = teUvCoordinates.st;
	leftCoords.s -= delta;
	leftCoords = clamp(leftCoords, 0, 1);

	vec3 leftVertex;
	leftVertex.x = ((u + x - delta) / (uSubdivisionCount + 1)) - 0.5f;
	leftVertex.y = uHeightScale * texture2D(uHeightmap, leftCoords).r;
	leftVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;

	// Up Vertex
	vec2 upCoords = teUvCoordinates.st;
	upCoords.t += delta;
	upCoords = clamp(upCoords, 0, 1);

	vec3 upVertex;
	upVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
	upVertex.y = uHeightScale * texture2D(uHeightmap, upCoords).r;
	upVertex.z = ((v + z + delta) / (uSubdivisionCount + 1)) - 0.5f;

	// Right Vertex
	vec2 rightCoords = teUvCoordinates.st;
	rightCoords.s += delta;
	rightCoords = clamp(rightCoords, 0, 1);

	vec3 rightVertex;
	rightVertex.x = ((u + x + delta) / (uSubdivisionCount + 1)) - 0.5f;
	rightVertex.y = uHeightScale * texture2D(uHeightmap, rightCoords).r;
	rightVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;

	// Down Vertex
	vec2 downCoords = teUvCoordinates.st;
	downCoords.t -= delta;
	downCoords = clamp(downCoords, 0, 1);

	vec3 downVertex;
	downVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
	downVertex.y = uHeightScale * texture2D(uHeightmap, downCoords).r;
	downVertex.z = ((v + z - delta) / (uSubdivisionCount + 1)) - 0.5f;

	// Normal vector calculation
	/*vec3 centerLeft = leftVertex - centerVertex;
	vec3 centerUp = upVertex - centerVertex;
	vec3 centerRight = rightVertex - centerVertex;
	vec3 centerDown = downVertex - centerVertex;

	vec3 nUpLeft = cross(centerUp, centerLeft);
	vec3 nLeftDown = cross(centerLeft, centerDown);
	vec3 nDownRight = cross(centerDown, centerRight);
	vec3 nRightUp = cross(centerRight, centerUp);

	teNormal = normalize((nUpLeft + nLeftDown + nDownRight + nRightUp) / 4);*/

	// VALID
	vec3 leftRight = rightVertex - leftVertex;
	vec3 downUp = upVertex - downVertex;
	teNormal = normalize(cross(leftRight, downUp));

	gl_Position.xyz = centerVertex;
	gl_Position.w = 1;

	// Calculation of the vertex-eye vector for per-fragment
	// parallax mapping
	vec3 worldSpaceVxPos = (uModelMatrix * gl_Position).xyz;
	teEyeVector = worldSpaceVxPos - uEyeWorldPosition;
	teUvCoordinates.z = texture2D(uHeightmap, teUvCoordinates.st).r;

	// Pass-through bounding box
	teBoundingBox = tcBoundingBox[0];

    // Vertex position translation to screen-space
    gl_Position = uMvpMatrix * gl_Position;
}
