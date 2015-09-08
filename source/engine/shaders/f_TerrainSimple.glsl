#version 330 core

in vec2 vUvCoordinates;
in vec3 vColor;

uniform sampler2D uTextureSampler;
out vec4 fFragmentColor;

void main()
{
    fFragmentColor = texture2D(uTextureSampler, vUvCoordinates);
}
