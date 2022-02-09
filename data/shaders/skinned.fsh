// Spot Light
uniform vec4 u_lightPosRange;
uniform vec3 u_lightColor;
uniform vec3 u_lightAttenuation;
uniform vec4 u_lightSpot;

// Point Light 1
uniform vec4 u_lightPosRange1;
uniform vec3 u_lightColor1;
uniform vec3 u_lightAttenuation1;

// Point Light 2
uniform vec4 u_lightPosRange2;
uniform vec3 u_lightColor2;
uniform vec3 u_lightAttenuation2;

// Point Light 3
uniform vec4 u_lightPosRange3;
uniform vec3 u_lightColor3;
uniform vec3 u_lightAttenuation3;

// Point Light 4
uniform vec4 u_lightPosRange4;
uniform vec3 u_lightColor4;
uniform vec3 u_lightAttenuation4;

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
vec3 calc_spot_lighting(vec4 lightPosRange, vec3 lightColor, vec3 lightAttenuation, vec4 spotlight, 
				   vec3 samplePos, vec3 n, vec3 viewDir)
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

// pointlight
vec3 calc_point_lighting(vec4 lightPosRange, vec3 lightColor, vec3 lightAttenuation, vec3 samplePos, vec3 n, vec3 viewDir)
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
		
		// Attenuate the light
		float att = 1.0 / dot(lightAttenuation, vec3(1.0, distance, distance*distance));

		return vec3(1.0f,1.0f,1.0f) * att;
	}
}


void main()
{
    vec3 diffuseMat = texture(u_diffuseTex, v_uv1).rgb;  
	vec3 diffuseLight = vec3(0.0005f, 0.0005f, 0.0005f) * max(0, dot(v_normal.xyz, u_lightDir));

    vec3 n = normalize(v_normal.xyz);
	
    vec3 color = vec3(0,0,0);
	vec3 viewDir = normalize(u_viewPos - v_pos);
	
	// Spot Light
	color += calc_spot_lighting(u_lightPosRange, u_lightColor, u_lightAttenuation, u_lightSpot, v_pos, n, viewDir);
	
	// 4 Point Lights
	color += calc_point_lighting(u_lightPosRange1, u_lightColor1, u_lightAttenuation1, v_pos, n, viewDir);
	color += calc_point_lighting(u_lightPosRange2, u_lightColor2, u_lightAttenuation2, v_pos, n, viewDir);
	color += calc_point_lighting(u_lightPosRange3, u_lightColor3, u_lightAttenuation3, v_pos, n, viewDir);
	color += calc_point_lighting(u_lightPosRange4, u_lightColor4, u_lightAttenuation4, v_pos, n, viewDir);

	// Diffuse and ambient light
	color += (u_ambientLight + diffuseLight) * diffuseMat;

	PixelColor = vec4(color, 1.0);
}

