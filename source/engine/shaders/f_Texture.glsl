#version 430 core

in vec2 vUvCoordinates;
out vec4 fFragmentColor;

uniform sampler2D uTextureSampler;

void main()
{
    fFragmentColor = texture2D(uTextureSampler, vUvCoordinates);
}
