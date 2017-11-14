#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 teUvCoordinates[];
in vec3 teNormal[];
in vec3 teEyeVector[];
in mat3 teTbnMatrix[];
in vec2 teVertEye[];

out vec2 gUvCoordinates;
out vec3 gNormal;
out vec3 gEyeVector;
out mat3 gTbnMatrix;
out vec2 gVertEye;

// Triangle count is obtained through an atomic counter
layout (binding = 0, offset = 0) uniform atomic_uint atTriangles;

void main()
{
	// Pass through triangles
	for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
		gUvCoordinates = teUvCoordinates[i];
		gNormal = teNormal[i];
		gEyeVector = teEyeVector[i];
		gTbnMatrix = teTbnMatrix[i];
		gVertEye = teVertEye[i];

        EmitVertex();
    }

	// Increment atomic counter
	atomicCounterIncrement(atTriangles);

    EndPrimitive();
}