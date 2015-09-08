#version 330 core

in vec4 aVertices;
in vec2 aUvCoordinates;
in vec2 aBoundingBox;

out vec2 vUvCoordinates;
out vec2 vBoundingBox;

uniform mat4 uMvpMatrix;

void main()
{
    vUvCoordinates = aUvCoordinates;
	vBoundingBox = aBoundingBox;
	gl_Position = aVertices;
}
