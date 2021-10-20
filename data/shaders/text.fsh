in vec2 v_uv1;

uniform bool u_outline;
uniform vec4 u_color;
uniform sampler2D u_tex;

out vec4 PixelColor;

void main()
{
    if(u_outline)
    {
        PixelColor = u_color;
    }
    else
    {
        PixelColor = vec4(u_color.rgb, texture(u_tex, v_uv1).a * u_color.a);
    }
}
