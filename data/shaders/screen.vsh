in vec2 a_position;
in vec2 a_uv1;

out vec2 v_uv1;

void main()
{
    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);;
    v_uv1 = a_uv1;
}
