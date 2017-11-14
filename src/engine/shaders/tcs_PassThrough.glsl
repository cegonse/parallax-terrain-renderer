// Terrain Viewer
// Tessellation control shader, file: tcs_PassThrough.glsl
//
// - This shader program calculates a dynamic level-of-detail
//   depending on the distance from the eye of the camera to
//   the world position of the tile, and passes the input from
//   the vertex shader through unmodified.
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

// Tessellation control shader set-up: output 1 vertex per patch
layout (vertices = 1) out;

// Input UV texture coordinates from the vertex shader
in vec2 vUvCoordinates[];

// Output UV texture coordinates to the tessellation evaluation shader
out vec2 tcUvCoordinates[];

// BEGIN_MARIANO: Con mipmapping en el mapa de alturas
out float tcsNivelLod[];
const int NumNiveles = 6; // Numero máximo de niveles variable
// END_MARIANO: Con mipmapping

// Controllable maximum level-of-detail from the client. To achieve the
// maximum level-of-detail for the machine, set it to GL_MAX_TESS_GEN_LEVEL
uniform float uLevelOfDetail;
// Position in world-space coordinates of the camera eye
uniform vec3 uEyeWorldPosition;

// Model-view-projection matrix
uniform mat4 uMvpMatrix;
// Tile model-view matrix
uniform mat4 uModelMatrix;

// Viewport dimensions in pixels
uniform vec2 uViewportSize;
// Minimum level of detail distance in world-space units
uniform float uMaxLodDistance;
// Tile position in world coordinates
uniform vec3 uTilePosition;
// Subdivision count to obtain the patch position
uniform int uSubdivisionCount;

// Per-patch frustrum culling switch
uniform bool uFrustrumCulling;
// Per-patch automatic LOD switch
uniform bool uManageLod;


const float k = uLevelOfDetail * uMaxLodDistance; // uMaxLodDistance: Ahora distancia a partir de la cual se activa la resolución máxima

float getTessLevel(float eyePatchDistance)
{
// BEGIN_Original
//	float nmax = floor(log2(uLevelOfDetail));
//	int n = int(clamp(-((nmax / uMaxLodDistance) * eyePatchDistance) + nmax + 1, 1, nmax));
//	return float(1 << n);
// END_Original

//  BEGIN_Prueba1
//	float nmax = log2(uLevelOfDetail);
//	int n = int(clamp(-((nmax / uMaxLodDistance) * eyePatchDistance) + nmax + 1, 1, nmax));
//	return float(1 << n);
//  END_Prueba1
//  BEGIN_Prueba2
//	int n = int(log2(clamp(uLevelOfDetail - (uLevelOfDetail * eyePatchDistance / uMaxLodDistance) + 1, 1, uLevelOfDetail)));
//	return float(1 << n);
//  END_Prueba2
//  BEGIN_Prueba3
//	int n = int(ceil(clamp(uLevelOfDetail - (uLevelOfDetail * eyePatchDistance / uMaxLodDistance), 1, uLevelOfDetail)));
//	return float(n);
//  END_Prueba3
//  BEGIN Prueba4
//	int n = int(clamp(k / eyePatchDistance, 1, uLevelOfDetail));
//	return float(n);
//	END Prueba4
//  BEGIN Definitiva
	int n = int(log2(clamp(k / eyePatchDistance, 1, uLevelOfDetail)));
	return float(1<<n);
//	END Definitiva
}

void main()
{
	// Per-patch culling: check if the projected vertices will be
	// inside the viewport, in order to discard them if they do not.
	bool inFrustrum = true;
	vec4 normControlPointPosition = (gl_in[gl_InvocationID].gl_Position / (uSubdivisionCount + 1) - vec4(0.5,0,0.5,0));
	normControlPointPosition.w = 1;
	vec4 patchScreenSpacePosition = uMvpMatrix * normControlPointPosition;

	if (
		(patchScreenSpacePosition.x > -1) &&
		(patchScreenSpacePosition.x <  1) &&
		(patchScreenSpacePosition.z > -1) &&
		(patchScreenSpacePosition.z <  1)
	)
	{
		inFrustrum = true;
	}

	if (inFrustrum || !uFrustrumCulling)
	{
		if (uManageLod)
		{
			vec4 patchWorldPosition = uModelMatrix * normControlPointPosition;
			float distanceToCamera = distance(uEyeWorldPosition, patchWorldPosition.xyz);
			float distanceToBorder = 0.5 / (uSubdivisionCount + 1);

			float tessLevelInner = getTessLevel(distanceToCamera);

			vec4 sidePosition;
			sidePosition.w = 1;

			sidePosition.xyz = normControlPointPosition.xyz + vec3(-distanceToBorder, 0, 0);
			patchWorldPosition = uModelMatrix * sidePosition;
			distanceToCamera = distance(uEyeWorldPosition, patchWorldPosition.xyz);
			float tessLevelOuter0 = getTessLevel(distanceToCamera);

			sidePosition.xyz = normControlPointPosition.xyz + vec3(0, 0, -distanceToBorder);
			patchWorldPosition = uModelMatrix * sidePosition;
			distanceToCamera = distance(uEyeWorldPosition, patchWorldPosition.xyz);
			float tessLevelOuter1 = getTessLevel(distanceToCamera);

			sidePosition.xyz = normControlPointPosition.xyz + vec3(distanceToBorder, 0, 0);
			patchWorldPosition = uModelMatrix * sidePosition;
			distanceToCamera = distance(uEyeWorldPosition, patchWorldPosition.xyz);
			float tessLevelOuter2 = getTessLevel(distanceToCamera);
			
			sidePosition.xyz = normControlPointPosition.xyz + vec3(0, 0, distanceToBorder);
			patchWorldPosition = uModelMatrix * sidePosition;
			distanceToCamera = distance(uEyeWorldPosition, patchWorldPosition.xyz);
			float tessLevelOuter3 = getTessLevel(distanceToCamera);

			gl_TessLevelOuter[0] = tessLevelOuter0;
			gl_TessLevelOuter[1] = tessLevelOuter1;
			gl_TessLevelOuter[2] = tessLevelOuter2;
			gl_TessLevelOuter[3] = tessLevelOuter3;

			gl_TessLevelInner[0] = tessLevelInner;
			gl_TessLevelInner[1] = tessLevelInner;

// BEGIN_MARIANO: Con mipmapping en el mapa de alturas
			tcsNivelLod[gl_InvocationID] = NumNiveles - log2(tessLevelInner);
// END_MARIANO: Con mipmapping
		}
		else
		{
			gl_TessLevelOuter[0] = uLevelOfDetail;
			gl_TessLevelOuter[1] = uLevelOfDetail;
			gl_TessLevelOuter[2] = uLevelOfDetail;
			gl_TessLevelOuter[3] = uLevelOfDetail;

			gl_TessLevelInner[0] = uLevelOfDetail;
			gl_TessLevelInner[1] = uLevelOfDetail;

// BEGIN_MARIANO: Con mipmapping en el mapa de alturas
			tcsNivelLod[gl_InvocationID] = NumNiveles - log2(uLevelOfDetail);
// END_MARIANO: Con mipmapping
		}
	}
	else
	{
		gl_TessLevelOuter[0] = 0;
		gl_TessLevelOuter[1] = 0;
		gl_TessLevelOuter[2] = 0;
		gl_TessLevelOuter[3] = 0;

		gl_TessLevelInner[0] = 0;
		gl_TessLevelInner[1] = 0;
	}

    tcUvCoordinates[gl_InvocationID] = vUvCoordinates[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
