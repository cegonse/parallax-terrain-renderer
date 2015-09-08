#version 430 core

layout (quads, equal_spacing, ccw) in;

in vec2 tcUvCoordinates[];
out vec2 teUvCoordinates;
out vec3 teColor;

uniform mat4 uMvpMatrix;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float omu = 1 - u;
    float omv = 1 - v;

    gl_Position.x = u * (gl_in[0].gl_Position.x*2 ) +
                    omu * (gl_in[1].gl_Position.x*2) +
                    omu * (gl_in[2].gl_Position.x*2) +
                    u * (gl_in[3].gl_Position.x*2);

    gl_Position.y = v * (gl_in[0].gl_Position.y*2) +
                    omv * (gl_in[1].gl_Position.y*2) +
                    omv * (gl_in[2].gl_Position.y*2) +
                    v * (gl_in[3].gl_Position.y*2);

    gl_Position.z = 0;
    gl_Position.w = 1;
    gl_Position = uMvpMatrix * gl_Position;

    teUvCoordinates.x = u * tcUvCoordinates[0].x +
            omu * tcUvCoordinates[1].x +
            omu * tcUvCoordinates[2].x +
            u * tcUvCoordinates[3].x;

    teUvCoordinates.y = v * tcUvCoordinates[0].y +
            omv * tcUvCoordinates[1].y +
            omv * tcUvCoordinates[2].y +
            v * tcUvCoordinates[3].y;

    teColor = gl_TessCoord;
}

