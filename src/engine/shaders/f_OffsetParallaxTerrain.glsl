// Terrain Viewer
// Fragment shader, file: f_TextureTessellation.glsl
//
// - This shader program is used to output the fragment
//	 color when coupled with the tessellation pipeline.
//	 The uParallax option allows the program to use
//   parallax mapping.
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

// Bias for UV coordinates
#define SCALE uHeightScale

/* 
  Uncomment the parallax algorithm to be used:
   - MODO_PRUEBAS
   - NO_PARALLAX
   - SIMPLE_PARALLAX
   - OFFSET_LIMITING_PARALLAX
   - SLOPE_PARALLAX
   - ITERATIVE_PARALLAX
   - BINARY_PARALLAX
   - LINEAR_SEARCH_PARALLAX
   - SECANT_PARALLAX
*/
#define OFFSET_LIMITING_PARALLAX

// Iteration count for iterative parallax mapping approximation
#define ITERATIVE_PARALLAX_N_IT 4
// Iteration count for binary search parallax mapping approximation
#define BINARY_PARALLAX_N_IT 3

// UV coordinates of the incoming vertex
in vec2 teUvCoordinates;
in vec3 teNormal;
in vec3 teEyeVector;
in float hbase;
//in vec3 tePos;
// BEGIN_MARIANO: Con mipmapping en el mapa de alturas
in float tesNivelLod;
// END_MARIANO: Con mipmapping

in float incUV;

// Output fragment color
out vec4 fFragmentColor;

// Texture sampler of the color texture
uniform sampler2D uTextureSampler;
uniform sampler2D uHeightmap;

// Enable parallax mapping switch
uniform float uHeightScale;

