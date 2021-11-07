// Spot Light
uniform vec4 u_lightPosRange;
uniform vec3 u_lightColor;
uniform vec3 u_lightAttenuation;
uniform vec4 u_lightSpot;

uniform sampler2D u_diffuseTex;

// Scene properties
uniform vec3 u_lightDir;
uniform vec3 u_viewPos;
uniform vec3 u_ambientLight;

in vec4 v_normal;
in vec2 v_uv1;
in vec3 v_pos;
out vec4 PixelColor;

// spotlight: xyz = spot direction, w = spot "angle"
vec3 calc_lighting(vec4 lightPosRange, vec3 lightColor, vec3 lightAttenuation, vec4 spotlight, 
				   vec3 samplePos, vec3 n, vec3 viewDir, vec3 diffuseColor)
{
	vec3 lightDir = lightPosRange.xyz - samplePos.xyz;
	float distance = length(lightDir);

	if( distance > lightPosRange.w )
	{
		return vec3(0,0,0);	
	}		
	else
	{
		// normalize it
		lightDir /= distance;
		
		// Calculate spot light falloff
		float spot = pow(max(dot(-lightDir,spotlight.xyz), 0.0), spotlight.w);
		// Attenuate the light
		float att = spot / dot(lightAttenuation, vec3(1.0, distance, distance*distance));

		return vec3(1,1,1) * att;
	}
}

void main()
{
    vec3 diffuseMat = texture(u_diffuseTex, v_uv1).rgb;  
	vec3 diffuseLight = u_lightColor * max(0, dot(v_normal.xyz, u_lightDir));

    vec3 n = normalize(v_normal.xyz);
	
    vec3 color = vec3(0,0,0);
	vec3 viewDir = normalize(u_viewPos - v_pos);
	
	color += calc_lighting(u_lightPosRange, u_lightColor, u_lightAttenuation, u_lightSpot, v_pos, n, viewDir, diffuseMat);
	color += (u_ambientLight + diffuseLight) * diffuseMat;

	PixelColor = vec4(color, 1.0);
}

