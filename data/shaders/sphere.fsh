uniform vec4 u_color;

in vec4 v_normal;
in vec2 v_uv1;
in vec3 v_pos;
out vec4 PixelColor;

void main()
{
	PixelColor = u_color;
	PixelColor.w = 0.3f;
}
