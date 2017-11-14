// Terrain Viewer
// Vertex shader, file: v_TerrainTess.glsl
//
// Copyright (c) César González Segura 2015-2016, ETSE-UV
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

in vec4 aVertices;
in vec2 aUvCoordinates;

out vec2 vUvCoordinates;

uniform mat4 uMvpMatrix;

void main()
{
    vUvCoordinates = aUvCoordinates;
	gl_Position = aVertices;
}
