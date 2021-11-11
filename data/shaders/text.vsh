uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

uniform vec4 u_translation;

in vec4 a_position;
in vec2 a_uv1;
out vec2 v_uv1;

void main()
{
    gl_Position = projection * view * world * (a_position + u_translation);
	v_uv1 = a_uv1;
}
