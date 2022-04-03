in vec2 v_pos;
in vec4 v_color;
out vec4 PixelColor;
uniform sampler2D u_texture;

void main()
{
    PixelColor = v_color;
}
