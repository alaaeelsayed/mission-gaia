layout (quads, equal_spacing, ccw) in;

in vec2 uvCoords[];
in vec3 v_positions[];
in vec4 v_normals[];
in vec4 v_tangents[];
in vec4 v_bitangents[];

out vec2 v_uv1;
out vec3 v_pos;
out precise vec4 v_normal;
out vec4 v_tangent;
out vec4 v_bitangent;

struct TerrainRegion
{
    float min;
    float max;
	sampler2D colorMap;
    sampler2D normalMap;
    sampler2D dispMap;
	float dispMult;
    float tileFactor;
};

uniform TerrainRegion terrainRegions[4];

uniform mat4 projection;
uniform mat4 view;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	
	vec2 uv0 = uvCoords[0];
	vec2 uv1 = uvCoords[1];
	vec2 uv2 = uvCoords[2];
	vec2 uv3 = uvCoords[3];
	
	vec2 topUV = uv0 + v * (uv3 - uv0);
	vec2 bottomUV = uv1 + v * (uv2 - uv1);
	precise vec2 texCoord = bottomUV + u * (topUV - bottomUV);

	vec3 v_pos0 = v_positions[0];
    vec3 v_pos1 = v_positions[1];
    vec3 v_pos2 = v_positions[2];
    vec3 v_pos3 = v_positions[3];

    vec3 topVPos = v_pos0 + v * (v_pos3 - v_pos0);
    vec3 bottomVPos = v_pos1 + v * (v_pos2 - v_pos1);
    v_pos = bottomVPos + u * (topVPos - bottomVPos);

	vec4 normal0 = v_normals[0];
	vec4 normal1 = v_normals[1];
	vec4 normal2 = v_normals[2];
	vec4 normal3 = v_normals[3];

	vec4 topNormal = normal0 + v * (normal3 - normal0);
	vec4 bottomNormal = normal1 + v * (normal2 - normal1);
	v_normal = bottomNormal + u * (topNormal - bottomNormal);

	precise float displacementValue = 0.0f;
    
	for (int i = 0; i < 4; i++)
    {
        if (v_pos.y >= terrainRegions[i].min && v_pos.y <= terrainRegions[i].max)
        {
			v_uv1 = texCoord * terrainRegions[i].tileFactor;
            displacementValue = texture(terrainRegions[i].dispMap, v_uv1).r * terrainRegions[i].dispMult;
            break;
        }
    }

    v_pos += displacementValue * v_normal.xyz;

    gl_Position = projection * view * vec4(v_pos, 1.0f);

	vec4 topTangent = v_tangents[0] + v * (v_tangents[3] - v_tangents[0]);
	vec4 bottomTangent = v_tangents[1] + v * (v_tangents[2] - v_tangents[1]);
	v_tangent = bottomTangent + u * (topTangent - bottomTangent);

	vec4 topBitangent = v_bitangents[0] + v * (v_bitangents[3] - v_bitangents[0]);
	vec4 bottomBitangent = v_bitangents[1] + v * (v_bitangents[2] - v_bitangents[1]);
	v_bitangent = bottomBitangent + u * (topBitangent - bottomBitangent);
}