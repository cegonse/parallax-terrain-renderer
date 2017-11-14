#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 teUvCoordinates[];
out vec2 gUvCoordinates;

// Triangle count is obtained through an atomic counter
layout (binding = 0, offset = 0) uniform atomic_uint atTriangles;

void main()
{
	// Pass through triangles
	for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
		gUvCoordinates = teUvCoordinates[i];
        EmitVertex();
    }

	// Increment atomic counter
	atomicCounterIncrement(atTriangles);

    EndPrimitive();
}