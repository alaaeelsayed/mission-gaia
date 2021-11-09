in vec2 v_uv1;

uniform bool u_outline;
uniform vec4 u_color;
uniform sampler2D u_tex;

out vec4 PixelColor;

const float width = 0.41;
const float edge = 0.1;

const float borderWidth = 0.5;
const float borderEdge = 0.4;

const vec2 offset = vec2(0.006, 0.006);

const vec3 outlineColor = vec3(0.2, 0.2, 0.2);

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
