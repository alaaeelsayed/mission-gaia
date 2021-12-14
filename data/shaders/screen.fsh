uniform sampler2D u_texture;

in vec2 v_uv1;

out vec4 PixelColor;

const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2( 0.0f,    offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0f),
        vec2( 0.0f,    0.0f),
        vec2( offset,  0.0f),
        vec2(-offset, -offset),
        vec2( 0.0f,   -offset),
        vec2( offset, -offset)
    );

    float kernel[9] = float[](
        1 / 16.0, 2 / 16.0, 1 / 16.0,
        2 / 16.0, 4 / 16.0, 2 / 16.0,
        1 / 16.0, 2 / 16.0, 1 / 16.0
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_texture, v_uv1.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    PixelColor = vec4(col, 1.0);
}