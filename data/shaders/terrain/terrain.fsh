in vec4 v_normal;
in vec2 v_uv1;
in vec3 v_pos;
in vec4 v_tangent;
in vec4 v_bitangent;

out vec4 PixelColor;

struct TerrainRegion {
    float min;
    float max;
    sampler2D colorMap;
    sampler2D normalMap;
    sampler2D dispMap;
	float dispMult;
    float tileFactor;
};

uniform sampler2D u_texture;
uniform TerrainRegion terrainRegions[4];
uniform vec3 u_lightPos;
uniform vec3 u_ambientLight;
uniform float u_spotLightFactor;
uniform vec3 u_spotLightPos;
uniform vec3 u_spotLightColor;
uniform vec3 u_spotLightAttenuation;
uniform vec3 u_spotLightDir;
uniform float u_spotLightConeAngle;
uniform float u_spotLightCutOff;

vec3 calc_spot_lighting(vec3 lightPos, vec3 lightColor, vec3 lightAttenuation, vec3 spotDirection, 
                        float coneAngle, float cutOffAngle, vec3 vertPos)
{
    vec3 vertToLight = lightPos - vertPos;
    float distance = length(vertToLight);
    vertToLight = normalize(vertToLight);
    
    // returns the cosine of the angle between ray from the vertex to the light and the spotlight center ray
    float theta = dot(vertToLight, normalize(-spotDirection));
    
    // Compute the intensity based on the theta, coneAngle (inner cutoff) and cutOffAngle (outer cutoff)
    float epsilon = coneAngle - cutOffAngle;
    float intensity = clamp((theta - cutOffAngle) / epsilon, 0.0, 1.0);
    
    // Calculate spot light falloff
    float spot = pow(max(dot(-vertToLight, spotDirection), 0.0), coneAngle);
    
    // Attenuate the light
    float att = spot * intensity / dot(lightAttenuation, vec3(1.0, distance, distance*distance));
    
    return lightColor * att;
}

float softstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

float compute_region_weight(float height, float regionMin, float regionMax) {
    float blendZone = 10.0;
    float lowerWeight = softstep(regionMin - blendZone, regionMin, height);
    float upperWeight = 1.0 - softstep(regionMax, regionMax + blendZone, height);
    return min(lowerWeight, upperWeight);
}

vec3 calc_blend_weights(vec3 normal) {
    vec3 weights = abs(normal);
    weights = normalize(max(weights, 0.0001));
    return weights / (weights.x + weights.y + weights.z);
}

vec4 triplanar_sample_texture(sampler2D textureSampler, vec3 worldPos, vec3 blendWeights, float tileFactor) {
    vec2 uvX = worldPos.zy * tileFactor;
    vec2 uvY = worldPos.xz * tileFactor;
    vec2 uvZ = worldPos.xy * tileFactor;

    vec4 texColorX = texture(textureSampler, uvX) * blendWeights.x;
    vec4 texColorY = texture(textureSampler, uvY) * blendWeights.y;
    vec4 texColorZ = texture(textureSampler, uvZ) * blendWeights.z;

    return texColorX + texColorY + texColorZ;
}

vec3 triplanar_sample_normal(sampler2D normalSampler, vec3 worldPos, vec3 blendWeights, float tileFactor, vec3 worldNormal) {
    vec2 uvX = worldPos.zy * tileFactor;
    vec2 uvY = worldPos.xz * tileFactor;
    vec2 uvZ = worldPos.xy * tileFactor;

    vec3 normalX = texture(normalSampler, uvX).xyz * 2.0 - 1.0;
    vec3 normalY = texture(normalSampler, uvY).xyz * 2.0 - 1.0;
    vec3 normalZ = texture(normalSampler, uvZ).xyz * 2.0 - 1.0;

    normalX = vec3(normalX.xy * sign(worldNormal.x), abs(normalX.z) * abs(worldNormal.x));
    normalY = vec3(normalY.xy * sign(worldNormal.y), abs(normalY.z) * abs(worldNormal.y));
    normalZ = vec3(normalZ.xy * sign(worldNormal.z), abs(normalZ.z) * abs(worldNormal.z));

    vec3 blendedNormal = normalize(
        normalX.zyx * blendWeights.x +
        normalY.xzy * blendWeights.y +
        normalZ.xyz * blendWeights.z
    );

    mat3 TBN = mat3(v_tangent.xyz, v_bitangent.xyz, worldNormal);
    return normalize(TBN * blendedNormal);
}

void main() {
    vec3 worldNormal = normalize(v_normal.xyz);
    vec3 blendWeights = calc_blend_weights(worldNormal);
    vec3 worldPos = v_pos;

    vec4 terrainColor = vec4(0.0);
    vec3 terrainNormal = vec3(0.0);
    float totalRegionWeight = 0.0;

    for (int i = 0; i < 4; i++) {
        float regionWeight = compute_region_weight(worldPos.y, terrainRegions[i].min, terrainRegions[i].max);
        totalRegionWeight += regionWeight;
        terrainColor += regionWeight * triplanar_sample_texture(terrainRegions[i].colorMap, worldPos, blendWeights, terrainRegions[i].tileFactor);
        terrainNormal += regionWeight * triplanar_sample_normal(terrainRegions[i].normalMap, worldPos, blendWeights, terrainRegions[i].tileFactor, worldNormal);
    }

    if (totalRegionWeight > 0.0) {
        terrainColor /= totalRegionWeight;
        terrainNormal = normalize(terrainNormal);
    }

    vec3 lightDir = normalize(u_lightPos - worldPos);
    vec3 diffuseLight = vec3(0.4, 0.5, 0.45) * max(dot(terrainNormal, lightDir), 0.0);
    vec3 finalColor = (u_ambientLight + diffuseLight) * terrainColor.rgb;

    if (u_spotLightFactor > 0.0) {
        finalColor += u_spotLightFactor * calc_spot_lighting(u_spotLightPos, u_spotLightColor, u_spotLightAttenuation, u_spotLightDir, u_spotLightConeAngle, u_spotLightCutOff, worldPos);
    }

    PixelColor = vec4(finalColor, 1.0);
}
