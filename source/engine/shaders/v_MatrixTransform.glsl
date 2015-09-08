#version 430 core

in vec3 aVertices;
in vec2 aUvCoordinates;

out vec2 vUvCoordinates;
uniform mat4 uMvpMatrix;

void main()
{
    gl_Position = uMvpMatrix * vec4(aVertices, 1);
    vUvCoordinates = aUvCoordinates;
}
