uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform vec3 u_ambientLight;
uniform sampler2D u_diffuseTex;

in vec4 v_normal;
in vec3 v_pos;
in vec2 v_uv1;
out vec4 PixelColor;

void main()
{
    vec3 n = normalize(v_normal.xyz);
    vec3 diffuseLight = vec3(0.0002f, 0.0002f, 0.0002f) * max(0, dot(v_normal.xyz, u_lightDir));
    vec3 diffuseMat = texture(u_diffuseTex, v_uv1).rgb;
    PixelColor.rgb = diffuseMat * (u_ambientLight + diffuseLight);
    PixelColor.a = 1.0;
}