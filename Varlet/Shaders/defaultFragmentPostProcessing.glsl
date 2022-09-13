#version 460

uniform sampler2D u_MainTexture;
uniform float u_Gamma;
uniform bool u_HDR;
uniform float u_Exposure;

in vec2 texCoord;

out vec4 fragColor;

void main()
{
	vec4 color = texture(u_MainTexture, texCoord);
	vec3 processedColor = color.rgb;

	if (u_HDR)
		processedColor /= processedColor + 1.f;

	processedColor = 1.f - exp(-processedColor * u_Exposure);

	fragColor = vec4(pow(processedColor.rgb, vec3(1.f / u_Gamma)), color.a);
}