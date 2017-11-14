#version 430 core

in vec3 aVertices;
in vec3 aColor;

out vec3 vColor;

uniform mat4 uMvpMatrix;

void main()
{
    gl_Position = uMvpMatrix * vec4(aVertices, 1);
    vColor = aColor;
}
