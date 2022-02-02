uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform vec3 u_ambientLight;
uniform sampler2D u_heightMap;
uniform vec3 u_waterColor;
uniform bool u_invertColors;

in vec4 v_normal;
in vec3 v_pos;
in vec2 v_uv1;
out vec4 PixelColor;

void main()
{
    float height = texture(u_heightMap, v_uv1).g * 0.02f;
    vec3 n = normalize(v_normal.xyz);
    vec3 diffuseLight = vec3(0.45f, 0.54f, 0.4f) * max(0, dot(v_normal.xyz, u_lightDir));
    if(u_invertColors)
        PixelColor.rgb = (u_waterColor.rgb + height*20) * (diffuseLight + u_ambientLight);
    else
        PixelColor.rgb = (u_waterColor.rgb - height*20) * (diffuseLight + u_ambientLight);
    PixelColor.a = 1.0f;
}
