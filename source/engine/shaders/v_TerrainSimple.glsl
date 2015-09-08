#version 330 core

in vec4 aVertices;
in vec2 aUvCoordinates;

out vec2 vUvCoordinates;
out vec3 vColor;

uniform mat4 uMvpMatrix;

void main()
{
    vUvCoordinates = aUvCoordinates;

    gl_Position.x = aVertices.x;
    gl_Position.z = aVertices.z;
    gl_Position.y = aVertices.y;
    gl_Position.w = aVertices.w;

    vColor = vec3(1 - gl_Position.y, 1 - gl_Position.y, 1 - gl_Position.y);
    gl_Position = uMvpMatrix * gl_Position;
}
