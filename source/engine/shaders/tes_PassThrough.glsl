#version 430 core

layout (triangles, equal_spacing, ccw) in;

in vec2 tcUvCoordinates[];
out vec2 teUvCoordinates;

uniform mat4 uMvpMatrix;

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
   	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

void main()
{
    gl_Position.xyz = interpolate3D(
        gl_in[0].gl_Position.xyz,
        gl_in[1].gl_Position.xyz,
        gl_in[2].gl_Position.xyz);

    gl_Position.w = 1.0;

    gl_Position = uMvpMatrix * gl_Position;

    teUvCoordinates = interpolate2D(tcUvCoordinates[0],
            tcUvCoordinates[1],
            tcUvCoordinates[2]);
}
