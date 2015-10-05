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
#define PARALLAX_IT 8

// UV coordinates of the incoming vertex
in vec4 teUvCoordinates;
in vec3 teNormal;
in vec2 teBoundingBox;
in mat4 teTbnMatrix;
in vec3 teEyeVector;

// Output fragment color
out vec4 fFragmentColor;

// Texture sampler of the color texture
uniform sampler2D uTextureSampler;
uniform sampler2D uHeightmap;

// Enable parallax mapping switch
uniform int uParallax;
uniform float uHeightScale;
uniform vec3 uEyeWorldPosition;

void main()
{
	vec3 normEyeVector = normalize(teEyeVector);
	int useParallax = uParallax;
	vec3 normal = normalize(teNormal);

	if (useParallax == 0)
	{
		fFragmentColor = texture(uTextureSampler, teUvCoordinates.st);
	}
	else
	{
		vec2 uvCoordOffset = teUvCoordinates.st;

#ifdef SIMPLE_PARALLAX
		float hsb = texture(uHeightmap, teUvCoordinates.st).r - teUvCoordinates.z;
		uvCoordOffset += hsb * SCALE * normEyeVector.xz / normEyeVector.y;
#endif

#ifdef OFFSET_LIMITING_PARALLAX
		float hsb = texture(uHeightmap, teUvCoordinates.st).r - teUvCoordinates.z;
		uvCoordOffset = teUvCoordinates.st + hsb * SCALE * normEyeVector.xz;
#endif

#ifdef SLOPE_PARALLAX
		/*float hsb = texture(uHeightmap, teUvCoordinates.st).r;
		uvCoordOffset = teUvCoordinates.st + (hsb - teUvCoordinates.z) * SCALE * normEyeVector.xz / normEyeVector.y;*/
		vec2 hn = texture(uHeightmap, uvCoordOffset.st).xy;
		float hsb = (hn.x - teUvCoordinates.z) * SCALE;
		uvCoordOffset = teUvCoordinates.st + hsb * hn.y * normEyeVector.xz;
#endif

#ifdef ITERATIVE_PARALLAX
		float rayHeight = 1;//teUvCoordinates.w * SCALE;
		vec2 hn = texture(uHeightmap, uvCoordOffset).rg;
		vec3 uvh = (hn.r - teUvCoordinates.z - rayHeight) * SCALE * hn.g * normEyeVector;

		for (int i = 0; i < PARALLAX_IT; i++)
		{
			hn = texture(uHeightmap, uvCoordOffset).rg;
			float h = hn.r * SCALE;

			uvh.st += (hn.r - teUvCoordinates.z - rayHeight) * SCALE * hn.g * normEyeVector.xz;
			uvCoordOffset += uvh.st;
			//rayHeight -= uvh.z;
		}
#endif

#ifdef BINARY_PARALLAX
		float hsb1 = /*teBoundingBox.s*/ - teUvCoordinates.z;
		vec2 ST1 = teUvCoordinates.st + hsb1 * SCALE * normEyeVector.xz / normEyeVector.y;

		float hsb2 = /*teBoundingBox.t*/1 - teUvCoordinates.z;
		vec2 ST2 = teUvCoordinates.st + hsb2 * SCALE * normEyeVector.xz / normEyeVector.y;

		vec2 STm;
		float hsbm, hsb;

		for (int i = 1; i <= PARALLAX_IT; i ++)
		{
			STm = (ST1 + ST2) / 2.0;
			hsbm = (hsb1 + hsb2) / 2.0;

			hsb = (texture(uHeightmap, STm).r - teUvCoordinates.z) * SCALE;

			if (hsb < hsbm)
			{
				ST2 = STm;
				hsb2 = hsbm;
			}
			else
			{
				ST1 = STm;
				hsb1 = hsbm;
			} 
		}

		uvCoordOffset = STm;
#endif

#ifdef LINEAR_SEARCH_PARALLAX
		float hsb1 = /*teBoundingBox.s*/- teUvCoordinates.z;
		float hsb2 = 0;///*teBoundingBox.t*/1.0 - teUvCoordinates.z;
		float delta = (hsb1 - hsb2) / PARALLAX_IT;

		vec2 ST1 = teUvCoordinates.st + hsb1 * SCALE * normEyeVector.xz;
		vec2 ST2 = teUvCoordinates.st + hsb2 * SCALE * normEyeVector.xz;
		vec2 STint;

		for (int i = 0; i < PARALLAX_IT; i++)
		{
			hsb1 -= delta;
			STint = ST1 * hsb1 + ST2 * (1 - hsb1);

			float hsbNext = (texture(uHeightmap, STint).r - teUvCoordinates.z) * SCALE;

			if (hsb2 == 0)
			{
				if (hsb1 <= hsbNext)
					hsb2 = hsbNext;
			}
		}

		uvCoordOffset = ST1 * hsb2 + ST2 * (1 - hsb2);
#endif

#ifdef SECANT_PARALLAX
		
		float Ha = /*teBoundingBox.s;*/  - teUvCoordinates.z;
		float Hb = /*teBoundingBox.t;*/  1 - teUvCoordinates.z;
		float Dh = (Ha - Hb) / PARALLAX_IT;
		float Da, Db;

		vec2 STa = /*clamp(teUvCoordinates.st + 0.1f * normEyeVector.xz, 0, 1);//*/teUvCoordinates.st + Ha * SCALE * normEyeVector.xz / normEyeVector.y;
		vec2 STb =/* clamp(teUvCoordinates.st - 0.1f * normEyeVector.xz, 0, 1);//*/teUvCoordinates.st + Hb * SCALE * normEyeVector.xz / normEyeVector.y;

		vec2 Ds = (STa - STb) / PARALLAX_IT;

		Ha = ((STa - teUvCoordinates.st) / SCALE) * (normEyeVector.y / normEyeVector.xz); 
		Hb = ((STb - teUvCoordinates.st) / SCALE) * (normEyeVector.y / normEyeVector.xz);
		Dh = (Ha - Hb) / PARALLAX_IT;

		Ha = Hb;
		STa = STb;

		for (int i = 0; i <= PARALLAX_IT; i++)
		{
			STa += Ds;
			Ha += Dh;

			Da = Ha - (texture(uHeightmap, STa) - teUvCoordinates.z);

			if (Da < 0)
				break;

			Db = Da;
			STb = STa;
		}

		if (Da == Db)
			discard;	
		else
			uvCoordOffset = STb * (Da / (Da - Db)) + STa * (Db / (Db - Da));

#endif
	
		// Discard fragments where the offset sets the UV coordinates
		// out of bounds
		if (uvCoordOffset.s > 1 || uvCoordOffset.t > 1 ||
			uvCoordOffset.s < 0 || uvCoordOffset.t < 0) discard;

		fFragmentColor = texture(uTextureSampler, clamp(uvCoordOffset, 0, 1));
	}
}
