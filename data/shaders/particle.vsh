uniform mat4 projection;
uniform mat4 view;
in vec4 a_position;
in vec4 a_color;
in float a_normal;
out vec4 v_color;

void main()
{
    gl_Position = projection * view * a_position;
    gl_PointSize = a_normal * 5.0f; // Scaling factor
	v_color = a_color;
}
