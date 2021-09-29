uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform vec3 u_ambientLight;
uniform vec3 u_viewPos;
uniform vec3 u_specularColor;
uniform float u_shininess;
uniform sampler2D u_diffuseTex;

in vec4 v_normal;
in vec3 v_pos;
in vec2 v_uv1;
out vec4 PixelColor;

void main()
{
    vec3 n = normalize(v_normal.xyz);

    vec3 diffuseLight = u_lightColor * max(0, dot(v_normal.xyz, u_lightDir));

    vec3 viewDir = normalize(u_viewPos - v_pos);
    vec3 spec = u_lightColor * pow(max(0.0,dot(reflect(-u_lightDir,n),viewDir)),u_shininess);

    vec3 diffuseMat = texture(u_diffuseTex, v_uv1).rgb;

    PixelColor.rgb = diffuseMat * (u_ambientLight + diffuseLight) +
                     u_specularColor * spec;
    PixelColor.a = 1.0;
}