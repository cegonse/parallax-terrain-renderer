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
in vec2 tcBoundingBox[];

// Output UV coordinates, interpolated using the tessellation
// patch coordinates
out vec3 teUvCoordinates;
out vec2 teBoundingBox;
out vec3 teNormal;
out vec3 teEyeVector;
out mat3 teTbnMatrix;

// World-space eye position, used for parallax mapping
uniform vec3 uEyeWorldPosition;
uniform mat4 uMvpMatrix;
uniform mat4 uModelMatrix;
uniform sampler2D uHeightmap;
uniform float uHeightScale;
uniform int uSubdivisionCount;

void main() 
{
	/*float u = gl_TessCoord.x;
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

	vec3 p0 = centerVertex;
	vec3 p1 = upVertex;
	vec3 p2 = rightVertex;

	float u0 = centerCoords.s;
	float u1 = upCoords.s;
	float u2 = rightCoords.s;

	float v0 = centerCoords.t;
	float v1 = upCoords.t;
	float v2 = rightCoords.t;

	vec3 T = normalize(
			 ((v1-v0)*(p2-p0)-(v2-v0)*(p1-p0))
			  /
			 ((u2-u0)*(v1-v0)-(u1-u0)*(v2-v0))
			 );
	
	vec3 B = normalize(
		     ((u1-u0)*(p2-p0)-(u2-u0)*(p1-p0))
			 /
			 ((u1-u0)*(v2-v0)-(u2-u0)*(v1-v0))
			 );
	
	vec3 N = normalize(cross(T, B));

	mat3 TBN = inverse(mat3(N,B,T));

	teEyeVector = TBN * teEyeVector;
	teNormal = N;

	// Pass-through bounding box
	teBoundingBox = tcBoundingBox[0];

    // Vertex position translation to screen-space
    gl_Position = uMvpMatrix * gl_Position;*/

	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float x = gl_in[0].gl_Position.x;
	float z = gl_in[0].gl_Position.z;

	vec4 centerVertex, upVertex, rightVertex, downVertex, leftVertex;
	vec2 centerCoords, upCoords, rightCoords, downCoords, leftCoords;

	centerCoords.s = (u + x) / (uSubdivisionCount + 1);
	centerCoords.t = (v + z) / (uSubdivisionCount + 1);

	centerVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
	centerVertex.y = uHeightScale * texture2D(uHeightmap, centerCoords).r;
	centerVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
	centerVertex.w = 1;

	vec4 T,B,N;

	if (u == 0 && v < 1 && v > 0)
	{
		vec4 upVertex, rightVertex;
		vec2 upCoords, rightCoords;

		upCoords.s = (u + x) / (uSubdivisionCount + 1);
		upCoords.t = (v + z + (1/(1+gl_TessLevelOuter[0]))) / (uSubdivisionCount + 1);

		upVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.y = uHeightScale * texture2D(uHeightmap, upCoords).r;
		upVertex.z = ((v + z + (1/(1+gl_TessLevelOuter[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.w = 1;

		rightCoords.s = (u + x + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1);
		rightCoords.t = (v + z) / (uSubdivisionCount + 1);

		rightVertex.x = ((u + x + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.y = uHeightScale * texture2D(uHeightmap, rightCoords).r;
		rightVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.w = 1;

		T = upVertex - centerVertex;
		B = rightVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else if (u < 1 && u > 0 && v == 0)
	{
		vec4 upVertex, rightVertex;
		vec2 upCoords, rightCoords;

		upCoords.s = (u + x) / (uSubdivisionCount + 1);
		upCoords.t = (v + z + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1);

		upVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.y = uHeightScale * texture2D(uHeightmap, upCoords).r;
		upVertex.z = ((v + z + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.w = 1;

		rightCoords.s = (u + x + (1/(1+gl_TessLevelOuter[1]))) / (uSubdivisionCount + 1);
		rightCoords.t = (v + z) / (uSubdivisionCount + 1);

		rightVertex.x = ((u + x + (1/(1+gl_TessLevelOuter[1]))) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.y = uHeightScale * texture2D(uHeightmap, rightCoords).r;
		rightVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.w = 1;

		T = upVertex - centerVertex;
		B = rightVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else if (u == 1 && v < 1 && v > 0)
	{
		vec4 upVertex, leftVertex;
		vec2 upCoords, leftCoords;

		upCoords.s = (u + x) / (uSubdivisionCount + 1);
		upCoords.t = (v + z + (1/(1+gl_TessLevelOuter[2]))) / (uSubdivisionCount + 1);

		upVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.y = uHeightScale * texture2D(uHeightmap, upCoords).r;
		upVertex.z = ((v + z + (1/(1+gl_TessLevelOuter[2]))) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.w = 1;

		leftCoords.s = (u + x - (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1);
		leftCoords.t = (v + z) / (uSubdivisionCount + 1);

		leftVertex.x = ((u + x - (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		leftVertex.y = uHeightScale * texture2D(uHeightmap, leftCoords).r;
		leftVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		leftVertex.w = 1;

		T = upVertex - centerVertex;
		B = leftVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else if (u < 1 && u > 0 && v == 1)
	{
		vec4 downVertex, rightVertex;
		vec2 downCoords, rightCoords;

		downCoords.s = (u + x) / (uSubdivisionCount + 1);
		downCoords.t = (v + z - (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1);

		downVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		downVertex.y = uHeightScale * texture2D(uHeightmap, downCoords).r;
		downVertex.z = ((v + z - (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		downVertex.w = 1;

		rightCoords.s = (u + x + (1/(1+gl_TessLevelOuter[3]))) / (uSubdivisionCount + 1);
		rightCoords.t = (v + z) / (uSubdivisionCount + 1);

		rightVertex.x = ((u + x + (1/(1+gl_TessLevelOuter[3]))) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.y = uHeightScale * texture2D(uHeightmap, rightCoords).r;
		rightVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.w = 1;

		T = downVertex - centerVertex;
		B = rightVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else if (u == 0 && v == 0)
	{
		vec4 upVertex, rightVertex;
		vec2 upCoords, rightCoords;

		upCoords.s = (u + x) / (uSubdivisionCount + 1);
		upCoords.t = (v + z + (1/(1+gl_TessLevelOuter[0]))) / (uSubdivisionCount + 1);

		upVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.y = uHeightScale * texture2D(uHeightmap, upCoords).r;
		upVertex.z = ((v + z + (1/(1+gl_TessLevelOuter[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.w = 1;

		rightCoords.s = (u + x + (1/(1+gl_TessLevelOuter[1]))) / (uSubdivisionCount + 1);
		rightCoords.t = (v + z) / (uSubdivisionCount + 1);

		rightVertex.x = ((u + x + (1/(1+gl_TessLevelOuter[1]))) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.y = uHeightScale * texture2D(uHeightmap, rightCoords).r;
		rightVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.w = 1;

		T = upVertex - centerVertex;
		B = rightVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else if (u == 1 && v == 0)
	{
		vec4 upVertex, leftVertex;
		vec2 upCoords, leftCoords;

		upCoords.s = (u + x) / (uSubdivisionCount + 1);
		upCoords.t = (v + z + (1/(1+gl_TessLevelOuter[2]))) / (uSubdivisionCount + 1);

		upVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.y = uHeightScale * texture2D(uHeightmap, upCoords).r;
		upVertex.z = ((v + z + (1/(1+gl_TessLevelOuter[2]))) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.w = 1;

		leftCoords.s = (u + x - (1/(1+gl_TessLevelOuter[1]))) / (uSubdivisionCount + 1);
		leftCoords.t = (v + z) / (uSubdivisionCount + 1);

		leftVertex.x = ((u + x - (1/(1+gl_TessLevelOuter[1]))) / (uSubdivisionCount + 1)) - 0.5f;
		leftVertex.y = uHeightScale * texture2D(uHeightmap, leftCoords).r;
		leftVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		leftVertex.w = 1;

		T = upVertex - centerVertex;
		B = leftVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else if (u == 0 && v == 1)
	{
		vec4 downVertex, rightVertex;
		vec2 downCoords, rightCoords;

		downCoords.s = (u + x) / (uSubdivisionCount + 1);
		downCoords.t = (v + z - (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1);

		downVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		downVertex.y = uHeightScale * texture2D(uHeightmap, downCoords).r;
		downVertex.z = ((v + z - (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		downVertex.w = 1;

		rightCoords.s = (u + x + (1/(1+gl_TessLevelOuter[3]))) / (uSubdivisionCount + 1);
		rightCoords.t = (v + z) / (uSubdivisionCount + 1);

		rightVertex.x = ((u + x + (1/(1+gl_TessLevelOuter[3]))) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.y = uHeightScale * texture2D(uHeightmap, rightCoords).r;
		rightVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.w = 1;

		T = downVertex - centerVertex;
		B = rightVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else if (u == 1 && v == 1)
	{
		vec4 downVertex, leftVertex;
		vec2 downCoords, leftCoords;

		downCoords.s = (u + x) / (uSubdivisionCount + 1);
		downCoords.t = (v + z - (1/(1+gl_TessLevelOuter[2]))) / (uSubdivisionCount + 1);

		downVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		downVertex.y = uHeightScale * texture2D(uHeightmap, downCoords).r;
		downVertex.z = ((v + z - (1/(1+gl_TessLevelOuter[2]))) / (uSubdivisionCount + 1)) - 0.5f;
		downVertex.w = 1;

		leftCoords.s = (u + x - (1/(1+gl_TessLevelOuter[3]))) / (uSubdivisionCount + 1);
		leftCoords.t = (v + z) / (uSubdivisionCount + 1);

		leftVertex.x = ((u + x - (1/(1+gl_TessLevelOuter[3]))) / (uSubdivisionCount + 1)) - 0.5f;
		leftVertex.y = uHeightScale * texture2D(uHeightmap, leftCoords).r;
		leftVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		leftVertex.w = 1;

		T = downVertex - centerVertex;
		B = leftVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}
	else
	{
		vec4 upVertex, rightVertex;
		vec2 upCoords, rightCoords;

		upCoords.s = (u + x) / (uSubdivisionCount + 1);
		upCoords.t = (v + z + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1);

		upVertex.x = ((u + x) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.y = uHeightScale * texture2D(uHeightmap, upCoords).r;
		upVertex.z = ((v + z + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		upVertex.w = 1;

		rightCoords.s = (u + x + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1);
		rightCoords.t = (v + z) / (uSubdivisionCount + 1);

		rightVertex.x = ((u + x + (1/(1+gl_TessLevelInner[0]))) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.y = uHeightScale * texture2D(uHeightmap, rightCoords).r;
		rightVertex.z = ((v + z) / (uSubdivisionCount + 1)) - 0.5f;
		rightVertex.w = 1;

		T = upVertex - centerVertex;
		B = rightVertex - centerVertex;
		N.xyz = cross(T.xyz,B.xyz);
		N.w = 0;
	}

	// TBN matrix
	T = normalize(T);
	B = normalize(B);
	N = normalize(N);

	teTbnMatrix = mat3(T.xyz,
					   B.xyz,
					   N.xyz);

	// Eye vector (in tangent space)
	vec3 worldSpaceVxPos = (uModelMatrix * centerVertex).xyz;
	teEyeVector = worldSpaceVxPos - uEyeWorldPosition;
	teEyeVector = teTbnMatrix * teEyeVector;
	
	// Normal vector (in tangent space)
	teNormal = N.xyz;

	// Pass-through bounding box
	teBoundingBox = tcBoundingBox[0];

	teUvCoordinates.s = centerCoords.s;
	teUvCoordinates.t = centerCoords.t;
	teUvCoordinates.p = texture2D(uHeightmap, centerCoords).r;
	gl_Position = uMvpMatrix * centerVertex;
}
