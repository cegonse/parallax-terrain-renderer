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

// UV coordinates of the incoming vertex
in vec2 teUvCoordinates;

// Output fragment color
out vec4 fFragmentColor;

// Texture sampler of the color texture
uniform sampler2D uTextureSampler;
uniform sampler2D uHeightmap;

void main()
{
	float hsb = texture(uHeightmap, teUvCoordinates).r; // Para realizar el mismo número de accesos a texturas que en el caso de parallax
	fFragmentColor = texture(uTextureSampler, teUvCoordinates);
}
