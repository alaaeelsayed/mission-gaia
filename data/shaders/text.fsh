in vec2 v_uv1;

uniform bool u_outline;
uniform vec4 u_color;
uniform sampler2D u_tex;

out vec4 PixelColor;

const float width = 0.41;
const float edge = 0.1;

void main()
{
    float distance = 1.0 - texture(u_tex, v_uv1).a;
    float alpha = 1.0 - smoothstep(width, width + edge, distance);

    if(u_outline)
    {
        PixelColor = u_color;
    }
    else
    {
        PixelColor = vec4(u_color.rgb, alpha);
    }
}
