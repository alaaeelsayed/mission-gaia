uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform mat4 worldIT;

in vec4 a_position;
in vec2 a_uv1;
in vec3 a_normal;
in vec3 a_tangent;
in vec3 a_bitangent;

out vec4 v_normal;
out vec3 v_pos;
out vec2 v_uv1;
out vec4 v_tangent;
out vec4 v_bitangent;

void main()
{
    // Calculate the initial position in world space
    vec4 worldPosition = world * a_position;
    
    // Calculate the final position in screen space
    gl_Position = projection * view * worldPosition;

    // Set other out values
    v_uv1 = a_uv1;
    v_pos = worldPosition.xyz;
    v_normal = worldIT * vec4(a_normal, 0.0);
    v_tangent = vec4(a_tangent, 0.0);
    v_bitangent = vec4(a_bitangent, 0.0);
}