void main()
{
#ifdef MODO_PRUEBAS
	vec3 v = normalize(teEyeVector);

	vec3 STH1 = vec3( teUvCoordinates, hbase * SCALE) + incUV * v.xzy;
	float d1 = -incUV * v.y;
	vec3 STH2 = vec3( teUvCoordinates, hbase * SCALE) - incUV * v.xzy;
	float d2 = incUV * v.y;

	vec3 STHm;
	float h;

	for (int i = 0; i < BINARY_PARALLAX_N_IT; i ++)
	{
		STHm = STH2 + (STH1 - STH2) * d2 / (d2 - d1);

		h = texture(uHeightmap, STHm.st).r * SCALE;

		if ( h < STHm.z )
		{
			d1 = h - STHm.z;
			STH1 = STHm;
		}
		else
		{
			d2 = h - STHm.z;
			STH2 = STHm;
		} 
	}
	STHm = STH2 + (STH1 - STH2) * d2 / (d2 - d1);
	fFragmentColor = texture(uTextureSampler, STHm.st);
#endif

#ifdef NO_PARALLAX
	float hsb = texture(uHeightmap, teUvCoordinates).r; // Para realizar el mismo número de accesos a texturas que en el caso de parallax
	fFragmentColor = texture(uTextureSampler, teUvCoordinates);
#endif

#ifdef OFFSET_LIMITING_PARALLAX 
	float hsb = (texture(uHeightmap, teUvCoordinates).r - hbase) * SCALE * normalize(teNormal).y; // OK
	// float hsb = (textureLod(uHeightmap, teUvCoordinates, tesNivelLod).r - hbase) * SCALE * normalize(teNormal).y; // No sale bien
	/* 
	vec4 hn = texture(uHeightmap, teUvCoordinates); // NO sale bien
	vec3 n = normalize(hn.bga * 2f - 1f);
	float hsb = (hn.r - hbase) * SCALE * n.y;
	*/
	fFragmentColor = texture(uTextureSampler, clamp(teUvCoordinates + hsb * normalize(teEyeVector).xz , 0, 1));
#endif


#ifdef SLOPE_PARALLAX
	vec3 n = normalize(teNormal);
	vec3 v = normalize(teEyeVector);
// BEGIN_MARIANO: Con mipmapping en el mapa de alturas
/*
	vec4 hn = textureLod(uHeightmap, teUvCoordinates, tesNivelLod);
	vec3 n = normalize(hn.bga * 2f - 1f);
	float hsb = (hn.r - hbase) * SCALE * n.y / max(dot(v,n), 0.5);
*/
//	float hsb = (textureLod(uHeightmap, teUvCoordinates, tesNivelLod).r - hbase) * SCALE * n.y / max(dot(v,n), 0.5);

// END_MARIANO: Con mipmapping
// BEGIN_MARIANO: Sin mipmapping en el mapa de alturas
/*
	vec4 hn = texture(uHeightmap, teUvCoordinates);
	vec3 n = normalize(teNormal);
	float hsb = (hn.r - hbase) * SCALE * n.y / max(dot(v,n), 0.5);
*/
	float hsb = (texture(uHeightmap, teUvCoordinates).r - hbase) * SCALE * n.y / max(dot(v,n), 0.5);
// END_MARIANO: Sin mipmapping
	fFragmentColor = texture(uTextureSampler, clamp(teUvCoordinates + hsb * v.xz , 0, 1));
#endif


#ifdef ITERATIVE_PARALLAX
	vec3 n = normalize(teNormal);
	vec3 v = normalize(teEyeVector);
	vec2 uv = teUvCoordinates;
	vec2 Kxz = v.xz * n.y * SCALE;
	for ( int i = ITERATIVE_PARALLAX_N_IT - 1; i >= 0; i -- )
	{
		uv += (texture(uHeightmap, uv).r - hbase) * Kxz;
	}
	fFragmentColor = texture(uTextureSampler, clamp(uv, 0, 1));
#endif


#ifdef BINARY_PARALLAX
	vec3 v = normalize(teEyeVector);

	//float h1 = hbase * SCALE + incUV * v.y;
	//vec2 ST1 = teUvCoordinates + incUV * v.xz;
	vec3 STH1 = vec3( teUvCoordinates, hbase * SCALE) + incUV * v.xzy;
	float d1 = -incUV * v.y;
	//float h2 = hbase * SCALE - incUV * v.y;
	//vec2 ST2 = teUvCoordinates - incUV * v.xz;
	vec3 STH2 = vec3( teUvCoordinates, hbase * SCALE) - incUV * v.xzy;
	float d2 = incUV * v.y;

	//vec2 STm;
	//float hm;
	vec3 STHm;
	float h;
	//vec4 hn;

	for (int i = 0; i < BINARY_PARALLAX_N_IT; i ++)
	{
		//STm = (ST1 + ST2) * 0.5;
		//hm = (h1 + h2) * 0.5;
		STHm = (STH1 + STH2) * 0.5;

		//h = texture(uHeightmap, STm).r * SCALE;
		h = texture(uHeightmap, STHm.st).r * SCALE;
		//hn = texture(uHeightmap, STm);

		//if (h < hm)
		if ( h < STHm.z )
		//if (hn.r * SCALE < hm)
		{
			//ST1 = STm;
			//d1 = h - hm;
			//h1 = hm;
			d1 = h - STHm.z;
			STH1 = STHm;
		}
		else
		{
			//ST2 = STm;
			//d2 = h - hm;
			//h2 = hm;
			d2 = h - STHm.z;
			STH2 = STHm;
		} 
	}
	// n = normalize(hn.gba * 2f - 1f);
	// STm += (hn.r * SCALE - hm) * v.xz * n.y; // / max(dot(v,n), 0.5);
	//STm = ST2 + (ST1 - ST2) * d2 / (d2 - d1);
	//fFragmentColor = texture(uTextureSampler, STm);
	STHm = STH2 + (STH1 - STH2) * d2 / (d2 - d1);
	fFragmentColor = texture(uTextureSampler, STHm.st);
#endif
}
