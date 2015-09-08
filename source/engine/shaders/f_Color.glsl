#version 430 core

in vec3 vColor;
out vec4 fFragmentColor;

void main()
{
    fFragmentColor = vec4(vColor.x, vColor.y, vColor.z, 1);
}
