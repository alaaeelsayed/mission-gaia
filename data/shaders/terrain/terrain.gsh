layout(points) in;
layout(line_strip, max_vertices = 5) out;

void main()
{
    // Assuming tessellated points are in a grid, let's determine
    // our current position in the grid based on the vertex ID
	int gridSize = int(sqrt(float(gl_PrimitiveIDIn + 1)));
	int currentRow = gl_PrimitiveIDIn / gridSize;
	int currentColumn = gl_PrimitiveIDIn % gridSize;

    // Emit the current vertex
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

    // If not the last column, emit right neighbor
	if (currentColumn < gridSize - 1)
	{
		gl_Position = gl_in[0].gl_Position + vec4(1.0 / gridSize, 0.0, 0.0, 0.0);
		EmitVertex();
	}

    // End the current line
	EndPrimitive();

    // If not the last row, emit bottom neighbor
	if (currentRow < gridSize - 1)
	{
		gl_Position = gl_in[0].gl_Position + vec4(0.0, 1.0 / gridSize, 0.0, 0.0);
		EmitVertex();
	}

    // End the primitive
	EndPrimitive();
}
