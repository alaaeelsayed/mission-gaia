uniform sampler2D u_texture;

in vec4 v_normal;
in vec2 v_uv1;
in vec3 v_pos;

out vec4 PixelColor;

void main()
{
	PixelColor = texture(u_texture, v_uv1 * 40.0);
}

