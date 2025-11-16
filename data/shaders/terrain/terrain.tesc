layout (vertices = 4) out;

in vec2 v_uv1[];
in vec3 v_pos[];
in vec4 v_normal[];
in vec4 v_tangent[];
in vec4 v_bitangent[];

out vec2 uvCoords[];
out vec3 v_positions[];
out vec4 v_normals[];
out vec4 v_tangents[];
out vec4 v_bitangents[];

layout (location = 0) uniform vec3 u_characterPosition;

const int MIN_TES = 2;
const int MAX_TES = gl_MaxTessGenLevel;
const float MAX_DIST = 300;

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	uvCoords[gl_InvocationID] = v_uv1[gl_InvocationID];
	v_positions[gl_InvocationID] = v_pos[gl_InvocationID];
    v_normals[gl_InvocationID] = v_normal[gl_InvocationID];
    v_tangents[gl_InvocationID] = v_tangent[gl_InvocationID];
    v_bitangents[gl_InvocationID] = v_bitangent[gl_InvocationID];


	if (gl_InvocationID == 0)
    {
        //
        vec3 center0 = v_pos[2] + (v_pos[1] - v_pos[2]) / 2.0;
        vec3 center1 = v_pos[1] + (v_pos[0] - v_pos[1]) / 2.0;
        vec3 center2 = v_pos[0] + (v_pos[3] - v_pos[0]) / 2.0;
        vec3 center3 = v_pos[3] + (v_pos[2] - v_pos[3]) / 2.0;
        
        float dist0 = length(u_characterPosition - center0);
        float dist1 = length(u_characterPosition - center1);
        float dist2 = length(u_characterPosition - center2);
        float dist3 = length(u_characterPosition - center3);

        int tes0 = int(mix(MAX_TES, MIN_TES, clamp(dist0 / MAX_DIST, 0.0, 1.0)));
        int tes1 = int(mix(MAX_TES, MIN_TES, clamp(dist1 / MAX_DIST, 0.0, 1.0)));
        int tes2 = int(mix(MAX_TES, MIN_TES, clamp(dist2 / MAX_DIST, 0.0, 1.0)));
        int tes3 = int(mix(MAX_TES, MIN_TES, clamp(dist3 / MAX_DIST, 0.0, 1.0)));

        gl_TessLevelOuter[0] = tes0;
        gl_TessLevelOuter[1] = tes1;
        gl_TessLevelOuter[2] = tes2;
        gl_TessLevelOuter[3] = tes3;
        
        gl_TessLevelInner[0] = max(tes1, tes3);
        gl_TessLevelInner[1] = max(tes0, tes2);
    }
}