#version 430 core

in vec4 aVertices;
in vec2 aUvCoordinates;

out vec2 vUvCoordinates;

void main()
{
    gl_Position = aVertices;
    vUvCoordinates = aUvCoordinates;
}
