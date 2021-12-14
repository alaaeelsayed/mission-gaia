uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform mat4 worldIT;
uniform sampler2D u_heightMap;
uniform sampler2D u_choppyX;
uniform sampler2D u_choppyZ;
uniform bool u_isChoppy;
uniform float u_choppiness;


in vec4 a_position;
in vec3 a_normal;
in vec2 a_uv1;
in vec3 a_tangent;

out vec4 v_normal;
out vec3 v_pos;
out vec2 v_uv1;
out vec4 v_tangent;

void main()
{
    v_uv1 = a_uv1;
    vec4 pos = a_position;

    pos.y += texture(u_heightMap, v_uv1).g * 0.02f;
    if(u_isChoppy){
        pos.x -= texture(u_choppyX, v_uv1).g * 0.02f * u_choppiness;
        pos.z -= texture(u_choppyZ, v_uv1).g * 0.02f * u_choppiness;
    }
	v_pos = (world * pos).xyz;
    v_normal = worldIT * vec4(a_normal, 0.0);
    v_tangent = worldIT * vec4(a_tangent, 0.0);
    gl_Position = projection * view * world * pos;
}
