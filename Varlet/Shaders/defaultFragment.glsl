#version 460

struct PointLight
{
    vec4 color;
    vec4 position;
    float linearAttenuation;
    float quadraticAttenuation;
    bool isActive;
};

#define MAX_POINTS_LIGHT_AMOUNT 10

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
    int u_renderId;
};

layout(std140, binding = 1) uniform Illumination
{
    PointLight u_pointsLight[MAX_POINTS_LIGHT_AMOUNT];
};

uniform float u_Ambient;
uniform float u_Shininess;

uniform sampler2D u_Deffuse;
uniform sampler2D u_Specular;

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

out vec4 fragColor;

vec3 CalculatePointLightFactors(in vec3 deffuseColor, in vec3 specularColor, in vec3 normal)
{
	vec3 factor = vec3(0.f);

	for (int i = 0; i < MAX_POINTS_LIGHT_AMOUNT; i++)
	{
		if (u_pointsLight[i].isActive == false)
			continue;

		const vec3 ambient = u_Ambient * deffuseColor * u_pointsLight[i].color.rbg;
	
		const vec3 directionToLight = normalize(u_pointsLight[i].position.xyz - fragPos);
		const float angle = max(dot(directionToLight, normal), 0.f);
		const vec3 deffuse = angle * deffuseColor * u_pointsLight[i].color.rgb;
	
		const vec3 viewDirection = normalize(fragPos - u_CameraPosition);
		const vec3 halfDirection = normalize(directionToLight - viewDirection);
		const float shininessCoefficient = pow(max(dot(halfDirection, normal), 0.f), u_Shininess);
		const vec3 specular = specularColor * shininessCoefficient;
	
		const float distance = distance(u_pointsLight[i].position.xyz, fragPos);
		const float attenuation = 1.f / (1.f + u_pointsLight[i].linearAttenuation * distance + u_pointsLight[i].quadraticAttenuation * distance * distance);
	
		factor += (ambient + deffuse + specular) * u_pointsLight[i].color.rgb * attenuation;
	}

	return factor;
}

void main()
{
	vec3 deffuseColor = texture(u_Deffuse, texCoord).rbg;
	vec3 specularColor = texture(u_Specular, texCoord).rbg;
	vec3 normalizedNormal = normalize(normal);

	vec3 pointLightFactor = CalculatePointLightFactors(deffuseColor, specularColor, normalizedNormal);

	fragColor = vec4(pointLightFactor, 1.f);
}