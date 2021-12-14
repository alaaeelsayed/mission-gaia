uniform sampler2D u_texture;

in vec4 v_normal;
in vec2 v_uv1;
in vec3 v_pos;

struct TerrainRegion
{
    float min;
    float max;
};

uniform TerrainRegion region1;
uniform TerrainRegion region2;
uniform TerrainRegion region3;
uniform TerrainRegion region4;

uniform sampler2D region1ColorMap;
uniform sampler2D region2ColorMap;
uniform sampler2D region3ColorMap;
uniform sampler2D region4ColorMap;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_ambientLight;

out vec4 PixelColor;

vec4 GetTerrainColor()
{
	vec4 terrainColor = vec4(0.0, 0.0, 0.0, 1.0);
    float height = v_pos.y;
    float regionMin = 0.0;
    float regionMax = 0.0;
    float regionRange = 0.0;
    float regionWeight = 0.0;
    
    regionMin = region1.min;
    regionMax = region1.max;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region1ColorMap, v_uv1.st);

    regionMin = region2.min;
    regionMax = region2.max;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region2ColorMap, v_uv1.st);

    regionMin = region3.min;
    regionMax = region3.max;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region3ColorMap, v_uv1.st);

    regionMin = region4.min;
    regionMax = region4.max;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region4ColorMap, v_uv1.st);

	return terrainColor;
}

void main()
{
    vec3 n = normalize(v_normal.xyz);

	vec3 lightDir = normalize(u_lightPos - v_pos);

	// 106, 121, 103
	vec3 diffuseLight = vec3(0.45f, 0.54f, 0.4f) * max(0, dot(n, lightDir));
	
	PixelColor = vec4((u_ambientLight + diffuseLight) * vec3(GetTerrainColor()), 1.0f);
}